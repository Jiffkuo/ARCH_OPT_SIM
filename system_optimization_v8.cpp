/* File name : system_optimization_v8.cpp
   Data      : 2007.03.29
   Author    : Jiff (Tzu-Chi, Kuo)
   Mail      : jiffkuo@gmail.com
   Lab       : Low Power High Performance Lab
   Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
   Decription:
               1. According to my optimization methodology, there have 4 parts and steps:
                  Step1: Trace File Analysis
                  Step2: Initial communication architecture mapping
                  Step3: Power/performance analysis
                  Step4: Explore all possible mapping --> repeat
                  Step5: Meet User-define Constraint and suggest architecture --> program stop
               2. 20070330 Step1_1 readSystemSetting(), Step1_2 readFileToSetDFGs()
                      0411 Step1_1_1 readExeCyclesSetting()
               3. 20070331 Step1_3 analyzeCompCorrelate() --> 1. Counting the correlation between two different Component and 
                  20070401                                    2. Counting Component Utilization
                      0412                                    3. Import Component execution cycles from readExeCyclesSetting()
               4. 20070405 All nType relative function is close and nType changes to cID, for *.dfg records Component ID
                           Add Node of Data_In and Data_Out in class Node and according initial bus setting to calculate
                  20070406 the bus information BusInCycle, TC_In, BusOutCycle and TC_Out
                  	   Step2_1 read Sharedbus Configure --> 0418 Add check sharedBus repeat or not
                  	   Step2_2 connect to shared bus --> 0416 Revise to shared_bus_setting()
                      0407 Step2_3 calculated Transition Count
                      0409 Step2_4 calculated BusInOutCycles	v0:Step2_3 and Step2_4 use the same program flow and according bandwidth 
                                                                    to decide the TC an Cycles -->maybe have some accurately problems
                                                                   And if DataIn and DataOut are zero than TC and Cycle are zero
                      0416 Step2_2 connect to shared bus --> shared_bus_setting()
                      0416 Step2_5 connect_bus_system() --> 2. Insert Shared bus commnunication node, 
                                                                  SBReadNode is in front of the cID (execution cycle means request/grant cycles)
                                                                  SBWriteNode is in back of the cID
                      0419                                  1. Count Burst times if the node present the same component and SBRead or SBWrite
                                                                  Count how many times of SBRead/Write with Burst transfer
                      0417                                  3. Insert Bridge Node
                                                                  Bridge Execution Cycle is decided by two buses of BandWidth
               5. 20070409 Step3 analyzePowerPerformance()
               	      0421 Step3_1 analyzeComp_TC_Cycles()	v0: Analyze each component of total Read/Write times, In/Out TC and Cycles 
               		   Step3_2 accumulate_TC_Cycles()	v0: For present Step2_3 and Step2_4 result
               	      0416                                          Add accumulate total ComponentExeCycles
               	                                                v1: Three parts:Bus Cycle, Bus TC, and Comp Cycles
               	      0417                                            BusCycles=In/OutCycle+RWCycles
               	      0420                                          If Burst transfer 
               	      						      BusCycles=BusCycles-(BurstSize-1)*Bursttimes*SBRead/WriteExe
			   Step3_3 read Bus parameter Configure (halt 20070409 because there maybe have many parameters that use to analyze)
               
               6. 20070421 Analysis.h --> For store the analyze Read/Write, TC. Cycles in each componenet
                           Stet3_1_1 setting_analyze_parameter()
   Modified  :
   Bugs      :
*/

#include "fileio.h"
#include "dfg.h"
#include "SharedBus.h"
#include "Analysis.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <string>

// For Output file
ofstream fout_compcorrelate	("./outRuleRef/compcorrealte.txt",ios::out);
ofstream fout_power		("./outPower/simlation_power.txt",ios::out);
ofstream fout_performance	("./outPerformance/simlation_performance.txt",ios::out);
ofstream fout_result		("./outResult/simulation_result.txt",ios::out);
ofstream fout_original_SystemDfg	("./outResult/original_system_dfg.txt",ios::out);
ofstream fout_simulation_SystemDfg	("./outResult/simulation_system_dfg.txt",ios::out);
ofstream fout_original_Bus_System	("./outResult/original_Bus_system.txt",ios::out);
ofstream fout_simulation_Bus_SystemDfg	("./outResult/simulation_Bus_system_dfg.txt",ios::out);

// Parameter Seeting for every function common usage
int NumOfComp=0, NumOfMaster=0, NumOfSlave=0, NumOfSharedMem=0, NumOfSharedBus=0;
int NumOfSBReadBurst=0, NumOfSBWriteBurst=0;
int SBRead=0, SBWrite=0, DBRead=0, DBWrite=0, SynExe=0, BridgeExe=0;
int SBReadNode=990, SBWriteNode=991, DBReadNode=992, DBWriteNode=993, SynNode=994, BridgeNode=995;

// Using vector to declare temp register because we can not assume how many storage volume we need
// For System Component
vector<int> ComponentID;
vector<int> correlateCount;
vector<int> CompExeCycles;		// Component Execution Cycles

// For System Bus
vector<int> SharedBusID;
vector<int> BurstSizeArray;
vector<int> BandWidthArray;
vector<int> NumOfCompOnSharedBus;
vector<int> CompIDOnSharedbus;
vector<int> BurstReadCnt;
vector<int> BurstWriteCnt;
SharedBusSystem BusSystem;

// For Power/Performance analysis
int TotalCompCycles;
vector<int> CompCyclesSB;		// Component Cycles in the Shared bus
int TotalBusTransitionCount;		// SharedBus + DedicatedBus
vector<int> BusTransitionCountSB;	// SharedBus
vector<int> BusTransitionCountDB;	// DedicatedBus
int TotalBusCycles;			// All SharedBus
vector<int> BusCyclesSB;
vector<int> BusContention;
int TotalBridgeCycles;
AnalyzeSystem AnalyzeSys;

// 20070330 Step1_1 readSystemSetting --> including System parameter and ComponentID
void readSystemSetting()
{
	ifstream		fin("./inConfig/system_setting.cfg",ios::in);
	int			tempInt;		// temp store int number value
	char*			cstrLine = new char [LINE_SIZE]; //LINE_SIZE = 200
	string			tempStr;		// temp storing cstrLine for dealing with read data
	string			tempNumStr;		// for dealing with read number values
	bool			conti;			// for control extract of the bItem
	int			sItemIdx;		// for system Item index increasing
	int			intArr[ARR_SIZE] = { 0 };// temp intArr for constructing bus ARR_SIZE = 40
	int			intArrIdx;		// index of intArr for component
	tempInt = 0;		// clear for tempInt
	
	if(fin.fail()) 
	{	// The file does not exist ... 
		cout << "Error !! /inConfig/system_setting.cfg file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	}
	while(!fin.eof()) 
	{	
		fin.getline(cstrLine, LINE_SIZE);
		tempStr = cstrLine;
		rmWhitespace(tempStr);	// Remove whitespace at head/tail of a string
		rmSharpComment(tempStr);// Remove sharp(#) label
		if(!tempStr.empty()) 
		{	// check tempStr is valid
			conti = true;			// reset the flag to extract word
			sItemIdx = 0;			// for system Item index increasing
			intArrIdx = 4;
			while(conti) 
			{	// extract data from sLine(tempStr)
				tempNumStr = extractANumStr(tempStr, conti);		// extract number and remove number from tempStr
				tempInt = stringNumberToIntNumber(tempNumStr.c_str());	// convert to int number
				switch(sItemIdx) 
				{	// for node item index increasing
					case 0: ++sItemIdx;	intArr[0] = tempInt;	break;
					case 1: ++sItemIdx;	intArr[1] = tempInt;	break;
					case 2: ++sItemIdx;	intArr[2] = tempInt;	break;
					case 3:	++sItemIdx;	intArr[3] = tempInt;	break;
					case 4:			intArr[intArrIdx++] = tempInt; break;
					default: cout  << "Error, no match node item index! \n";
				}
			}
			NumOfComp = intArr[0];
			NumOfMaster = intArr[1];
			NumOfSlave = intArr[2];
			NumOfSharedMem = intArr[3];
			if(NumOfComp != intArrIdx-4){
				cout<<"Error: NumOfComponent is not Matching to Num of ComponentID!!"<<endl;
				system("pause"); 
				exit(0);
			}
			intArrIdx = 4;
			for (int i=0; i<intArr[0]; i++)
			{
				ComponentID.push_back(intArr[intArrIdx++]);
			}
		}
	}
	delete cstrLine;	// release new char[] 
}

// 20070411 Step1_1_1 read Shared/Dedicated Bus Read/Write Cycle, Synchronize Execution cycles and Component Execution cycles
void readExeCyclesSetting()
{
	ifstream		fin("./inConfig/execycles_setting.cfg",ios::in);
	int			tempInt;		// temp store int number value
	char*			cstrLine = new char [LINE_SIZE]; //LINE_SIZE = 200
	string			tempStr;		// temp storing cstrLine for dealing with read data
	string			tempNumStr;		// for dealing with read number values
	bool			conti;			// for control extract of the bItem
	int			sItemIdx;		// for system Item index increasing
	int			intArr[ARR_SIZE] = { 0 };// temp intArr for constructing bus ARR_SIZE = 40
	int			intArrIdx;		// index of intArr for component
	tempInt = 0;		// clear for tempInt
	
	if(fin.fail()) 
	{	// The file does not exist ... 
		cout << "Error !! /inConfig/Execycles_setting.cfg file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	}
	while(!fin.eof()) 
	{	
		fin.getline(cstrLine, LINE_SIZE);
		tempStr = cstrLine;
		rmWhitespace(tempStr);	// Remove whitespace at head/tail of a string
		rmSharpComment(tempStr);// Remove sharp(#) label
		if(!tempStr.empty()) 
		{	// check tempStr is valid
			conti = true;			// reset the flag to extract word
			sItemIdx = 0;			// for system Item index increasing
			intArrIdx = 5;
			while(conti) 
			{	// extract data from sLine(tempStr)
				tempNumStr = extractANumStr(tempStr, conti);		// extract number and remove number from tempStr
				tempInt = stringNumberToIntNumber(tempNumStr.c_str());	// convert to int number
				switch(sItemIdx) 
				{	// for node item index increasing
					case 0: ++sItemIdx;	intArr[0] = tempInt;	break;
					case 1: ++sItemIdx;	intArr[1] = tempInt;	break;
					case 2: ++sItemIdx;	intArr[2] = tempInt;	break;
					case 3:	++sItemIdx;	intArr[3] = tempInt;	break;
					case 4:	++sItemIdx;	intArr[4] = tempInt;	break;
					case 5:			intArr[intArrIdx++] = tempInt; break;
					default: cout  << "Error, no match node item index! \n";
				}
			}
			SBRead = intArr[0];
			SBWrite = intArr[1];
			DBRead = intArr[2];
			DBWrite = intArr[3];
			SynExe = intArr[4];
			if(NumOfComp != intArrIdx-5){
				cout<<"Error: NumOfComponent is not Matching to Num of Component Execution Cycles!!"<<endl;
				system("pause"); 
				exit(0);
			}
			intArrIdx = 5;
			for (int i=0; i<NumOfComp; i++)
			{
				CompExeCycles.push_back(intArr[intArrIdx++]);
			}
		}
	}
	delete cstrLine;	// release new char[] 
}

// 20070331 Step1_3 analyzeCompCorrelate --> Counting the correlation between two different Component 
void analyzeCompCorrelate(DFGs& allsystem)
{
	int	currentcID;	// For Store current Component nType
	int	nextcID;	// For Store next Component nType
	int	tempCorrIdx;	// For point correlate component location
	int	tempCompNum;	// For Record number of component
	int	tempCompID;	// For Record ComponentID
	int	tempCurrent, tempNext;    // Point to CorrelateCount vector array	
	int 	importCompExeCycleCount;  // To find the relative component execution cycles
	
	vector<int>::iterator	get_ComponentID;	
	list<Graph>::iterator	posGList;
	list<Node>::iterator	posNList;
	
	posGList = allsystem.gList.begin();
	posNList = posGList->nList.begin();

	// For extend correlateCount array and initial array value 
	tempCompNum = ComponentID.size();
	for(int i = 0 ; i < tempCompNum*tempCompNum; i++){
		correlateCount.push_back(0);
	}

	while(posNList != posGList->nList.end())
	{
		// Initial
		currentcID = posNList->get_cID();
		tempCurrent = 0;
		tempNext = 0;
		tempCompID = 0;
		tempCorrIdx = 0;
		importCompExeCycleCount=0;
		// For Serach what is current nType and set tempCurrent point
		for(get_ComponentID = ComponentID.begin(); get_ComponentID != ComponentID.end(); get_ComponentID++){
			if(ComponentID[tempCompID] == currentcID){
				tempCurrent = tempCompID*tempCompNum;	 //Record vector allocation
				tempCorrIdx = tempCompID*(tempCompNum+1);//Record vector allocation
				posNList->set_execution_cycles(CompExeCycles[importCompExeCycleCount]);
				tempCompID=0;
				importCompExeCycleCount=0;
			}else{
				tempCompID++;
				importCompExeCycleCount++;
			}
		}
		++posNList;
		nextcID = posNList->get_cID();
		
		// 20070401 Counting Component Utilization
		correlateCount[tempCorrIdx] = correlateCount[tempCorrIdx] +1;
		// For Serach what is next nType and set tempNext point
		for(get_ComponentID = ComponentID.begin(); get_ComponentID != ComponentID.end(); get_ComponentID++){
			if(ComponentID[tempCompID] == nextcID){
				tempNext = tempCompID;
				tempCompID=0;
			}else{
				tempCompID++;
			}
		}
		// Compare currentcID and nextcID, if not equal, accumulate currentcID correlation or currentcID utilization
		if(currentcID != nextcID){
			correlateCount[tempCurrent+tempNext] = correlateCount[tempCurrent+tempNext] +1;
		}else{
			//correlateCount[tempCurrent+tempNext] = correlateCount[tempCurrent+tempNext] +1;
		}
	}
	// Report /outRuleRef/compcorrealte.txt
	fout_compcorrelate<<"ComponentID";
	cout<<"ComponentID";
	for(int x=0; x<tempCompNum; x++){
		fout_compcorrelate<<setw(7)<<ComponentID[x];
		cout<<setw(7)<<ComponentID[x];
	}fout_compcorrelate<<endl;cout<<endl;
	for(int y=0; y<tempCompNum; y++){
		fout_compcorrelate<<ComponentID[y]<<"        ";
		cout<<ComponentID[y]<<"        ";
		for(int z=0; z<tempCompNum; z++){
			fout_compcorrelate<<setw(7)<<correlateCount[y*tempCompNum+z];
			cout<<setw(7)<<correlateCount[y*tempCompNum+z];
		}fout_compcorrelate<<endl;cout<<endl;
	}
}

// 20070406 Step2_1 Read Sharedbus configure
void readSharedBusCfg()
{
	ifstream		fin("./inConfig/sharedbus.cfg",ios::in);
	int			tempInt;		// temp store int number value
	char*			cstrLine = new char [LINE_SIZE];
	string			tempStr;		// temp storing cstrLine for dealing with read data
	string			tempNumStr;		// for dealing with read number values
	bool			conti;			// for control extract of the bItem 
	BusItem			bItem;			// enum declaration, bus item for constructing BusSystem
	int			bItemIdx;		// for bItem index increasing
	int			intArr[ARR_SIZE] = { 0 };// temp intArr for constructing bus ARR_SIZE = 40
	int			intArrIdx;		// index of intArr for component
	int			tempcheck=0;		// Check Shared Bus ID is the same?
	tempInt = 0;		// clear for tempInt
	
	if ( fin.fail() ) 
	{ // The file does not exist ... 
		cout << "Error !! sharedbus.cfg file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) 
	{	
		fin.getline( cstrLine, LINE_SIZE );
		tempStr = cstrLine;
		rmWhitespace( tempStr );	// Remove whitespace at head/tail of a string
		rmSharpComment( tempStr );	// Remove sharp(#) label
		if ( !tempStr.empty() ) 
		{	// check tempStr is valid
			conti = true;			// reset the flag to extract word
			bItem = NITEM_SHAREDBUS_ID;	// reset node item for constructing node
			bItemIdx = 0;			// for nItem index increasing 
			intArrIdx = 4;
			while ( conti ) 
			{	// extract data from sLine(tempStr)
				tempNumStr = extractANumStr( tempStr, conti );		// extract number and remove number from tempStr
				tempInt = stringNumberToIntNumber( tempNumStr.c_str() );// convert to int number
				switch ( bItemIdx ) 
				{	// for node item index increasing
					case 0: bItem = NITEM_SHAREDBUS_ID;	++bItemIdx;	intArr[0] = tempInt;	break;
					case 1: bItem = NITEM_BURSTSIZE; 	++bItemIdx;	intArr[1] = tempInt;	break;
					case 2: bItem = NITEM_BANDWIDTH; 	++bItemIdx;	intArr[2] = tempInt;	break;
					case 3: bItem = NITEM_COMPONENT_NUM;	++bItemIdx;	intArr[3] = tempInt;	break;
					case 4: bItem = NITEM_COMPONENT;		intArr[intArrIdx++] = tempInt;	break;
					default: cout  << "Error, no match node item index! \n";
				}
			}		
			if(intArr[3] != intArrIdx-4){
				cout<<"Error: NumOfComponent is not Matching to Num of ComponentID in SharedBus["<<intArr[0]<<"]!!"<<endl;
				system("pause"); 
				exit(0);
			}
			intArrIdx = 4;
			NumOfSharedBus = intArr[0];
			SharedBusID.push_back(intArr[0]);
			// 20070418 check SharedBus ID
			if(tempcheck==intArr[0]){
				cout<<"Error: Shared Bus ID is repeated !!"<<endl;
				system("pause"); 
				exit(0);
			}
			tempcheck=intArr[0];
			
			BurstSizeArray.push_back(intArr[1]);
			BandWidthArray.push_back(intArr[2]);
			NumOfCompOnSharedBus.push_back(intArr[3]);
			for (int i=0; i<intArr[3]; i++)
			{
				CompIDOnSharedbus.push_back(intArr[intArrIdx++]);
			}
		}
	}
	delete cstrLine;	// release new char[] 
}

// 20070406 Step2_2 shared_bus_setting()
void shared_bus_setting()
{
	SharedBus bus;
	Component comp;
	int intArrIdx=0; //20070407 For set CompIDOnSharedbus to component ID on the shared bus
	list<SharedBus>::iterator posSBList;
	
	BusSystem.sbList.push_back(bus);
	posSBList = BusSystem.sbList.begin();
	for(int i=0; i<NumOfSharedBus; i++)
	{
		if (i>0)
		{
			BusSystem.sbList.push_back(bus);
			++posSBList;
		}
		for (int j=0; j<NumOfCompOnSharedBus[i]; j++)
		{
			comp.set_cID(CompIDOnSharedbus[intArrIdx++]);
			posSBList->cList.push_back(comp);
		}
		posSBList->set_SharedBusID(SharedBusID[i]);
		posSBList->set_BurstSize(BurstSizeArray[i]);
		posSBList->set_BandWidth(BandWidthArray[i]);
		posSBList->set_numOfComponent();
	}
	BusSystem.set_numOfSharedBus();
}


// 20070406 Step2_3 calculateTransCount --> calculate transition count
void calculateTransCount(DFGs& allsystem)
{
	int currentcID;		// For Store current Component nType
	int tempCompNum;	// For Record number of component
	int tempBandwidth;	// For shift bit to calculate TC
	int tempDataIn=0;	// For Record Last DataIn
	int tempDataOut=0;	// For Record Last DataOut
	int tempXORIn=0;	// For record after XOR two datain
	int tempXOROut=0;	// For record after XOR two dataout
	int tempTCIn=0;		// For Record TC_In
	int tempTCOut=0;	// For Record TC_Out
	int tempIn;		// For store XOR Data_In result and shift
	int tempOut;		// For store XOR Data_Out result and shift
	bool TC_flag = false;	// true: calculate TC; false: not calculate

	list<Graph>::iterator	posGList;
	list<Node>::iterator	posNList;
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();	
	
	// If there has more than a shared bus, only to calculate TC in the relative components
	for(int i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		
		// For the new sharedbus
		tempDataIn=0;
		tempDataOut=0;
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempBandwidth = posSBList->get_BandWidth();

		while(posNList != posGList->nList.end())
		{
			// First differentiate component in system is the same with component in sharedbus
			currentcID = posNList->get_cID();
			for(int j=0; j<tempCompNum; j++){
				if(posCList->get_cID()==currentcID){
					tempXORIn = posNList->get_dataInput();
					tempXORIn = (tempXORIn ^ tempDataIn);
					tempXOROut = posNList->get_dataOutput();
					tempXOROut = (tempXOROut ^ tempDataOut);
					tempDataIn = posNList->get_dataInput();
					tempDataOut = posNList->get_dataOutput();
					TC_flag=true;
				}
				++posCList;
			}
			posCList = posSBList->cList.begin(); // Repoint

			while(TC_flag){
				tempIn = tempXORIn;
				tempOut = tempXOROut;
				for(int k=0; k<tempBandwidth; k++){
					if(tempDataIn==0){
						tempTCIn = 0;
					}else if((tempIn&1)==1){
						tempTCIn++;
					}
					tempIn = tempIn >> 1;
					
					if(tempDataOut==0){
						tempTCOut = 0;
					}else if((tempOut&1)==1){
						tempTCOut++;
					}
					tempOut = tempOut >> 1;
				}
				posNList->set_tcIn(tempTCIn);
				posNList->set_tcOut(tempTCOut);
				tempTCIn=0;
				tempTCOut=0;
				TC_flag=false;
			}
			++posNList;
		}
		++posSBList;
	}
}

// 20070409 Step2_4 calculate Bus In/Out Cycles, means how many cycles to transfer data in the shared bus
void calculateBusInOutCycles(DFGs& allsystem)
{
	int currentcID;		// For Store current Component nType
	int tempCompNum;	// For Record number of component in shared bus
	int tempBandwidth;	// For shift bit to calculate TC
	int tempCyclesIn=0;	// For record Bus In cycles 
	int tempCyclesOut=0;	// For record Bus Out cycles
	int tempTCIn=0;		// For temp store TC In
	int tempTCOut=0;	// For temp store TC Out
	int tempDataIn=0;	// For temp store Data In, if zero than Cycle = 0
	int tempDataOut=0;	// For temp store Data Out, if zero than cycle = 0
	bool Cycle_flag = false;
	
	list<Graph>::iterator	posGList;
	list<Node>::iterator	posNList;
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	
	// If there has more than a shared bus, noly to calculate cycle of the relative components in the same shared bus
	for(int i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		// For the new sharedbus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempBandwidth = posSBList->get_BandWidth();

		while(posNList != posGList->nList.end())
		{
			// First differentiate component in system is the same with component in sharedbus
			currentcID = posNList->get_cID();
			for(int j=0; j<tempCompNum; j++){
				if(posCList->get_cID()==currentcID){
					Cycle_flag=true;
					tempTCIn = posNList->get_tcIn();
					tempTCOut = posNList->get_tcOut();
					tempDataIn = posNList->get_dataInput();
					tempDataOut = posNList->get_dataOutput();
				}
				++posCList;
			}
			posCList = posSBList->cList.begin(); // Repoint the component in the shared bus

			while(Cycle_flag){
				// 20070409 v0 according banswidth to decide the bus cycle
				if(tempDataIn==0){
					tempCyclesIn=0;
				}else if(tempTCIn>tempBandwidth){
					while(tempTCIn<=tempBandwidth){
						tempTCIn-=tempBandwidth;
						tempCyclesIn+=1;
					}
				}else{
					tempCyclesIn+=1;
				}
				if(tempDataOut==0){
					tempCyclesOut=0;
				}else if(tempCyclesOut>tempBandwidth){
					while(tempCyclesOut<=tempBandwidth){
						tempCyclesOut-=tempBandwidth;
						tempCyclesOut+=1;
					}
				}else{
					tempCyclesOut+=1;
				}
				posNList->set_busInCycles(tempCyclesIn);
				posNList->set_busOutCycles(tempCyclesOut);
				tempCyclesIn=0;
				tempCyclesOut=0;
				Cycle_flag=false;
			}
			++posNList;
		}
		++posSBList;
	}
}

// 20070416 Step2_5 Connect bus system to systemDFG, 1. insert shraed bus Read and Write Node, if the same comp will merge node
//                                                   2. Bridge Node
void connect_bus_system(DFGs& allsystem)
{
	int currentcID;		// systemDFG current cID
	int nextcID;		// systemDFG next cID
	int currentnID;		// systemDFG current nID
	int nextnID;		// systemDFG next nID	
	int tempShareBusID;	// Shared bus ID --> set Nodes nID to present Shared bus node ID
	int tempCompNum;	// For Record number of component in shared bus
	int tempBurstSize;	// For merge SBRead and SBWrite according BurstSize;
	int cntSBReadNode=1;	// For record burst SBRead times
	int cntSBWriteNode=1;	// For record burst SEWrite times
	int cntCompNode=1;	// For record sequential component means burst transfer
	int tempnextscID;	// For record next cID and compare the current cID
	int i;			// For loop because there are many bus
	bool insert_flag=false;	// Check insert Node
	bool check_NList=true;	// Check insert Birdge
	bool burst_flag=false;	// Check Burst size and accumulate Burst times
	
	list<Graph>::iterator	posGList;
	list<Node>::iterator	posNList;
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	
	// 20070419 1. Accumulate Burst times if SBRead and SBWrite with the same component	
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	// If there has more than a shared bus, noly to calculate cycle of the relative components in the same shared bus
	for(i=0; i< NumOfSharedBus; i++){
		BurstReadCnt.push_back(0);
		BurstWriteCnt.push_back(0);
	}
	for(i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		// For the new sharedbus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempBurstSize = posSBList->get_BurstSize();	// Burst Size in the shared bus
		tempShareBusID = posSBList->get_SharedBusID();	// SharedbusID for present different shared bus, set to nID in systemDFG

		while(posNList != posGList->nList.end())
		{
			// First differentiate component in system is the same with component in sharedbus
			currentcID = posNList->get_cID();
			for(int j=0; j<tempCompNum; j++){
				if(posCList->get_cID()==currentcID){
					burst_flag=true;
					if(posNList->get_dataInput()!=0){cntSBReadNode++;}
					if(posNList->get_dataOutput()!=0){cntSBWriteNode++;}
				}
				++posCList;
			}
			posCList = posSBList->cList.begin(); // Repoint the component in the shared bus		
			++posNList;
			tempnextscID = posNList->get_cID();
			while(burst_flag){
				if(tempnextscID==currentcID){
					cntCompNode++;
					if(tempBurstSize==cntCompNode){
						cntCompNode=1;
						if(tempBurstSize==cntSBReadNode){NumOfSBReadBurst++;cntSBReadNode=1;}
						if(tempBurstSize==cntSBWriteNode){NumOfSBWriteBurst++;cntSBWriteNode=1;}
					}
				}
				else{cntCompNode=1;cntSBReadNode=1;cntSBWriteNode=1;}
				burst_flag=false;
			}
		}
		++posSBList;
		BurstReadCnt[i]=NumOfSBReadBurst;
		BurstWriteCnt[i]=NumOfSBWriteBurst;
	}

	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	
	// 2. Insert SharedBus Node
	// If there has more than a shared bus, noly to calculate cycle of the relative components in the same shared bus
	for(i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		// For the new sharedbus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempShareBusID = posSBList->get_SharedBusID();	// SharedbusID for present different shared bus, set to nID in systemDFG
		
		//Set Bus Node( int gid, int nid, int cid, int dip, int dop, int bic, int ti, int boc, int to) 
		Node sb_read(0, tempShareBusID, SBReadNode, 0,0,0,0,0,0);
		Node sb_write(0, tempShareBusID, SBWriteNode, 0,0,0,0,0,0);		
		
		while(posNList != posGList->nList.end())
		{
			// First differentiate component in system is the same with component in sharedbus
			currentcID = posNList->get_cID();
			for(int j=0; j<tempCompNum; j++){
				if(posCList->get_cID()==currentcID){
					insert_flag=true;
				}
				++posCList;
			}
			posCList = posSBList->cList.begin(); // Repoint the component in the shared bus
			check_NList=true;
			while(insert_flag){
				// Shared Bus Read Node
				if(posNList->get_dataInput()!=0){
					sb_read.set_busInCycles(posNList->get_busInCycles());
					sb_read.set_tcIn(posNList->get_tcIn());
					sb_read.set_dataInput(posNList->get_dataInput());
					sb_read.set_execution_cycles(SBRead);
					posGList->nList.insert(posNList, sb_read);
				}
				// Shared Bus Write Node
				if(posNList->get_dataOutput()!=0){
					sb_write.set_busOutCycles(posNList->get_busOutCycles());
					sb_write.set_tcOut(posNList->get_tcOut());
					sb_write.set_dataOutput(posNList->get_dataOutput());
					sb_write.set_execution_cycles(SBWrite);
					++posNList;
					posGList->nList.insert(posNList, sb_write);
					check_NList=false;
				}
				else{
					check_NList=true;
				}
				insert_flag=false;
			}
			// check posNList add yet?
			if(check_NList==true){
				++posNList;
			}
		}
		++posSBList;
	}
	
	// 20070417 3. Insert Bridge Node
	// System DFG
	posGList = allsystem.gList.begin();
	posNList = posGList->nList.begin();
	Node sb_bridge(0, 0, BridgeNode, 0,0,0,0,0,0);
	while(posNList != posGList->nList.end())
	{
		currentcID = posNList->get_cID();
		currentnID = posNList->get_nID();	
		++posNList;
		nextcID = posNList->get_cID();
		nextnID = posNList->get_nID();
		if((SBReadNode==currentcID || SBWriteNode==currentcID) && 
		   (SBReadNode==nextcID || SBWriteNode==nextcID)){	// Make Sure cID is SB
			if(currentnID!=nextnID){			// Different nID means Different SharedBus ID
				if(currentcID!=nextcID){		// Different cID means SBRead and SBWrite				
					//sb_bridge.set_busOutCycles(posNList->get_busOutCycles());
					// 0417 if SBWrite bandwidth is smaller than SBRead's, cycle is 1,
					if(BandWidthArray[currentnID-1]>BandWidthArray[nextnID-1]){
						BridgeExe=BandWidthArray[currentnID-1]/BandWidthArray[nextnID-1];
					}else{BridgeExe=1;}
					sb_bridge.set_execution_cycles(BridgeExe);
					posGList->nList.insert(posNList, sb_bridge);
				}
			}
		}
	}
}

// 20070406 Step2 Initial Communication Architecture Mapping
void commArchiMapping(DFGs& allsystem)
{
	int tempCount=0;	// for record how many time to read CompIDOnSharedbus
	list<Graph>::iterator posGList;
	list<Node>::iterator posNList;
	
	// 20070406 Step2_1 read Sharedbus Configure
	readSharedBusCfg();
	fout_result<<"NumOfSharedBus = "<<NumOfSharedBus<<endl<<endl;
	cout<<"NumOfSharedBus = "<<NumOfSharedBus<<endl<<endl;
	for(int i=0; i<NumOfSharedBus; i++){
		fout_result<<"SharedBusID = "<<SharedBusID[i]<<endl
		           <<" BurstSizeArray["<<SharedBusID[i]<<"] = "<<BurstSizeArray[i]<<endl
		           <<" BandWidthArray["<<SharedBusID[i]<<"] = "<<BandWidthArray[i]<<endl
		           <<" NumOfCompOnSharedBus["<<SharedBusID[i]<<"] = "<<NumOfCompOnSharedBus[i]<<endl;
		cout<<"SharedBusID = "<<SharedBusID[i]<<endl
		    <<" BurstSizeArray["<<SharedBusID[i]<<"] = "<<BurstSizeArray[i]<<endl
		    <<" BandWidthArray["<<SharedBusID[i]<<"] = "<<BandWidthArray[i]<<endl
		    <<" NumOfCompOnSharedBus["<<SharedBusID[i]<<"] = "<<NumOfCompOnSharedBus[i]<<endl;
		fout_result<<" CompIDOnSharedbus = ";
		cout<<" CompIDOnSharedbus = ";
		for(int y=0; y<NumOfCompOnSharedBus[i]; y++){
			fout_result<<CompIDOnSharedbus[tempCount]<<", ";
			cout<<CompIDOnSharedbus[tempCount]<<", ";
			tempCount++;
		}fout_result<<endl<<endl;
		cout<<endl<<endl;
	}
	cout<<"readSharedBusCfg is Completed!!"<<endl<<".........."<<endl;
	fout_result<<"readSharedBusCfg is Completed!!"<<endl<<".........."<<endl;
	
	// 20070406 Step2_2 shared_bus_setting, not actually connect it, I think I have to insert communicaiotn node
	shared_bus_setting();
	cout<<"shared_bus_setting is Completed!!"<<endl<<".........."<<endl;
	fout_result<<"shared_bus_setting is Completed!!"<<endl<<".........."<<endl;
	
	// 20070406 Step2_3 calculated Transition Count
	calculateTransCount(allsystem);
	cout<<"CalculateTransCount is Completed!!"<<endl<<".........."<<endl;
	fout_result<<"CalculateTransCount is Completed!!"<<endl<<".........."<<endl;
	
	// 20070409 Step2_4
	calculateBusInOutCycles(allsystem);
	cout<<"CalculateBusInOutCycles is Completed!!"<<endl<<".........."<<endl;
	fout_result<<"CalculateBusInOutCycles is Completed!!"<<endl<<".........."<<endl;
	// 20070416 Output system DFG not including Bus Node
	posGList = allsystem.gList.begin();
	posNList = posGList->nList.begin();
	while(posNList != posGList->nList.end()){
			fout_simulation_SystemDfg<<posNList->get_gID()
					    <<setw(4)<<posNList->get_nID()<<setw(4)<<posNList->get_cID()
					    <<setw(4)<<posNList->get_dataInput()<<setw(4)<<posNList->get_dataOutput()
					    <<setw(4)<<posNList->get_busInCycles()<<setw(4)<<posNList->get_tcIn()
					    <<setw(4)<<posNList->get_busOutCycles()<<setw(4)<<posNList->get_tcOut()
					    <<setw(4)<<posNList->get_execution_cycles()
					    <<endl;
			++posNList;	    
	}
	
	// 20070416 Step2_5
	connect_bus_system(allsystem);
	cout<<"connect_bus_system is Completed!!"<<endl<<".........."<<endl;
	fout_result<<"connect_bus_system is Completed!!"<<endl<<".........."<<endl;
}

// 20070421 Stet3_1_1 setting_analyze_parameter() according NumOfComp and componentID to extend list
void setting_analyze_parameter()
{
	int tempCompNum;
	ComponentOnSharedBus cosb;
	Parameter para;
	
	list<ComponentOnSharedBus>::iterator posCOSBList;
	list<Parameter>::iterator posPARAList;
	
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
			
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	
	AnalyzeSys.cosbList.push_back(cosb);
	posCOSBList = AnalyzeSys.cosbList.begin();
	
	for(int i=0; i<NumOfSharedBus; i++)
	{
		if(i>0){
			AnalyzeSys.cosbList.push_back(cosb);
			++posCOSBList;
		}
		// For new Bus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus	
		// Extend parameter according NumOfComp in the shared bus
		for(int j=0; j<tempCompNum; j++){
			posCOSBList->paraList.push_back(para);
		}
		posCOSBList->set_SharedBusID(posSBList->get_SharedBusID());
		posCOSBList->set_numOfcomp();
		// Initial ComponentID in Analysis
		posPARAList = posCOSBList->paraList.begin();
		for(int k=0; k<tempCompNum; k++){
			posPARAList->set_ComponentID(posCList->get_cID());
			++posPARAList;
			++posCList;
		}
		++posSBList;
	}
	
}

// 20070421 Step3_1 Analyze each component of total R/W times, In/Out TC and cycles in the sharedBus
void analyzeComp_TC_Cycles(DFGs allsystem)
{		
	setting_analyze_parameter();
	fout_compcorrelate<<AnalyzeSys;
	
	int currentcID;
	int tempCompNum;
	int tempShareBusID;
	int i;
	
	list<Graph>::iterator posGList;
	list<Node>::iterator posNList;
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
			
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	// According different shared bus to accumulate Transition Count and Bus Cycles
	for(i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		// For the new sharedbus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempShareBusID = posSBList->get_SharedBusID();	// SharedbusID for present different shared bus, set to nID in systemDFG
		fout_compcorrelate<<"Shared Bus["<<tempShareBusID<<"]"<<endl;
		fout_compcorrelate<<"# gID nID cID DataIn DataOut BusInCycles TC_In BusOutCycles TC_out ExeCycles"<<endl;
		while(posNList != posGList->nList.end())
		{
			currentcID = posNList->get_cID();
			if(SBWriteNode==currentcID){
				--posNList;	// back the last cID means component
				fout_compcorrelate<<posNList->get_gID()
					    <<setw(4)<<posNList->get_nID()<<setw(4)<<posNList->get_cID()
					    <<setw(4)<<posNList->get_dataInput()<<setw(4)<<posNList->get_dataOutput()
					    <<setw(4)<<posNList->get_busInCycles()<<setw(4)<<posNList->get_tcIn()
					    <<setw(4)<<posNList->get_busOutCycles()<<setw(4)<<posNList->get_tcOut()
					    <<setw(4)<<posNList->get_execution_cycles()
					    <<endl;
				++posNList;
			}
			++posNList;
		}
		++posSBList;
	}
	
}

// 20070409 Step3_2
void accumulate_TC_Cycles(DFGs allsystem)
{
	int currentcID;		// For Store current Component nType
	int currentnID;		// 0416 current Bus nID
	int tempCompNum;	// For Record number of component
	int tempShareBusID;	// 0416 Record SharedBus ID
	int tempTCIn=0;
	int tempTCOut=0;
	int tempBusInCycles=0;
	int tempBusOutCycles=0;
	int tempCompExeCycles=0;
	int tempRWCycles=0;
	int tempBridgeCycles=0;
	int i;
	bool accumulateBus_flag=false;	// 0416 for accumulate Bus Cycles and TC
	bool accumulateComp_flag=false;	// 0416 for accumulate Comp Cycles
	bool accumulateBridge_flag=false;	// 0417 for accumulate Bridge Cycles
	
	list<Graph>::iterator posGList;
	list<Node>::iterator posNList;
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	 
	for(i=0; i<NumOfSharedBus; i++){
		BusTransitionCountSB.push_back(0);
		BusCyclesSB.push_back(0);
		CompCyclesSB.push_back(0);	// 0416 Comp Cycles in the SharedBus
	}
	
	// Shared Bus
	posSBList = BusSystem.sbList.begin();
	posCList = posSBList->cList.begin();
	
	// According different shared bus to accumulate Transition Count and Bus Cycles
	for(i=0; i< NumOfSharedBus; i++)
	{
		// System DFG
		posGList = allsystem.gList.begin();
		posNList = posGList->nList.begin();
		// For the new sharedbus
		posCList = posSBList->cList.begin();		// 20070413 For repoint componentID in the Sharedbus
		tempCompNum = posSBList->get_numOfComponent();	// Number of components in the shared bus
		tempShareBusID = posSBList->get_SharedBusID();	// SharedbusID for present different shared bus, set to nID in systemDFG
		
		while(posNList != posGList->nList.end())
		{
			// First differentiate component in system is the same with component in sharedbus
			// 20070416 For Accumulate Component Cycles in the Shared Bus
			currentcID = posNList->get_cID();
			currentnID = posNList->get_nID();
			for(int j=0; j<tempCompNum; j++){
				if(posCList->get_cID()==currentcID){
					tempCompExeCycles = posNList->get_execution_cycles();
					accumulateComp_flag = true;
				}
				++posCList;
			}
			// 20070416 For Accumulate Bus Cycles and TC
			if((tempShareBusID==currentnID&&SBReadNode==currentcID) || 
			   (tempShareBusID==currentnID&&SBWriteNode==currentcID)){
				tempTCIn = posNList->get_tcIn();
				tempTCOut = posNList->get_tcOut();
				tempBusInCycles = posNList->get_busInCycles();
				tempBusOutCycles = posNList->get_busOutCycles();
				tempRWCycles = posNList->get_execution_cycles();
				accumulateBus_flag = true;
			}
			// 20040417 For Accumulate Bridge Cycles
			if(BridgeNode==currentcID){
				tempBridgeCycles = posNList->get_execution_cycles();
				accumulateBridge_flag = true;
			}
			posCList = posSBList->cList.begin(); // Repoint the component in the shared bus
			
			// Accumulate --> 20070409 Assume total = In + Out, not consider Read/Write signal 
			// 20070416 seperate two parts: Bus and Component
			while(accumulateComp_flag){
				CompCyclesSB[i]=CompCyclesSB[i]+tempCompExeCycles;
				accumulateComp_flag = false;
			}
			while(accumulateBus_flag){
				BusTransitionCountSB[i]=BusTransitionCountSB[i]+tempTCIn+tempTCOut;
				BusCyclesSB[i]=BusCyclesSB[i]+tempBusInCycles+tempBusOutCycles+tempRWCycles;
				accumulateBus_flag = false;
			}
			// 20070417 Bridge Cycles
			while(accumulateBridge_flag){
				TotalBridgeCycles=TotalBridgeCycles+tempBridgeCycles;
				accumulateBridge_flag = false;
			}
			++posNList;
		}
		++posSBList;
		
	}
}

// 20070409 Step3 analyze Power(Transition Count) and Performance(Cycles)
void analyzePowerPerformance(DFGs allsystem)
{
	// 20070421 Step3_1
	analyzeComp_TC_Cycles(allsystem);
	// 20070409 Step3_2
	accumulate_TC_Cycles(allsystem);
}


void main()
{
	vector<int>::iterator get_ComponentID;
	vector<int>::iterator get_CompExeCycles;
	list<Graph>::iterator	posGList;
	list<Node>::iterator	posNList;

	fout_result<<endl<<"*****Start optimization*****"<<endl<<endl;
	cout<<"*****Start optimization*****"<<endl<<endl;
	
	// CPU Execution Time Report
	time_t start, stop;
	double timeused;
	time(&start);
	
	// Step1 Trace File Analyze
	// Step1_1 readSystemSetting
		fout_result<<"Reading System Configure...."<<endl;
		cout<<"Reading System Setting...."<<endl;
	readSystemSetting();
		fout_result<<" NumOfComp = "<<NumOfComp<<endl<<" NumOfMaster = "<<NumOfMaster<<endl
			<<" NumOfSlave = "<<NumOfSlave<<endl<<" NumOfSharedMem = "<<NumOfSharedMem<<endl;
		cout<<" NumOfComp = "<<NumOfComp<<endl<<" NumOfMaster = "<<NumOfMaster<<endl
			<<" NumOfSlave = "<<NumOfSlave<<endl<<" NumOfSharedMem = "<<NumOfSharedMem<<endl;
		fout_result<<" ComponentID = ";
		cout<<" ComponentID = ";
		for(get_ComponentID = ComponentID.begin(); get_ComponentID != ComponentID.end(); get_ComponentID++){ 
			fout_result<<*get_ComponentID<<", ";
			cout<<*get_ComponentID<<", ";
		}fout_result<<endl<<endl;
		cout<<endl<<endl;
	// Step1_1_1 readExeCyclesSetting()
		fout_result<<"Reading Execution Cycles Configure...."<<endl;
		cout<<"Reading Execution Cycles Setting...."<<endl;
	readExeCyclesSetting();
		fout_result<<" SBRead = "<<SBRead<<" SBWrite = "<<SBWrite<<endl
			<<" DBRead = "<<DBRead<<" DBWrite = "<<DBWrite<<endl
			<<" SynExe = "<<SynExe<<" BridgeExe = "<<BridgeExe<<endl;
		cout<<" SBRead = "<<SBRead<<" SBWrite = "<<SBWrite<<endl
			<<" DBRead = "<<DBRead<<" DBWrite = "<<DBWrite<<endl
			<<" SynExe = "<<SynExe<<" BridgeExe = "<<BridgeExe<<endl;
		fout_result<<" ComponentID = ";
		cout<<" ComponentID = ";
		for(get_ComponentID = ComponentID.begin(); get_ComponentID != ComponentID.end(); get_ComponentID++){ 
			fout_result<<setw(4)<<*get_ComponentID<<", ";
			cout<<setw(4)<<*get_ComponentID<<", ";
		}fout_result<<endl;
		cout<<endl;
		fout_result<<" Execycles =   ";
		cout<<" Execycles =   ";
		for(get_CompExeCycles = CompExeCycles.begin(); get_CompExeCycles != CompExeCycles.end(); get_CompExeCycles++){ 
			fout_result<<setw(4)<<*get_CompExeCycles<<", ";
			cout<<setw(4)<<*get_CompExeCycles<<", ";
		}fout_result<<endl<<endl;
		cout<<endl<<endl;

	// Step1_2 readFileToSetDFGs
	DFGs SystemDfg;
		fout_result<<"Read Trace File..."<<endl<<endl;
		cout<<"Read Trace File..."<<endl<<endl;
	readFileToSetDFGs("./inConfig/trace_all_system.all", SystemDfg);
		fout_original_SystemDfg<<SystemDfg;
		fout_result<<"Analyze System Utilization and Correlation...."<<endl<<endl;
		cout<<"Analyze System Utilization and Correlation...."<<endl<<endl;	
	// Step1_3 Analyze Component Correlate
	analyzeCompCorrelate(SystemDfg);
		fout_result<<endl<<"====AnalyzeCompCorrelate Finished===="<<endl<<endl;
		cout<<endl<<"====AnalyzeCompCorrelate Finished===="<<endl<<endl;

	// Step2 Initial communication architecture mapping
		fout_result<<"Initial Communication Architecture Mapping...."<<endl<<endl;
		cout<<"Initial Communication Architecture Mapping...."<<endl<<endl;
		fout_simulation_SystemDfg<<"# gID nID cID DataIn DataOut BusInCycles TC_In BusOutCycles TC_out ExeCycles"<<endl;
		fout_simulation_Bus_SystemDfg<<"# gID nID cID DataIn DataOut BusInCycles TC_In BusOutCycles TC_out ExeCycles"<<endl;
	commArchiMapping(SystemDfg);
		fout_original_Bus_System<<BusSystem;
		posGList = SystemDfg.gList.begin();
		posNList = posGList->nList.begin();
		while(posNList != posGList->nList.end()){
			fout_simulation_Bus_SystemDfg<<posNList->get_gID()
					    <<setw(4)<<posNList->get_nID()<<setw(4)<<posNList->get_cID()
					    <<setw(4)<<posNList->get_dataInput()<<setw(4)<<posNList->get_dataOutput()
					    <<setw(4)<<posNList->get_busInCycles()<<setw(4)<<posNList->get_tcIn()
					    <<setw(4)<<posNList->get_busOutCycles()<<setw(4)<<posNList->get_tcOut()
					    <<setw(4)<<posNList->get_execution_cycles()
					    <<endl;
			++posNList;	    
		}
		fout_result<<endl<<"====Initial communication architecture mapping Finished===="<<endl<<endl;
		cout<<endl<<"====Initial communication architecture mapping Finished===="<<endl<<endl;

	// Step3 Initial Power and Performance Analysis
		fout_result<<"Initial Power and Performance Analyzing...."<<endl<<endl;
		cout<<"Initial Initial Power and Performance Analyzing...."<<endl<<endl;
	analyzePowerPerformance(SystemDfg);
		int i;
		// Power
		for(i=0; i<NumOfSharedBus; i++){
			fout_result<<"BusTransitionCountSB["<<SharedBusID[i]<<"] = "<<BusTransitionCountSB[i]<<endl;
			fout_power<<"BusTransitionCountSB["<<SharedBusID[i]<<"] = "<<BusTransitionCountSB[i]<<endl;
			cout<<"BusTransitionCountSB["<<SharedBusID[i]<<"] = "<<BusTransitionCountSB[i]<<endl;
			TotalBusTransitionCount = TotalBusTransitionCount + BusTransitionCountSB[i];
		}
		fout_result<<"TotalBusTransitionCount = "<<TotalBusTransitionCount<<endl<<endl;
		fout_power<<"TotalBusTransitionCount = "<<TotalBusTransitionCount<<endl<<endl;
		cout<<"TotalBusTransitionCount = "<<TotalBusTransitionCount<<endl<<endl;
		// Performance
		// 20070417 Bridge Cycles
		fout_result<<"TotalBridgeCycles = "<<TotalBridgeCycles<<endl<<endl;
		fout_performance<<"TotalBridgeCycles = "<<TotalBridgeCycles<<endl<<endl;
		cout<<"TotalBridgeCycles = "<<TotalBridgeCycles<<endl<<endl;
		
		// Single Transfer
		fout_result<<"----Single Transfer----"<<endl;
		fout_performance<<"----Single Transfer----"<<endl;
		cout<<"----Single Transfer----"<<endl;
		for(i=0; i<NumOfSharedBus; i++){
			fout_result<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl;
			fout_performance<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl;
			cout<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl;
			TotalBusCycles = TotalBusCycles + BusCyclesSB[i];
		}
		// TotalSBBusCycles + Bridge Cycles
		fout_result<<endl<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;
		fout_performance<<endl<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;
		cout<<endl<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;		
		fout_result<<"-----------------------"<<endl<<endl;
		fout_performance<<"-----------------------"<<endl<<endl;
		cout<<"-----------------------"<<endl<<endl;
		
		// 20070419 Burst Transfer
		fout_result<<"----Burst Transfer----"<<endl;
		fout_performance<<"----Burst Transfer----"<<endl;
		cout<<"----Burst Transfer----"<<endl;
		// 20070419 Burst Transfer Cycles
		TotalBusCycles=0;
		for(i=0; i<NumOfSharedBus; i++){
			fout_result<<"BurstSizeSB["<<SharedBusID[i]<<"] = "<<BurstSizeArray[i]<<endl
				   <<"BurstReadCntSB["<<SharedBusID[i]<<"] = "<<BurstReadCnt[i]<<endl
				   <<"BurstWriteCntSB["<<SharedBusID[i]<<"] = "<<BurstWriteCnt[i]<<endl;
			fout_performance<<"BurstSizeSB["<<SharedBusID[i]<<"] = "<<BurstSizeArray[i]<<endl
					<<"BurstReadCntSB["<<SharedBusID[i]<<"] = "<<BurstReadCnt[i]<<endl
					<<"BurstWriteCntSB["<<SharedBusID[i]<<"] = "<<BurstWriteCnt[i]<<endl;
			cout<<"BurstSizeSB["<<SharedBusID[i]<<"] = "<<BurstSizeArray[i]<<endl
			    <<"BurstReadCntSB["<<SharedBusID[i]<<"] = "<<BurstReadCnt[i]<<endl
			    <<"BurstWriteCntSB["<<SharedBusID[i]<<"] = "<<BurstWriteCnt[i]<<endl;
			BusCyclesSB[i] = BusCyclesSB[i] - ((BurstSizeArray[i]-1)*(SBRead*BurstReadCnt[i]+SBWrite*BurstWriteCnt[i]));
			fout_result<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl<<endl;
			fout_performance<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl<<endl;
			cout<<"BusCyclesSB["<<SharedBusID[i]<<"] = "<<BusCyclesSB[i]<<endl<<endl;
			TotalBusCycles = TotalBusCycles + BusCyclesSB[i];
		}
		// TotalSBBusCycles + Bridge Cycles
		fout_result<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;
		fout_performance<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;
		cout<<"TotalBusCycles = "<<TotalBusCycles+TotalBridgeCycles<<endl;
		fout_result<<"-----------------------"<<endl<<endl;
		fout_performance<<"-----------------------"<<endl<<endl;
		cout<<"-----------------------"<<endl<<endl;
		
		// 20070416 Component Execution Cycles
		fout_result<<"----Component Execution Cycles----"<<endl;
		fout_performance<<"----Component Execution Cycles----"<<endl;
		cout<<"----Component Execution Cycles----"<<endl;
		for(i=0; i<NumOfSharedBus; i++){			
			fout_result<<"CompCyclesSB["<<SharedBusID[i]<<"] = "<<CompCyclesSB[i]<<endl;
			fout_performance<<"CompCyclesSB["<<SharedBusID[i]<<"] = "<<CompCyclesSB[i]<<endl;
			cout<<"CompCyclesSB["<<SharedBusID[i]<<"] = "<<CompCyclesSB[i]<<endl;
			TotalCompCycles = TotalCompCycles + CompCyclesSB[i];
		}
		fout_result<<"TotalCompCycles = "<<TotalCompCycles<<endl;
		fout_performance<<"TotalCompCycles = "<<TotalCompCycles<<endl;
		cout<<"TotalCompCycles = "<<TotalCompCycles<<endl;
		fout_result<<"----------------------------------"<<endl;
		fout_performance<<"----------------------------------"<<endl;
		cout<<"----------------------------------"<<endl;
		
		fout_result<<endl<<"====Initial Power and Performance Analyzing Finished===="<<endl<<endl;
		cout<<endl<<"====Initial Initial Power and Performance Analyzing Finished===="<<endl<<endl;

	// CPU Execution Time Report
	time(&stop);
	timeused = difftime(stop, start);
	fout_result<<"Total time used is "<<timeused<<endl;
	cout<<"Total time used is "<<timeused<<endl;

}
