// DFG : dfg.h
#ifndef _dfg_h_
#define _dfg_h_
// ***************************************************************************
// 注意: 輸入node和edge格式, gID,nID和eID必需按照小到大順序輸入, 
//       否則將會有誤, 因為有些使用 iterator 遞增以取資料 !
// ***************************************************************************
// System Optimization used key word: sysopt
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************

// ** Include library 
#include <iostream>
#include <iomanip>			// for setw(), setprecision(), setf()
#include <list>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <stdlib.h>
using namespace std;
//#include "globalConst.h"


// ** class pre-declaration 
class NodeID;
class DFGs;
class Graph;
class Node;
class Edge;


// ** functions pre-declaration
template <class T1>
	T1 maxPositiveNumber( T1 a, T1 b );
template <class T2>
	T2 minPositiveNumber( T2 a, T2 b );
//void ASAP( Graph& g );
//void ALAP( Graph& g, int tc );


// ======= The content is Node and Edge spec. =======
// ** Node Type
enum NodeType {
	NONE=118, 		     // 118, not assigned 20070405
	NOP=119,	             // 119, for outputs only 20070405
	/* 20070405 because I dont need fix NodeType, I need that I could configure it
	UVLC=101,	// 101, 101~110 are reserved for VLC nodes
	Lum16DC, 	// 102,
	ChrDC, 		// 103, 
	Lum16AC, 	// 104,
	Luma, 		// 105, 
	ChrAC, 		// 106, 
	
	Lum16DC_Hada_IQ=111, // 111,  111~119 are reserved for IDCT&IQ nodes
	ChrDC_Hada_IQ,       // 112,
	Lum16AC_IQ,          // 113,
	Luma_IQ,             // 114,
	ChrAC_IQ,            // 115,
	Luma_IDCT,           // 116,
	Chroma_IDCT,         // 117,
	
	NONE, 		     // 118, not assigned
	NOP,	             // 119, for outputs only
	SB_CM_read,          // 120,  120~123 are used for CM (communication)
	SB_CM_write,         // 121
	DB_CM_read,          // 122
	DB_CM_write,         // 123
	
	IntrapredLuma16x16=131,  // 131,  131~136 are used for MC (Motion Compensation)
	IntrapredLuma,           // 132
	InterpredLuma,           // 133
	IntrapredChroma,         // 134
	InterpredChroma,         // 135
	SynExecution,            // 136
	
	GetStrength=141,         // 141,  141~144 are used for deblocking filter
	YEdgeLoop,               // 142
	UEdgeLoop,               // 143
	VEdgeLoop                // 144
	*/
}; // End of enum
 // Total node type
const int NumOfNodeType = 44;
const string NodeTypeName[] = {"UVLC", "Lum16DC", "ChrDC", "Lum16AC", "Luma", "ChrAC", " ", " ", " ", " ", 
                               "Lum16DC_Hada_IQ", "ChrDC_Hada_IQ", "Lum16AC_IQ", "Luma_IQ", "ChrAC_IQ", "Luma_IDCT", "Chroma_IDCT",
                               "NONE", "NOP", "SB_CM_read", "SB_CM_write", "DB_CM_read", "DB_CM_write", " ", " ", " ", " ", " ", " ", " ",
                               "IntrapredLuma16x16", "IntrapredLuma", "InterpredLuma", "IntrapredChroma", "InterpredChroma", "SynExecution", " ", " ", " ", " ",
                               "GetStrength", "YEdgeLoop", "UEdgeLoop", "VEdgeLoop"};
const int NodeDelayTable[NumOfNodeType] = { 1,1,1,3,1,  3,0,0,1,1,  1,0,1,0,0, 0 };
// 20070405 original it for execution cycle, but I need to configure it by myself
const int NodeCycleTable[NumOfNodeType] = { 10, 40, 30, 40, 40, 40, 0,0,0,0, 
                                            5, 5, 5, 5, 5, 5, 5, 0,0, 
                                            4, 4, 2, 2, 0,0,0,0,0,0,0, 
                                            30, 15, 15, 10, 10, 2, 0, 0, 0, 0,
                                            20, 10, 10, 10};
/*const int NodeCycleTable[NumOfNodeType] = { 5, 10, 8, 10, 10, 8, 0,0,0,0, 
                                            3, 2, 3, 3, 2, 3, 2, 0,0, 
                                            4, 4, 2, 2, 0,0,0,0,0,0,0, 
                                            10, 10, 10, 5, 5, 1, 0, 0, 0, 0,
                                            8, 5, 3, 3};*/
const int NodeNumOfInp[NumOfNodeType] = { 1,2,2,2,2,  2,-1,0,1,2,  2,-1,4,-1,1, 1 }; // -1: don't care
const int NodeNumOfOut[NumOfNodeType] = { 1,1,1,1,1,  1,1,0,1,1,  1,-1,2,-1,0, 1 }; // -1: don't care
 // Active node type
//const int NumOfActNode = 6;
//const NodeType ActNode[NumOfActNode] = {ADD, DIV, MUL, SUB, COMP, ADDSUB};
 // Max # of nodes in one DFG
const int MaxSizeOfNodeinDFG = 500; 
 // convert int to NodeType for set node type
//NodeType intToNodeType ( const int nt );
 // node size
const int NodeSize[NumOfNodeType] = { 0,0,1,10,1, 20,0,0,0,1, 1,2,0,0,0, 0 }; // need to modified from the synthesis results
const int MaxNodeSize = 10; 


// ** Edge Type
enum EdgeType { 
	FW, 		// 0, forward: an ancestor to a child node
	BW, 		// 1, backward: a child node to its ancestor
	CRS, 		// 2, coss: no relationship
	CTRL, 		// 3, control signal
	DUMY, 		// 4, dummt edge: no effect
	OUT, 		// 5, output edge: no effect
	INP 		// 6, input edge with FW properties: no effect
}; // End of enum
 // Total edge type
const int NumOfEdgeType = 7;
const string EdgeTypeName[] = {"FW", "BW", "CRS", "CTRL", "DUMY", "OUT", "INP"};
 // convert int to EdgeType for set edge type
EdgeType intToEdgeType ( const int et );
// ======= End of the Node and Edge spec. =======


//// ** idList class
//class idList {
//private:
//	 int ID;
//public:
//	idList(): ID(-1) { }
//	idList( int id ) { ID = id; }
//	~idList() { }
//};// End of idList class


// *** Class NodeID *** 
class NodeID {
public:
	int			gID;
	int			nID;
	// constructors
	NodeID(): gID(-1), nID(-1) { }
	NodeID( int gid, int nid ) { gID = gid; nID = nid; }
	~NodeID() { }
	// member functions
	inline void set_gID( int id ) { gID = id; }
	inline int get_gID() { return gID; }
	inline void set_nID( int id ) { nID = id; }
	inline int get_nID() { return nID; }
	// operator functions
	void operator=( NodeID& aID ); 
	bool operator==( NodeID& aID ); 
	bool operator!=( NodeID& aID ); 
	// friend functions
	friend ostream& operator<<( ostream& os, const NodeID& aID ) {
		os << "(" << aID.gID << "-" << aID.nID << ") ";
		return os;
	}
};// *** End of NodeID *** 


// ** Edge class
class Edge {
 private:
	 int gID;		// graph ID 
	 int eID;		// edge ID 
	 EdgeType eType;	// edge type 
	 int eWeight;		// edge weight 
	 int partiID;		// partition ID ( start at 1 )
	 int regID;		// which register ID stores the data 
	 bool isCut;		// is cut: true, isn't cut: false 
	 int totLifeT;		// the total life time = beginTStep - endTStep 
 public:
	 int tempValue;		// for preparing any process using 
	 int from_nID;		// comes from node ID 
	 int to_nID;		// goes to node ID 
	 int beginTStep;	// edge begins at which time step 
	 int endTStep;		// edge ends at which time step 
	 string edgeNote;	// the note from the xxx.edge file # 
	 // constructor
	 Edge(): gID(-1), eID(-1), eType(DUMY), from_nID(-1), to_nID(-1), eWeight(1), tempValue(-1), 
		 beginTStep(0), endTStep(0), regID(-1), partiID(-1), totLifeT(-1), isCut(false) { }	// eWeight default = 1
	 Edge( int gid, int eid, EdgeType et, int fnid, int tnid ) { 
		 gID=gid; eID=eid; eType=et; from_nID=fnid; to_nID=tnid; eWeight=1; tempValue=-1; 
		 beginTStep=-1; endTStep=-1; regID=-1; partiID=-1; totLifeT=-1; isCut=false;		// eWeight default = 1
	 }	// use Graph class eList to construct initial edge !
	 ~Edge() { } 
	 inline void set_gID( int id ) { gID = id; }
	 inline int get_gID() { return gID; }
	 inline void set_eID( int id ) { eID = id; }
	 inline int get_eID() { return eID; }
	 inline void set_eType( EdgeType et ) { eType = et; }
	 inline EdgeType get_eType() { return eType; }
	 inline void set_from_nID( int id ) { from_nID = id; }
	 inline void set_to_nID( int id ) { from_nID = id; }
	 inline int get_from_nID() { return from_nID; }
	 inline int get_to_nID() { return to_nID; }
	 inline void set_eWeight( int w ) { eWeight = w; }
	 inline int get_eWeight() { return eWeight; }
	 inline void set_beginTStep( int t ) { beginTStep = t; }
	 inline void set_endTStep( int t ) { endTStep = t; }
	 inline int get_beginTStep() { return beginTStep; }
	 inline int get_endTStep() { return endTStep; }
	 inline void set_totLifeT( int t ) { totLifeT = t; }
	 inline void set_totLifeT() { totLifeT = endTStep - beginTStep; }
	 inline int get_totLifeT() { return totLifeT; }
	 inline void set_regID( int id ) { regID = id; }
	 inline int get_regID() { return regID; }
	 inline void set_partiID( int id ) { partiID = id; }
	 inline int get_partiID() { return partiID; } 
	 inline void set_tempValue( int val ) { tempValue = val; }
	 inline int get_tempValue() { return tempValue; }
	 inline void set_isCut( bool c ) { 
		 if( c == true ) isCut = true;
		 else isCut = false;
	 }
	 inline bool get_isCut() { return isCut; }
	 void set_edgeNote( string& aStr ) { edgeNote = aStr; } 
	 string& get_edgeNote() { return edgeNote; } 
	 // show functions
	 void showEdgeSpec(); 
	 void showEdgeData(); 
	 // operator functions
	 bool operator==( Edge & e );	// check *this is equal to e or not
	 void operator=( const Edge& sourceEdge );
	 // friend functions
	 friend ostream& operator<<( ostream& os, const Edge& e ) {		// for show_eList in DFGs class
		 os << "gID= " << e.gID << " eID= " << e.eID << "; eType= " << EdgeTypeName[e.eType] 
			<< "; from_nID= " << e.from_nID << " to_nID= " << e.to_nID  
			<< "; eWeight= " << e.eWeight << "; tv= " << e.tempValue << ";\n" 
			<< "beginTStep= " << e.beginTStep << " endTStep= " << e.endTStep << "; totLifeT: " << e.totLifeT 
			<< "; regID= " << e.regID << "; partiID= " << e.partiID << "; isCut: " << e.isCut << "; "; 
		 if ( e.eType == INP || e.eType == OUT ) { 
			 os << e.edgeNote; 
		 } 
		 os << "\n==========\n";
		 return os;
	 }
};// End of Edge class


// ** Node class
// sysopt
class Node {
 private:
	 int nID;		// node ID
	 int numOfSucc;		// number of successor nodes
	 int numOfPred;		// number of predecessor nodes
 public:
	 int gID;		// graph ID
	 NodeType nType;	// node type
	 int tStep;		// time step which operation starts	( start at 1 )
	 int dTime;		// delay time of operation
	 int tASAP;		// time step by ASAP
	 int tALAP;		// time step by ALAP
	 int mobility;		// mobility = the valid range of time = (tALAP - tASAP)
	 list<int> successor;	// the successor nodes of this node
	 list<int> predecessor;	// the predecessor nodes of this node
	 int partiID;		// partition ID ( start at 1 )
	 int clusID;		// cluster ID ( start at 1 )
	 int assignID;		// resource assignment ID ( start at 1 )
	 int schPairClusID;	// clustering two nodes have the same successor, rs-scheduling need ( start at 1 )
	 int schShiftClusID;	// clustering all predecessor of shiftNode and those predecessor's predecessors ( follow certain schPairClusID )
	 int nSize;		// node size
	 //---sysopt------------------------------------------------------------------------------------------------------
	 int cID;		// 20070405 Component ID
	 int dataInput;		// 20070405 for orginal component Data Input
	 int dataOutput;	// 20070405 for orginal component Data Output
	 bool input_valid;  	// used by EX node
	 int start_time;  	// the start time of the node
	 int execution_cycles;  // the occupied clk cycles of the node (CM_write node would change by accumulating the busCycles of EX nodes)
	 int busInCycles;  	// the used bus cycles when reading data
	 int busOutCycles;  	// the used bus cycles when writing data by this node (will be accumulated by CM_write node)
	 int busInBandwidth;  	// bandwidth of DB or SB
	 int busOutBandwidth;  	// bandwidth of DB or SB
	 int tcOut;  		// the transition count of output data
	 int tcIn;  		// the transition count of input data
	 int InBus;  		// will be set up by analysis algorithm accroding to the bus.cfg  (the connected bus) (0 stands for no input)
	 int OutBus;  		// will be set up by analysis algorithm accroding to the bus.cfg  (the connected bus) (0 stands for no output)
	 int ComponentType;  	// will be set up by analysis algorithm accroding to the bus.cfg  (master or slave)
	 int priority;
	 //---------------------------------------------------------------------------------------------------------------
	 // constructor
	 Node(): gID(-1), nID(-1), nType(NONE), nSize(-1), tStep(-1), dTime(-1), tASAP(-1), tALAP(-1), 
		 partiID(-1), clusID(-1), assignID(-1), schPairClusID(-1), schShiftClusID(-1), numOfSucc(-1), numOfPred(-1) { }	
	 Node( int nid ) { gID=-1; nID=nid; nType=NONE; nSize=-1; tStep=-1; dTime=-1; tASAP=-1; tALAP=-1; 
		 partiID=-1; clusID=-1; assignID=-1; schPairClusID=-1; schShiftClusID=-1; numOfSucc=-1; numOfPred=-1; 
	 }	// use Graph class nList to construct initial node !
	 Node( int gid, int nid ) { gID=gid; nID=nid; nType=NONE; nSize=-1; tStep=-1; dTime=-1; tASAP=-1; tALAP=-1; 
		 partiID=-1; clusID=-1; assignID=-1; schPairClusID=-1; schShiftClusID=-1; numOfSucc=-1; numOfPred=-1; 
	 }	// use Graph class nList to construct initial node !
	 
	 //---sysopt-----relative to FIFO.cpp and constructNodeList-------------------------------------------------------
	 Node( int gid, int nid, int cid, int dip, int dop, int bic, int ti, int boc, int to) 
	 {
	 	//nType=nt;
	 	gID=gid; nID=nid; cID=cid; dataInput=dip; dataOutput=dop;
	 	execution_cycles=0; input_valid=false; busInCycles=bic; tcIn=ti;
		busOutCycles=boc; tcOut=to; start_time=-1; InBus=0; OutBus=0; ComponentType=-1; priority=-1;
		 // modified
	 }	 // use Graph class nList to construct initial node !
	 //---------------------------------------------------------------------------------------------------------------
	 ~Node() { }
	 
	 inline void set_gID( int id ) { gID = id; }
	 inline int get_gID() { return gID; }
	 inline void set_nID( int id ) { nID = id; }
	 inline int get_nID() { return nID; }
	 inline void set_nType( NodeType nt ) { nType = nt; }
	 inline NodeType get_nType() { return nType; }
	 inline void set_nSize( int s ) { nSize = s; }
	 inline int get_nSize() { return nSize; }
	 inline void set_tStep( int t ) { tStep = t; }
	 inline int get_tStep() { return tStep; }
	 inline void set_dTime( NodeType nt ) { dTime = NodeDelayTable[nt]; }
	 inline int get_dTime() { return dTime; }
	 inline void set_tASAP( int t ) { tASAP = t; }
	 inline void set_tALAP( int t ) { tALAP = t; }
	 inline int get_tASAP() { return tASAP; }
	 inline int get_tALAP() { return tALAP; }
	 inline void set_mobility() { mobility = tALAP - tASAP; }
	 inline int get_mobility() { return mobility; }
	 inline void set_partiID( int id ) { partiID = id; }
	 inline int get_partiID() { return partiID; } 
	 inline void set_clusID( int id ) { clusID = id; }
	 inline int get_clusID() { return clusID; } 
	 inline void set_assignID( int id ) { assignID = id; }
	 inline int get_assignID() { return assignID; } 
	 inline void set_schPairClusID( int id ) { schPairClusID = id; }
	 inline int get_schPairClusID() { return schPairClusID; } 
	 inline void set_schShiftClusID( int id ) { schShiftClusID = id; }
	 inline int get_schShiftClusID() { return schShiftClusID; } 
	 //----sysopt--------------------------------------------------------------------------------------------------------
	 inline void set_cID( int cid ) { cID = cid; }	//20070405 set DFG of component ID
	 inline int get_cID() { return cID; }		//20070405 set DFG of component ID
	 inline void set_dataInput( int dip ) { dataInput = dip; }	//20070405 set DFG of component Data Input
	 inline int get_dataInput() { return dataInput; }		//20070405 set DFG of component Data Input
	 inline void set_dataOutput( int dop ) { dataOutput = dop; }	//20070405 set DFG of component Data Output
	 inline int get_dataOutput() { return dataOutput; }		//20070405 set DFG of component Data Output
	 inline void set_start_time( int Stime ) { start_time = Stime; }
	 inline int get_start_time() { return start_time; }
	 inline void set_execution_cycles( int cycles ) { execution_cycles = cycles; }
	 inline int get_execution_cycles() { return execution_cycles; }
	 inline void set_input_valid( bool valid ) { input_valid = valid; }
	 inline bool get_input_valid() { return input_valid; }
	 inline void set_busInCycles( int bic ) { busInCycles = bic; }
	 inline int get_busInCycles() { return busInCycles; }
	 inline void set_busInBandwidth( int bib ) { busInBandwidth = bib; }
	 inline int get_busInBandwidth() { return busInBandwidth; }
	 inline void set_tcIn( int ti ) { tcIn = ti; }
	 inline int get_tcIn() { return tcIn; }
	 inline void set_busOutCycles( int boc ) { busOutCycles = boc; }
	 inline int get_busOutCycles() { return busOutCycles; }
	 inline void set_busOutBandwidth( int bob ) { busOutBandwidth = bob; }
	 inline int get_busOutBandwidth() { return busOutBandwidth; }
	 inline void set_tcOut( int to ) { tcOut = to; }
	 inline int get_tcOut() { return tcOut; }
	 inline void set_InBus( int ib ) { InBus = ib; }
	 inline int get_InBus() { return InBus; }
	 inline void set_OutBus( int ob ) { OutBus = ob; }
	 inline int get_OutBus() { return OutBus; }
	 inline void set_ComponentType( int ct ) { ComponentType = ct; }	// 20070405 represent Master or Slave
	 inline int get_ComponentType() { return ComponentType; }
	 inline void set_priority( int pr ) { priority = pr; }
	 inline int get_priority() { return priority; }
	 //---------------------------------------------------------------------------------------------------------------
	 void insertSuccessor( int id ) { successor.push_back( id ); }		// construct by node file info !
	 void insertPredecessor( int id ) { predecessor.push_back( id ); }	// construct by pass throught already node set !
//	 void deleteSuccessor() { successor.pop_back(); }
//	 void deletePredecessor() { predecessor.pop_back(); }
	 inline void set_numOfSucc( int num ) { numOfSucc = num; }
	 void set_numOfSucc() { numOfSucc = successor.size(); }
	 inline int get_numOfSucc() { return numOfSucc; } 
	 inline void set_numOfPred( int num ) { numOfPred = num; }
	 void set_numOfPred() { numOfPred = predecessor.size(); }
	 inline int get_numOfPred() { return numOfPred; } 
	 // show functions
	 void showEdgeSpec();
	 void showEdgeData();
	 // operator functions
	 bool operator==( Node & n );	// check *this is equal to n or not
	 bool operator!=( Node & n );	// check *this is equal to n or not
	 void operator=( const Node& sourceNode );
	 // friend functions
	 friend ostream& operator<<( ostream& os, const Node& n ) {		// for show_nList in DFGs class
		 os <<"gID= "<<n.gID<<", nID= "<<n.nID<<", cID= "<<n.cID<<", dataIn= "<<n.dataInput<<", dataOut= "<<n.dataOutput
		 	<<", ex_cycles = "<<n.execution_cycles<<", startTime= "<<n.start_time
		 	<<", \nbusInCycles= "<<n.busInCycles<<", tcIn= "<<n.tcIn
		 	<<", \nbusOutCycles= "<<n.busOutCycles<<", tcOut= "<<n.tcOut
		 	<<", \nInBus= "<<n.InBus<<", OutBus= "<<n.OutBus<<", ComponentType= "<<n.ComponentType<<", priority= "<<n.priority
			<<"\n";    
			// modified: NodeTypeName[(n.nType)-101]
		 //os << "NumOfsuccessor : " << n.numOfSucc << ", ";
		 //copy( n.successor.begin(), n.successor.end(), ostream_iterator<int>(os," ->") );
		 //os << "\n" << "NumOfpredecessor : " << n.numOfPred << ", ";
		 //copy( n.predecessor.begin(), n.predecessor.end(), ostream_iterator<int>(os," ->") );
		 os << "\n==========\n";
		 return os;
	 }
	 // other functions
	 bool searchSucc( int id );
	 bool searchPred( int id );
	 int findCommonSucc( Node& n );		// return nID by int
	 int findCommonPred( Node& n );		// return nID by int
	 bool checkTC( int tc );	// check timing constraints, time step can't over tALAP
};// End of Node class


// ** Graph class
class Graph {
 private:
	 int gID;		// graph ID
	 int nTotalNum;		// total number of nodes
	 int beginTStep;	// the beginning time step of this graph (default:ASAP time)
	 int endTStep;		// the ending time step of this graph (default:ASAP time)
 public:
	 list<Node> nList;	// graph node list
	 list<Edge> eList;	// graph edge list
	 
	 // constructor
	 Graph(): gID(-1), nTotalNum(-1), beginTStep(-1), endTStep(-1) { }
	 Graph( int id ) { gID=id; nTotalNum=-1; beginTStep=-1; endTStep=-1; }	// use DFGs class gList to construct initial graph !
	 ~Graph() { }	
	 	void set_gID( int id );// { gID = id; }
	 inline int get_gID() { return gID; }
	 inline void set_nTotalNum() { nTotalNum = nList.size(); }
	 inline int get_nTotalNum() { return nTotalNum; }
	 inline void set_beginTStep( int t ) { beginTStep = t; }
	 inline int get_beginTStep() { return beginTStep; }
	 inline void set_endTStep( int t ) { endTStep = t; }
	 inline int get_endTStep() { return endTStep; }
	 // nList functions
	 bool find_nList( Node& n );	// find node
	 void addNode_nList( Node n );		// construct nodes 
	 void setNodePredList();
	 void show_nList();
	 bool findAndUpdataNode( Node n );
 	 Node& returnTheSucc( int succID );		// return node for access
//	 void delete_nList() { nList.pop_back(); }
	 // eList functions
	 bool find_eList( Edge& e );	// find edge
	 void addEdge_eList( Edge e );		// construct edges 
	 void show_eList();
//	 void delete_eList() { eList.pop_back(); }
	 // operator functions
	 bool operator==( Graph & g );		// check *this is equal to g or not
	 // friend functions
	 friend ostream& operator<<( ostream& os, const Graph& g ) {	// for show_gList in DFGs class
		 os << "gID = " << g.gID << "; nTotalNum = " << g.nTotalNum 
			<< "; beginfTStep = " << g.beginTStep << "; endTStep = "
			<< g.endTStep;
		 os << "\n----------\n";
		 os << "\n" << "nList : " << "\n";
		 copy( g.nList.begin(), g.nList.end(), ostream_iterator<Node>(os, "") );
		 os << "\n" << "eList : " << "\n";
		 copy( g.eList.begin(), g.eList.end(), ostream_iterator<Edge>(os, "") );
		 return os;
	 }
	 // other functions 
	 int returnEdgeWeight( Node& aNode, int aSucc );
	 int returnNodeClusID( int nID );
};// End of Graph class


// ** DFGs class
// sysopt
class DFGs {
 private:
	 int numOfGraph;		// number of graphs
	 int rsActNumOfType;		// the number of active resource type
	 int clusNumConstraint;		// user defined cluster number
	 int clusSizeConstraint;	// user defined cluster size constraint
	 int edgeCutNum;		// how many edges are cut
 public:
	 int tConstraint;		// timing constraint
	 list<Graph> gList;		// graph list
	 vector<int> rsConstraint;	// resource constraint (its index maps to the NodeType)
	 
	 // constructor
	 DFGs(): numOfGraph(-1), tConstraint(-1), rsActNumOfType(-1), edgeCutNum(-1), 
		 clusNumConstraint(2), clusSizeConstraint(-1) { }
	 ~DFGs() { }
	 
	 // menber functions 
	 inline void set_numOfGraph( int num ) { numOfGraph = num; }
	 inline int get_numOfGraph() { return numOfGraph; }
	 inline void set_tConstraint( int tc ) { tConstraint = tc; }
	 inline int get_tConstraint() { return tConstraint; }
	 inline void set_rsActNumOfType( int num ) { rsActNumOfType = num; }
	 inline int get_rsActNumOfType() { return rsActNumOfType; }
	 inline void set_clusNumConstraint( int cc ) { clusNumConstraint = cc; }
	 inline int get_clusNumConstraint() { return clusNumConstraint; }
	 inline void set_clusSizeConstraint( int cs ) { clusSizeConstraint = cs; }
	 inline int get_clusSizeConstraint() { return clusSizeConstraint; }
	 	void set_clusSizeConstraint(); 
	 inline void set_edgeCutNum( int num ) { edgeCutNum = num; }
	 inline int get_edgeCutNum() { return edgeCutNum; }
	 // operator functions
	 	void operator=( DFGs& tempDFGs );		// assignment operator 
	 // friend functions
	 friend ostream& operator<<( ostream& os, const DFGs& aDFGs ) {
		 os << "aDFGs : \n";
		 os << "numOfGraph : " << aDFGs.numOfGraph << ", tConstraint : " << aDFGs.tConstraint 
			<< ", edgeCutNum : " << aDFGs.edgeCutNum << endl
		    << "clusNumConstraints : " << aDFGs.clusNumConstraint 
		    << ", clusSizeConstraint : " << aDFGs.clusSizeConstraint << endl;
		 os << "rsConstraint vector : ";
		 copy( aDFGs.rsConstraint.begin(), aDFGs.rsConstraint.end(), ostream_iterator<int>(os," ") );
		 os << "\n----------\n";
		 copy( aDFGs.gList.begin(), aDFGs.gList.end(), ostream_iterator<Graph>(os," ") );
		 os << "\n======= End of aDFGs =======\n";
		 return os;
	 }
	 // gList functions
	 bool find_gList( Graph & g );		// checks the gid is already in gList or not, by (Graph & g)
	 bool find_gList( int id );		// checks the gid is already in gList or not, by (int id)
	 void addGraph_gList( int id );		// construct graphs
	 void show_gList();
	 bool findAndUpdataNodeInGList( Node n );		// Updata node, it is useful in the algorithm
	 Node& findAndAccessNodeInGList( Node& n );		// Find and return this Node for access
	 Node& findAndAccessNodeInGList( int gid, int nid );
	 bool findNodeInGList( int gid, int nid );		// for the kWayPartition will have no this node in this partiID 
	 bool findAndUpdataEdgeInGList( Edge e );		// Updata edge, it is useful in the algorithm
	 Edge& findAndAccessEdgeInGList( Edge& e );		// Find and return this Edge for access
	 Edge& findAndAccessEdgeInGList( int gid, int eid );
	 Edge& findAndAccessEdgeInGList( int gid, int from_nID, int to_nID ); 
//	 void delete_gList() { gList.pop_back(); }
	 // access nList functions
	 void constructNodeList ( int intArr[] );
	 void setNodeProdecessor();
	 void setNumOfNodeInGraph();
	 bool DFGsNotEmpty();
//	 void deleteNodeList() { }
	 // access eList functions
	 void constructEdgeList ( int intArr[], string eNote );
	 void setEdgeLifeTime();
//	 void deleteEdgeList() { }
	 // rsConstraint functions
	 void init_rsConstraint();
	 void set_rsConstraint( NodeType nt, int num );
	 bool check_rsConstraint( NodeType nt, int num );
	 bool check_rsConstraint();
	 void show_rsConstraint();
	 // run ASAP/ALAP
	 //void runASAP();
	 //void runALAP();
	 // other functions
	 int returnWeightBetweenNodes( Node& s, Node& d );
	 void updataEdgeLiftTime();
	 void updataGraphEndTS();
	 //void setIsCutAndCalEdgeCutNum();
	 //void setIsCutAndCalEdgeCutNumForKWay(); 
	 int getClusterSize( int cID ); 
	 int getNumOfNodeInCluster( int cID ); 
	 //bool checkPartitionSizeForContinueFMPartiOrNot(); 
};// End of DFGs class


#endif	// End of _dfg_h_
