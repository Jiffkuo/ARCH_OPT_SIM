// ********************************************************************
// fileio.h 
//     1. Read files to setup the DFGs. (Read)
//     2. Generate the output file for DFGShow format. (Write)
//     3. Generate the output file to record the DFGs cout. (Write)
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************
#ifndef _fileio_h_
#define _fileio_h_
// **********************************************
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <stdlib.h>			// for _itoa() "int" translate to "string"
#include <iomanip>			// for width(), setw(), setprecision(), setf()
#include <ctime>			// for <time.h>{struct tm, clock(), asctime()}, <ctime>{GetCurrentTime()} 
#include <vector>

using namespace std;
#include "dfg.h"

// Global Constants Definition
const int	LINE_SIZE = 200;	// temp cstrLine size for getline()
const int	ARR_SIZE = 40;		// temp intArr size
const string	SUBNAME_DFG = "dfg";	// file sub-name .dfg
const string	SUBNAME_TC = "tc";	// file sub-name .tc
const string	SUBNAME_RC = "rc";	// file sub-name .rc
const string	SUBNAME_EDGE = "edge";	// file sub-name .edge
const string	SUBNAME_STR = "str";	// file sub-name .str
// file sub-name
enum FileTag { 
	F_NONE, 
	F_DFG, 
	F_TC, 
	F_RC, 
	F_EDGE, 
	F_STR 
};	
// node item for construct nodes
// sysopt modified_20070405
enum NodeItem {
	NITEM_GID,
	NITEM_NID,
	NITEM_CID,
	NITEM_DATAIN,
	NITEM_DATAOUT,
	NITEM_BUSINCYCLES,
	NITEM_TC_IN,
	NITEM_BUSOUTCYCLES,
	NITEM_TC_OUT,
};	
// edge item for construct edges
enum EdgeItem {
	EITEM_GID,
	EITEM_EID,
	EITEM_ETYPE,
	EITEM_FROMNID,
	EITEM_TONID
};
// fu sub-type for FuncUnit 
enum FUSubType { 
	FU_NONE,		// 0, NONE 
	FU_ADD_RPL,		// 1, DW01_add rpl 
	FU_ADD_CLA,		// 2, DW01_add cla 
	FU_MUL_NBW,		// 3, DW02_mult nbw ( Booth Wallacw-tree if a_width + b_width > 41 ) 
	FU_SUB_CLA,		// 4, DW01_sub cla 
	FU_ADDSUB_CLA,	// 5, DW01_addsub cla 
	FU_DIV_CLA,		// 6, DW_div cla 
	FU_COMP_CLA,	// 7, DW01_cmp6 cla 
	FU_CNNTR_CLA	// 8, DW03_updn_ctr cla
}; 
// fu sub-type name for FuncUnit 
const string FUSubTypeName[] = { "none", "addrpl", "addcla", "mulnbw", "subcla", 
							   "addsubcla", "divcla", "compcla", "cnntrcla" };
// DataFlowRS type for DataFlowRS & CtrlSignal
enum DFRSType { 
	DFRS_NONE,		// 0, NONE 
	DFRS_FU,		// 1, FU 
	DFRS_MUX,		// 2, MUX 
	DFRS_REG		// 3, REG 
}; 
// DataFlowRS name for DataFlowRS & CtrlSignal
const string DFRSName[] = { "NONE", "FU", "MUX", "REG" }; 
// CtrlTypeName for DataFlowRS & CtrlSignal ( corresponging to DataFlowRS type )
const string CtrlTypeNameBeg[] = { "none", "none", "mux", "reg" }; 
const string CtrlTypeNameEnd[] = { "none", "none", "sel", "en" }; 
// mux type for DataFlowRS & CtrlSignal 
enum MuxType { 
	MUX_NONE,	// 0, none
	MUX_C,		// 1, c type, only one configuration  
	MUX_H,		// 2, h type, multiple configuration 
	MUX_U		// 3, u type, unknow configuration 
}; 
// MuxType name for DataFlowRS & CtrlSignal 
const string MuxTypeName[] = { "none", "c", "h", "u" }; 
// InterLine edge type for InterLine 
enum ILType { 
	IL_NONE,			// 0, NONE 
	IL_IMMEDIATE,		// 1, IMMEDIATE 
	IL_INPUT,			// 2, INPUT 
	IL_OUTPUT, 			// 3, OUTPUT 
	IL_FUOUT_TO_MUX,	// 4, FU OUT TO MUX
	IL_MUXOUT_TO_REG,	// 5, MUX OUT TO REG 
	IL_REGOUT_TO_MUX, 	// 6, REG OUT TO MUX 
	IL_MUXOUT_TO_FU		// 7, MUX OUT TO FU 
}; 
// InterLine edge type name for InterLine 
const string ILTypeName[] = { "none", "immediate", "input", "output", 
							  "fuout_to_mux", "muxout_to_reg", "regout_to_mux", "muxout_to_fu" }; 
// InterLine line name for DataFlowRS & CtrlSignal ( corresponging to DataFlowRS type )
const string ILLineNameBeg[] = { "none", "none", "mux", "reg" }; 
const string ILLineNameEnd = "out"; 


// class declaration 
class DFRSid; 
class CtrlSignal;
class InterLine;  
class DataFlowRS;
class RTLStruc; 


// class definition 
// Class DFRSid
class DFRSid { 
public: 
	DFRSType			dfrsType;		// FU, MUX, or REG 
	int					dfrsID;			// start at 0 in each dfrsType 
	string				dfrsName;		// printed out name 
	// constructor 
	DFRSid(): dfrsType(DFRS_NONE), dfrsID(-1) { dfrsName="--no name--"; } 
	~DFRSid() { } 
	// member functions 
	inline void set_dfrsType( DFRSType aType ) { dfrsType = aType; } 
	inline DFRSType get_dfrsType() { return dfrsType; } 
	inline void set_dfrsID( int id ) { dfrsID = id; } 
	inline int get_dfrsID() { return dfrsID; } 
	void set_dfrsName( string str ) { dfrsName = str; } 
	string get_dfrsName() { return dfrsName; } 
	void preset_dfrsName(); 
	// operator functions 
	void operator=( const DFRSid& aDFRSid ); 
	bool operator==( DFRSid& aDFRSid ); 
	bool operator!=( DFRSid& aDFRSid ); 
	// friend functions 
	friend ostream& operator<<( ostream& os, const DFRSid& aDFRSid ) { 
		os << aDFRSid.dfrsName; 
		return os;
	}
	// other functions 
}; // End of DFRSid 

// Class CtrlSignal 
class CtrlSignal { 
private: 
	int				ctrlID; 
	DFRSType		ctrlType; 
	string			ctrlName; 
	int				accessNum; 
	MuxType			muxConfType; 
public: 
	list<int>		eIDList;	// mapping to the eID of the DFGs
	list<int>		cstepList;	// cstep and its oppsite conf 
	list<int>		confList;	// cstep and its oppsite conf 
	// constructor 
	CtrlSignal(): ctrlID(-1), ctrlType(DFRS_NONE), accessNum(-1), muxConfType(MUX_NONE) { ctrlName = "none"; }
	~CtrlSignal() { } 
	// member functions 
	inline void set_ctrlID( int id ) { ctrlID = id; } 
	inline int get_ctrlID() { return ctrlID; } 
	inline void set_ctrlType( DFRSType t ) { ctrlType = t; } 
	inline DFRSType get_ctrlType() { return ctrlType; } 
	inline void set_ctrlName( string name ) { ctrlName = name; } 
	inline string get_ctrlName() { return ctrlName; } 
	inline void set_accessNum( int num ) { accessNum = num; } 
	inline int get_accessNum() { return accessNum; } 
	inline void set_muxConfType( MuxType t ) { muxConfType = t; } 
	inline MuxType get_muxConfType() { return muxConfType; } 
	void pushBack_eIDList( int id ) { eIDList.push_back( id ); } 
	void pushPop_eIDList() { eIDList.pop_back(); } 
	void copyin_eIDList( list<int>& aIDList ) { 
		eIDList.insert( eIDList.end(), aIDList.begin(), aIDList.end() ); }
	void pushBack_cstepList( int s ) { cstepList.push_back( s ); }
	void pushPop_cstepList() { cstepList.pop_back(); }
	void pushBack_confList( int c ) { confList.push_back( c ); }
	void pushPop_confList() { confList.pop_back(); }
	// operator functions 
	void operator=( CtrlSignal& aCS ); 
	// friend functions 
	friend ostream& operator<<( ostream& os, const CtrlSignal& aCtrlSignal ) { 
		os << aCtrlSignal.ctrlName << "  " << aCtrlSignal.accessNum; 
		if ( aCtrlSignal.ctrlType == DFRS_MUX ) { 
			os << " " << MuxTypeName[aCtrlSignal.muxConfType]; 
		} 
		os << endl; 
		copy( aCtrlSignal.cstepList.begin(), aCtrlSignal.cstepList.end(), ostream_iterator<int>(os, "  ") ); 
		os << endl; 
		copy( aCtrlSignal.confList.begin(), aCtrlSignal.confList.end(), ostream_iterator<int>(os, "  ") ); 
		os << endl; 
//copy(aCtrlSignal.eIDList.begin(),aCtrlSignal.eIDList.end(),ostream_iterator<int>(os," "));	os << endl;
		return os;
	}
	// other functions 
	void clear_CtrlSignal(); 
}; // End of CtrlSignal 

// Class InterLine 
class InterLine { 
private: 
	int				ilID; 
	string			ilName; 
	int				bitNum; 
	string			ilNote; 
	ILType			ilType; 
	bool			isCut;		// the edge cut mark		// -- 2005.11.21 added the edge cut mark --
public: 
	list<int>		eIDList;	// mapping to the eID of the DFGs 
	// constructor
	InterLine(): ilID(-1), bitNum(-1), ilType(IL_NONE), isCut(false) { ilName = "NONE"; ilNote = "NONE"; }
	~InterLine() { } 
	// member functions 
	inline void set_ilID( int id ) { ilID = id; } 
	inline int get_ilID() { return ilID; } 
	inline void set_ilName( string name ) { ilName = name; } 
	inline string get_ilName() { return ilName; } 
	inline void set_bitNum( int n ) { bitNum = n; } 
	inline int get_bitNum() { return bitNum; } 
	inline void set_ilNote( string note ) { ilNote = note; } 
	inline string get_ilNote() { return ilNote; } 
	inline void set_ilType( ILType t ) { ilType = t; } 
	inline ILType get_ilType() { return ilType; } 
	inline void set_isCut( bool tf ) { isCut = tf; } 
	inline int get_isCut() { return isCut; } 
	void pushBack_eIDList( int id ) { eIDList.push_back(id); } 
	void popBack_eIDList() { eIDList.pop_back(); } 
	void copyin_eIDList( list<int>& aIDList ) { 
		eIDList.insert( eIDList.end(), aIDList.begin(), aIDList.end() ); }
	// operator functions 
	void operator=( InterLine& aIL ); 
	// friend functions 
	friend ostream& operator<<( ostream& os, const InterLine& aInterLine ) { 
		os << "-- " << ILTypeName[aInterLine.ilType] << " " << aInterLine.ilName << " " 
		   << aInterLine.bitNum << " " << aInterLine.ilNote << endl; 
//copy(aInterLine.eIDList.begin(),aInterLine.eIDList.end(),ostream_iterator<int>(os," "));	os << endl;
//os << aInterLine.isCut << endl;
		return os; 
	} 
	// other functions 
	void clear_InterLine(); 
	bool searchEidOfInterLine( int eid ); 
}; // End of InterLine 

// Class DataFlowRS 
class DataFlowRS { 
private: 
	DFRSid				theDFRSid;			// dfrsType, dfrsID, and dfrsName for FU, MUX, and REG 
	NodeType			theFUType;			// NodeType is defined in dfg.h, only for FU 
	FUSubType			theFUSubType;		// the FU sub-type, only for FU 
	MuxType				muxConfType;	// only for MUX 
	int					bitNum;			// only for REG 
	int					inputNum;		// the number of inputs, for FU, MUX, and REG 
	int					outputNum;		// the number of outputs, for FU, MUX, and REG 
	int					rsPartiID;		// the partition ID number of the resource    // -- 2005.11.21 added the rsPartiID -- 
public: 
	list<int>			neIDList;			// map to DFGs nID or eID list for FU and REG 
	CtrlSignal			aCtrlSignal;		// control signal 
	list<InterLine>		inputILList;		// input InterLine list 
	InterLine			outputInterLine;	// output InterLine 
	// constructor
	DataFlowRS(): theFUType(NONE), theFUSubType(FU_NONE), muxConfType(MUX_NONE), bitNum(-1), 
		inputNum(-1), outputNum(-1), rsPartiID(-1) { }
	~DataFlowRS() { } 
	// member functions 
	void set_theDFRSid( DFRSid& aDFRSid ) { theDFRSid = aDFRSid; }  
	DFRSid get_theDFRSid() { return theDFRSid; } 
	inline void set_theFUType( NodeType nt ) { theFUType = nt; } 
	inline NodeType get_theFUType() { return theFUType; } 
	inline void set_theFUSubType( FUSubType fust ) { theFUSubType = fust; }
	inline FUSubType get_theFUSubType() { return theFUSubType; }
	inline void set_muxConfType( MuxType mt ) { muxConfType = mt; }
	inline MuxType get_muxConfType() { return muxConfType; }
	inline void set_bitNum( int n ) { bitNum = n; } 
	inline int get_bitNum() { return bitNum; } 
	inline void set_inputNum( int n ) { inputNum = n; } 
	inline int get_inputNum() { return inputNum; } 
	void set_inputNum() { inputNum = inputILList.size(); } 
	inline void set_outputNum( int n ) { outputNum = n; } 
	inline int get_outputNum() { return outputNum; } 
	inline void set_rsPartiID( int id ) { rsPartiID = id; }			// -- 2005.11.21 added the rsPartiID -- 
	inline int get_rsPartiID() { return rsPartiID; }				// -- 2005.11.21 added the rsPartiID -- 
	void pushBack_neIDList( int id ) { neIDList.push_back(id); } 
	void popBack_neIDList() { neIDList.pop_back(); } 
	void set_aCtrlSignal( CtrlSignal& aCS ) { aCtrlSignal = aCS; }  
	CtrlSignal get_aCtrlSignal() { return aCtrlSignal; } 
	void pushBack_inputILList( InterLine& aIL ) { inputILList.push_back(aIL); } 
	void popBack_inputILList() { inputILList.pop_back(); } 
	void pushFront_inputILList( InterLine& aIL ) { inputILList.push_front(aIL); } 
	void popFront_inputILList() { inputILList.pop_front(); } 
	void copyin_inputILList( list<InterLine>& aILList ) { 
		inputILList.insert( inputILList.end(), aILList.begin(), aILList.end() ); }  
	void set_outputInterLine( InterLine& aIL ) { outputInterLine = aIL; }  
	InterLine get_outputInterLine() { return outputInterLine; } 
	// operator functions 
	void operator=( DataFlowRS& aDFRS ); 
	// friend functions 
	friend ostream& operator<<( ostream& os, const DataFlowRS& aDataFlowRS ) { 
		DFRSid							tempDFRSid; 
		list<InterLine>					tempInputILList; 
		list<InterLine>::iterator		iterInputILList;
		string							tempName; 
		list<CtrlSignal>				tempCtrlSignalList; 
		tempDFRSid = aDataFlowRS.theDFRSid; 
		os << "#" << endl << "-- " << aDataFlowRS.theDFRSid << " " << aDataFlowRS.inputNum 
		   << " " << aDataFlowRS.outputNum << " "; 
		if ( tempDFRSid.get_dfrsType() == DFRS_FU ) { 
			os << FUSubTypeName[aDataFlowRS.theFUSubType] << endl; 
			// FU's inputILList 
			tempInputILList.insert( tempInputILList.end(), aDataFlowRS.inputILList.begin(), aDataFlowRS.inputILList.end() ); 
			iterInputILList = tempInputILList.begin(); 
			while ( iterInputILList != tempInputILList.end() ) { 
				tempName = iterInputILList->get_ilName(); 
				os << tempName << " "; 
				++iterInputILList; 
			} 
			os << endl; 
			// FU's outputInterLine	
			tempInputILList.clear(); 
			tempInputILList.push_back( aDataFlowRS.outputInterLine ); 
			tempName = tempInputILList.begin()->get_ilName(); 
			os << tempName << endl; 
//copy(aDataFlowRS.neIDList.begin(),aDataFlowRS.neIDList.end(),ostream_iterator<int>(os," "));	os << endl; 
		} 
		else if ( tempDFRSid.get_dfrsType() == DFRS_MUX ) { 
			os << MuxTypeName[aDataFlowRS.muxConfType] << endl; 
			// MUX's ctrl signal 
			tempCtrlSignalList.push_back( aDataFlowRS.aCtrlSignal ); 
			os << tempCtrlSignalList.begin()->get_ctrlName() << " " 
			   << tempCtrlSignalList.begin()->get_accessNum() << endl; 
			// MUX's inputILList 
			tempInputILList.insert( tempInputILList.end(), aDataFlowRS.inputILList.begin(), aDataFlowRS.inputILList.end() ); 
			iterInputILList = tempInputILList.begin(); 
			while ( iterInputILList != tempInputILList.end() ) { 
				tempName = iterInputILList->get_ilName(); 
				os << tempName << " "; 
				++iterInputILList; 
			} 
			os << endl; 
			// MUX's outputInterLine 
			tempInputILList.clear(); 
			tempInputILList.push_back( aDataFlowRS.outputInterLine ); 
			tempName = tempInputILList.begin()->get_ilName(); 
			os << tempName << endl; 
//copy(aDataFlowRS.neIDList.begin(),aDataFlowRS.neIDList.end(),ostream_iterator<int>(os," "));	os << endl; 
		} 
		else if ( tempDFRSid.get_dfrsType() == DFRS_REG ) { 
			os << aDataFlowRS.bitNum << endl; 
			// REG's ctrl signal 
			tempCtrlSignalList.push_back( aDataFlowRS.aCtrlSignal ); 
			os << tempCtrlSignalList.begin()->get_ctrlName() << " " 
			   << tempCtrlSignalList.begin()->get_accessNum() << endl; 
			// REG's inputILList 
			tempInputILList.insert( tempInputILList.end(), aDataFlowRS.inputILList.begin(), aDataFlowRS.inputILList.end() ); 
			iterInputILList = tempInputILList.begin(); 
			while ( iterInputILList != tempInputILList.end() ) { 
				tempName = iterInputILList->get_ilName(); 
				os << tempName << " "; 
				++iterInputILList; 
			} 
			os << endl; 
			// REG's outputInterLine 
			tempInputILList.clear(); 
			tempInputILList.push_back( aDataFlowRS.outputInterLine ); 
			tempName = tempInputILList.begin()->get_ilName(); 
			os << tempName << endl; 
//copy(aDataFlowRS.neIDList.begin(),aDataFlowRS.neIDList.end(),ostream_iterator<int>(os," "));	os << endl; 
		} 
		else { 
			cout << endl << "Warning, no this DFRSType !! " << endl; 
		} 
		return os; 
	} 
	// other functions 
	void clear_DataFlowRS(); 
	bool searchNEIDofNEIDList( int id );	// only for REG & MUX, FU need to use the other one 
	bool searchNEIDofEIDListofOutputInterLine( int id );	// only for FU, REG & MUX need to use the other one 
}; // End of DataFlowRS 

// Class RTLStruc 
class RTLStruc {
private: 
	int					numOfFU; 
	int					numOfMux; 
	int					numOfReg; 
public: 
	list<DataFlowRS>	aFUList; 
	list<DataFlowRS>	aMuxList; 
	list<DataFlowRS>	aRegList; 
	list<InterLine>		aInterLineList; 
	list<CtrlSignal>	aCtrlSignalList; 
	// constructor 
	RTLStruc(): numOfFU(0), numOfMux(0), numOfReg(0) { } 
	~RTLStruc() { } 
	// member functions 
	void set_numOfFU() { numOfFU = aFUList.size(); } 
	inline int get_numOfFU() { return numOfFU; } 
	void set_numOfMux() { numOfMux = aMuxList.size(); } 
	inline int get_numOfMux() { return numOfMux; } 
	void set_numOfReg() { numOfReg = aRegList.size(); } 
	inline int get_numOfReg() { return numOfReg; } 
	void pushBack_aFUList( DataFlowRS& aDFRS ) { aFUList.push_back( aDFRS ); } 
	void popBack_aFUList() { aFUList.pop_back(); } 
	void pushBack_aMuxList( DataFlowRS& aDFRS ) { aMuxList.push_back( aDFRS ); } 
	void popBack_aMuxList() { aMuxList.pop_back(); } 
	void pushBack_aRegList( DataFlowRS& aDFRS ) { aRegList.push_back( aDFRS ); } 
	void popBack_aRegList() { aRegList.pop_back(); } 
	void pushBack_aInterLineList( InterLine& aIL ) { aInterLineList.push_back( aIL ); } 
	void popBack_aInterLineList() { aInterLineList.pop_back(); } 
	void pushBack_aCtrlSignalList( CtrlSignal& aCS ) { aCtrlSignalList.push_back( aCS ); } 
	void popBack_aCtrlSignalList() { aCtrlSignalList.pop_back(); } 
	// friend functions 
	friend ostream& operator<<( ostream& os, const RTLStruc& aRTLStruc ) { 
//os << "The data flow resource : --------------------------------" << endl; 
//os << "*** aMuxList : ***\n"; 
		copy( aRTLStruc.aMuxList.begin(), aRTLStruc.aMuxList.end(), ostream_iterator<DataFlowRS>(os, "") ); 
		os << "# \n# \n# \n";	// the boundry between DFRS_MUX and DFRS_REG  
//os << "*** aRegList : ***\n"; 
		copy( aRTLStruc.aRegList.begin(), aRTLStruc.aRegList.end(), ostream_iterator<DataFlowRS>(os, "") ); 
		os << "# \n# \n# \n";	// the boundry between DFRS_REG and DFRS_FU 
//os << "*** aFUList : ***\n"; 
		copy( aRTLStruc.aFUList.begin(), aRTLStruc.aFUList.end(), ostream_iterator<DataFlowRS>(os, "") );
//os << "*** aInterLineList : ***\n"; 
//copy( aRTLStruc.aInterLineList.begin(), aRTLStruc.aInterLineList.end(), ostream_iterator<InterLine>(os, "") );
		os << "# \n# \n# \n";	// the boundry between DFRS and CS 
//os << "*** aCtrlSignalList : ***\n"; 
		copy( aRTLStruc.aCtrlSignalList.begin(), aRTLStruc.aCtrlSignalList.end(), ostream_iterator<CtrlSignal>(os, "") ); 
//os << endl << "------------------------------------------------------------" << endl; 
		return os; 
	} 
	// other functions 
	DataFlowRS& findAndAccessDFRS( DFRSid& aDFRSid ); 
	bool findAndUpdataDFRS( DataFlowRS& aDFRS ); 
	bool searchDFRS( DFRSid& aDFRSid ); 
	InterLine& findAndAccessIL( ILType ailtp, int eid ); 
	InterLine& findAndAccessIL( string& aILName ); 
	bool findAndUpdataIL( InterLine& aIL ); 
	void detailConnectingFuRegMux(); 
	void deleteRedundantMux(); 
	CtrlSignal& findAndAccessCS( string& aILName ); 
	bool findAndUpdataCS( CtrlSignal& aCS ); 
	void detailConstructCtrlSignal( DFGs& aDFGs ); 
}; // End of RTLStruc 


// 1. Read files to setup the DFGs. (Read)
void readFileToSetDFGs( char* filename, DFGs& aDFGs );	// the main of read file and set DFGs function
// ** (1). Read xxx.all
void preReadFile( char* filename, DFGs& aDFGs, vector<string>& filenameArray );
// ** Filename split into basename(prefix) and extname(suffix)
void fileSplit( string& fName, string& baseName, string& extName );
// ** File format label for construct graphs
FileTag fileFormat( const string& extn );
// ** Node type label for setting resource constraints
NodeType setNodeType( const string& op );
// ** (2)-dfg. Read xxx.dfg
void readDfgFile( string& fName, DFGs& aDFGs );
// ** (2)-tc. Read xxx.tc
void readTcFile( string& fName, DFGs& aDFGs );
// ** (2)-rc. Read xxx.rc
void readRcFile( string& fName, DFGs& aDFGs );
// (2)-edge. Read xxx.edge
void readEdgeFile( string& fName, DFGs& aDFGs );
// ** (2)-str. Read xxx.str
void readStrFile( string& fName, DFGs& aDFGs );


// 2. Generate the output file for DFGShow format. (Write)
void opf_DFGShow( DFGs& aDFGs );


// 3. Generate the output file to record the DFGs cout. (Write) 
void opf_DFGsLog( DFGs& aDFGs, char * fileName );


// 4. Generate the output file for VDHL generator. (Write)
void opf_DFGsToVHDLGen( char* fileCstr, DFGs& aDFGs ); 
string& findTheMappingFUNameByOPAssID( vector<string>& aFuNameVect, list<int>& mappingOpAssIDList, int opAssID ); 
int findTheMappingFUNumByOPAssID( list<int>& opAssIDList, list<int>& fuAssIDList, int aOPAssID ); 
int convertRegIDToMRegID( Edge& aEdge ); 
int findTheMappingRSRegIDByMRegID( list<int>& eMRegIDList, list<int>& rsRegIDList, int aMRegID  ); 
int findTheMappingRSMuxIDByMRegID( list<int>& eMRegIDListForMux, list<int>& rsMuxIDListForMux, int aMRegID ); 
bool checkTheSameIDList( list<int>& aIntList, list<int>& bIntList ); 
int searchPartiIDBySuccessorOfEdge( DFGs& aDFGs, Edge& tempEdge );		// -- 2005.11.21 added the rsPartiID -- 


// --------------------------------------------------------------------
// Other useful functions in the fileio

// ** ASCII_number_string_to_INT_number (with removing whitespace in the head/tail)
template <class Any>
Any myPow ( const Any& base, int ext ); 
int asciiToDigit( const char  d );
int stringNumberToIntNumber( const char* str );
// End of ASCII_number_string_to_INT_number ** 

// ** Remove whitespace at head/tail of a string 
void rmWhitespace( string& str );

// ** Remove sharp(#) comment
void rmSharpComment( string& tempStr );

// ** Extract a number string from the input string
string extractANumStr( string& str, bool& conti );
// --------------------------------------------------------------------


#endif 


