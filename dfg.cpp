// DFG : dfg.cpp
// ************************
// System Optimization used key word: sysopt
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************
#include "dfg.h"


// convert int to NodeType for set node type
NodeType intToNodeType ( const int nt ) {
	/* 20070405
	switch ( nt ) {
	case 101: return UVLC;
	case 102: return Lum16DC;
	case 103: return ChrDC;
	case 104: return Lum16AC;
	case 105: return Luma;
	case 106: return ChrAC;
	
	case 111: return Lum16DC_Hada_IQ;
	case 112: return ChrDC_Hada_IQ;
	case 113: return Lum16AC_IQ;
	case 114: return Luma_IQ;
	case 115: return ChrAC_IQ;
	case 116: return Luma_IDCT;
	case 117: return Chroma_IDCT;
	
	case 120: return SB_CM_read;
	case 121: return SB_CM_write;
	case 122: return DB_CM_read;
	case 123: return DB_CM_write;
	
	case 131: return IntrapredLuma16x16;
	case 132: return IntrapredLuma;
	case 133: return InterpredLuma;
	case 134: return IntrapredChroma;
	case 135: return InterpredChroma;
	case 136: return SynExecution;
	
	case 141: return GetStrength;
	case 142: return YEdgeLoop;
	case 143: return UEdgeLoop;
	case 144: return VEdgeLoop;
	//case : return NONE;
	//case : return NOP;
	//case : return DOUT;
	default: cout << "Error, int to NodeType !" << endl; 
	return NONE;
	}
	*/
	return NONE;
}// End of convert int to NodeType for set node type


// convert int to EdgeType for set edge type
EdgeType intToEdgeType ( const int et ) {
	switch ( et ) {
	case 0: return FW;
	case 1: return BW;
	case 2: return CRS;
	case 3: return CTRL;
	case 4: return DUMY;
	case 5: return OUT;
	case 6: return INP;
	default: cout << "Error, int to EdgeType !" << endl; 
		return DUMY;
	}
}// End of convert int to EdgeType for set edge type


// Class Edge
// show functions
void Edge::showEdgeSpec() { 
	cout << "gID" << setw(4) << "eID" << setw(6) << "eType" << setw(10) << "from_nID" << setw(8) << "to_nID" << setw(7) 
		 << "eWeight" << setw(12) << "beginTStep" << setw(10) << "endTStep" << setw(6) << "regID" << setw(6) << "isCut" 
		 << endl;
}
void Edge::showEdgeData() { 
	cout << gID << setw(4) << eID << setw(6) << eType << setw(10) << from_nID << setw(8) << to_nID << setw(7) 
		 << eWeight << setw(12) << beginTStep << setw(10) << endTStep << setw(6) << regID << setw(6) << isCut << endl;
}
// operator functions
bool Edge::operator==( Edge & e ) {	// check *this is equal to e or not
	if ( get_gID()==e.get_gID() && get_eID()==e.get_eID() ) { return true; }
	else { return false; }
}
void Edge::operator=( const Edge& sourceEdge ) {
	gID = sourceEdge.gID;
	eID = sourceEdge.eID;
	eType = sourceEdge.eType;
	from_nID = sourceEdge.from_nID;
	to_nID = sourceEdge.to_nID;
	eWeight = sourceEdge.eWeight;
	beginTStep = sourceEdge.beginTStep;
	endTStep = sourceEdge.endTStep;
	regID = sourceEdge.regID;
	partiID = sourceEdge.partiID;
	totLifeT = sourceEdge.totLifeT;
	isCut = sourceEdge.isCut;
	tempValue = sourceEdge.tempValue;
	edgeNote = sourceEdge.edgeNote; 
}
// End of Class Edge


// *** Class CGNodeID *** 
// operator functions
void NodeID::operator=( NodeID& aID ) {
	gID = aID.gID;
	nID = aID.nID;
}
bool NodeID::operator==( NodeID& aID ) {
	if ( gID==aID.gID && nID==aID.nID ) {
		return true;
	}
	else {
		return false;
	}
}
bool NodeID::operator!=( NodeID& aID ) {
	if ( gID==aID.gID && nID==aID.nID ) {
		return false;
	}
	else {
		return true;
	}
}
// *** End of CGNodeID *** 


// Class Node
// show functions
void Node::showEdgeSpec() { 
	cout << "gID" << setw(4) << "nID" << setw(6) << "nType" << setw(10) << "tStep" << setw(8) << "dTime" << setw(8) 
		 << "tASAP" << setw(8) << "tALAP" << setw(8) << "Mobility" << setw(8) << "schPairClusID" << setw(8) << "clusID" << setw(8)
		 << schPairClusID << endl;
}
void Node::showEdgeData() { 
	cout << gID << setw(4) << nID << setw(6) << nType << setw(10) << tStep << setw(8) << dTime << setw(8) 
		 << tASAP << setw(8) << tALAP << setw(8) << mobility << setw(8) << schPairClusID << setw(8) << clusID << setw(8) 
		 << schPairClusID << endl;
	cout << "Successor : ";
	copy( successor.begin(), successor.end(), ostream_iterator<int>(cout," -> ") );
	cout << endl;
	cout << "Predecessor : ";
	copy( predecessor.begin(), predecessor.end(), ostream_iterator<int>(cout," -> ") );
	cout << endl << endl;
}
// operator functions
bool Node::operator==( Node & n ) {	// check *this is equal to n or not
	if ( get_gID()==n.get_gID() && get_nID()==n.get_nID() ) { return true; }
	else { return false; }
}
bool Node::operator!=( Node & n ) {	// check *this is equal to n or not
	if ( get_gID()==n.get_gID() && get_nID()==n.get_nID() ) { return false; }
	else { return true; }
}
void Node::operator=( const Node& sourceNode ) {
	gID = sourceNode.gID;
	nID = sourceNode.nID;
	nType = sourceNode.nType;
	start_time = sourceNode.start_time;
	execution_cycles = sourceNode.execution_cycles;
	input_valid = sourceNode.input_valid;
	busInCycles = sourceNode.busInCycles;
	busInBandwidth = sourceNode.busInBandwidth;
	tcIn = sourceNode.tcIn;
	busOutCycles = sourceNode.busOutCycles;
	busOutBandwidth = sourceNode.busOutBandwidth;
	tcOut = sourceNode.tcOut;
	ComponentType = sourceNode.ComponentType;
	InBus = sourceNode.InBus;
	OutBus = sourceNode.OutBus;
	priority = sourceNode.priority;
}
// other functions
bool Node::searchSucc( int id ) {
	list<int>::iterator iterSucc;
	iterSucc = successor.begin();
	while ( iterSucc != successor.end() ) {
		if ( id == *iterSucc ) {
			return true;
		}
		else {
			++iterSucc;
		}
	}
	return false;
}
bool Node::searchPred( int id ) {
	list<int>::iterator iterPred;
	iterPred = predecessor.begin();
	while ( iterPred != predecessor.end() ) {
		if ( id == *iterPred ) {
			return true;
		}
		else {
			++iterPred;
		}
	}
	return false;
}
int Node::findCommonSucc( Node& n ) {
	list<int>::iterator iterSuccN;
	list<int>::iterator iterSuccThis;
	iterSuccN = n.successor.begin();
	iterSuccThis = successor.begin();
	while ( iterSuccThis != successor.end() ) {
		while ( iterSuccN != n.successor.end() ) {
			if ( *iterSuccThis == *iterSuccN ) {
				return *iterSuccThis;
			}
			else {
				++iterSuccN;
			}
		}
		++iterSuccThis;
	}
	return -1;	// if not find common succssor
}
int Node::findCommonPred( Node& n ) {
	list<int>::iterator iterPredN;
	list<int>::iterator iterPredThis;
	iterPredN = n.predecessor.begin();
	iterPredThis = predecessor.begin();
	while ( iterPredThis != predecessor.end() ) {
		while ( iterPredN != n.predecessor.end() ) {
			if ( *iterPredThis == *iterPredN ) {
				return *iterPredThis;
			}
			else {
				++iterPredN;
			}
		}
		++iterPredThis;
	}
	return -1;	// if not find common succssor
}
bool Node::checkTC( int t ) {	// check timing constraints, time step can't over tALAP
	if ( t <= tALAP ) { return true; }
	else { return false; }
}
// End of Class Node


// Class Graph
void Graph::set_gID( int id )   // modified to not only set the gID of this graph but also set the gID of each node in this graph
{
	gID = id;
	list<Node>::iterator posNode;         // 
	posNode = nList.begin();              // 
	while ( posNode != nList.end() ) {    // 
		posNode->set_gID(id);             // 
		++posNode;                        // 
	}                                     // 
}
// nList functions
bool Graph::find_nList( Node& n ) {	// find node
	list<Node>::iterator posNode;
	posNode = nList.begin();
	while ( posNode != nList.end() ) {
		if ( n == *posNode ) {
			return true;
		}
		else {
			++posNode;
		}
	}
	return false;
}
void Graph::addNode_nList( Node n ) {	// construct nodes 
	nList.push_back( n );
//	show_nList();																				// --test
}
void Graph::setNodePredList() { 
	list<int>				tempSucc;	// for assign tempSucc to successor
	list<Node>::iterator	posNList;	// for nList iterator
	list<Node>::iterator	posNListPred;	// for finding which nList node id want to add predecessor
	int					succNID;	// for finding which nList node id want to add predecessor
	int					predNID;	// for set predecessor node id into tempPred
	posNList = nList.begin();
	while ( posNList != nList.end() ) {
		tempSucc = posNList->successor;
		while ( !tempSucc.empty() ) {
			succNID = tempSucc.back();	// takes node id in the successor list as the node wants to add predecessor
			tempSucc.pop_back();	// remove the nid has been done
			predNID = posNList->get_nID();	// takes node id in the nList as predecessor 
			posNListPred = nList.begin();	// reset the searching pred. nID index of nList
			for ( int nidx=1; nidx<succNID; ++nidx ) {
				++posNListPred;	// index increasing, we can't justify equality between "int" and "int *class.get_nID()" 
			}						// so, using int comparison and index for nList
			posNListPred->insertPredecessor( predNID );
			posNListPred->set_numOfPred();
		}
		if ( tempSucc.empty() ) {
			posNList->set_numOfPred();	// if predecessor is empty, set_numOfPred = 0
		}
		++posNList;
	}
}
void Graph::show_nList() {
	cout << "Show_nList : " << endl;
	copy ( nList.begin(), nList.end(), 
		   ostream_iterator<Node>(cout," ") );	// need operator<< overload
	cout << endl;
}
bool Graph::findAndUpdataNode( Node n ) {
	list<Node>::iterator posNode;
	posNode = nList.begin();
	while ( posNode != nList.end() ) {
		if ( n == *posNode ) {
			*posNode = n;
			return true;
		}
		else {
			++posNode;
		}
	}
	return false;
}
Node& Graph::returnTheSucc( int succID ) {
	Node succNode( gID, succID );	// construct a temp succNode for searching
	list<Node>::iterator iterNList;
	iterNList = nList.begin();
	while ( iterNList != nList.end() ) {
		if ( succNode == *iterNList ) {
			return *iterNList;
		}
		else {
			++iterNList;
		}
	}
	cout << "Can't Graph::returnTheSucc !! \n";	// show the notice if return dummy Node
	exit(0);	// Error, exit program !!
//	return *iterNList;	// dummy Node, must avoid this return
}
// eList functions
bool Graph::find_eList( Edge& e ) {	// find edge
	list<Edge>::iterator posEdge;
	posEdge = eList.begin();
	while ( posEdge != eList.end() ) {
		if ( e == *posEdge ) {
			return true;
		}
		else {
			++posEdge;
		}
	}
	return false;
}
void Graph::addEdge_eList( Edge e ) {	// construct edges 
	eList.push_back( e );
//	show_eList();																				// --test
}
void Graph::show_eList() {
	cout << "Show_eList : " << endl;
	copy ( eList.begin(), eList.end(), 
		   ostream_iterator<Edge>(cout," ") ); // need operator<< overload
	cout << endl;
}
// operator functions
bool Graph::operator==( Graph & g ) {	// check *this is equal to g or not
	if ( get_gID() == g.get_gID() ) { return true; }
	else { return false; }
}
// other functions
int Graph::returnEdgeWeight( Node& aNode, int aSucc ) {
	list<Edge>::iterator		iterEList;
	iterEList = eList.begin();
	while ( iterEList != eList.end() ) {
		if ( (iterEList->get_from_nID() == aNode.get_nID() ) &&
			(iterEList->get_to_nID() == aSucc) ) {
			return iterEList->get_eWeight();
		}
		++iterEList;
	}
	return -1;
}
int Graph::returnNodeClusID( int nid ) {
	list<Node>::iterator		iterNList;
	iterNList = nList.begin();
	while ( iterNList != nList.end() ) {
		if ( iterNList->get_nID() == nid ) {
			return iterNList->get_clusID();
		}
		++iterNList;
	}
	return -1;
}
// End of Class Graph


// Class DFGs
// member functions 
// sysopt
void DFGs::set_clusSizeConstraint() { 
	list<Graph>::iterator		iterGList; 
	list<Node>::iterator		iterNList; 
	int				size; 
	int				maxSize;
	size = 0; 
	maxSize = 0; 
	iterGList = gList.begin(); 
	while ( iterGList != gList.end() ) { 
		iterNList = iterGList->nList.begin(); 
		while ( iterNList != iterGList->nList.end() ) { 
			size = NodeSize[iterNList->get_nType()]; 
			if ( maxSize < size ) { 
				maxSize = size; 
			} 
			++iterNList; 
		} 
		++iterGList; 
	} 
	clusSizeConstraint = maxSize; 
} 
// operator functions
void DFGs::operator=( DFGs& tempDFGs ) { 
	numOfGraph = tempDFGs.get_numOfGraph();	
	rsActNumOfType = tempDFGs.get_rsActNumOfType();	
	clusNumConstraint = tempDFGs.get_clusNumConstraint();
	clusSizeConstraint = tempDFGs.get_clusSizeConstraint();	
	edgeCutNum = tempDFGs.get_edgeCutNum();	
	tConstraint = tempDFGs.get_tConstraint(); 
	gList.clear(); 
	gList.insert( gList.end(), tempDFGs.gList.begin(), tempDFGs.gList.end() ); 
	rsConstraint.clear(); 
	rsConstraint.insert( rsConstraint.end(), tempDFGs.rsConstraint.begin(), tempDFGs.rsConstraint.end() ); 
} 
// gList functions
bool DFGs::find_gList( Graph & g ) {	// checks the gid is already in gList or not, by (Graph & g)
	list<Graph>::iterator pos;
	if ( !gList.empty() ) {
		pos = gList.begin();
		do {
			if ( g.get_gID() == pos->get_gID() ) {	// use "pos->" as the gList element to use its class
				return true;
			}
			else {
				++pos;
			}
		} while( pos != gList.end() );
	}
	return false;
}
bool DFGs::find_gList( int id ) {	// checks the gid is already in gList or not, by (int id)
	Graph tempG(id);
	list<Graph>::iterator pos;
	pos = gList.begin();
	do {
		if ( tempG == *pos ) {	// use "*pos" as the gList element to use its class
			return true;
		}
		else {
			++pos;
		}
	} while( pos != gList.end() );
	return false;
}
void DFGs::addGraph_gList( int id ) {	// construct graphs
	Graph tempG(id);
	if ( !find_gList( tempG ) ) {
		gList.push_back( id );
	}
}
void DFGs::show_gList() {
	cout << "Show_gList : " << endl;
	copy ( gList.begin(), gList.end(), 
		   ostream_iterator<Graph>(cout," ") ); // need operator<< overload
	cout << endl;
}
bool DFGs::findAndUpdataNodeInGList( Node n ) {		// Updata node, it is useful in the algorithm
	list<Graph>::iterator posGList;
	posGList = gList.begin();
	while ( posGList != gList.end() ) {
		if ( posGList->find_nList(n) ) {
			 posGList->findAndUpdataNode(n);
			 return true;
		}
		else { 
			++posGList;
		}
	}
	return false;
}
Node& DFGs::findAndAccessNodeInGList( Node& n ) {	// Find and return this Node for access
	list<Graph>::iterator		posGList;
	list<Node>::iterator		posNList;
	posGList = gList.begin();
	while ( posGList != gList.end() ) {
		posNList = posGList->nList.begin();
		while ( posNList != posGList->nList.end() ) {
			if ( *posNList == n ) {
				return *posNList;
			}
			else {
				++posNList;
			}
		}
		++posGList;
	}
	cout << "Can't DFGs::findAndAccessNodeInGList !! \n";	// show the notice if return dummy Node
	exit(0);	// Error, exit program !!
//	return *posNList;	// dummy, must avoid this return
}
Node& DFGs::findAndAccessNodeInGList( int gid, int nid ) {
	list<Graph>::iterator		posGList;
	list<Node>::iterator		posNList;
	posGList = gList.begin();
	while ( posGList != gList.end() ) {
		if ( posGList->get_gID() == gid ) {
			posNList = posGList->nList.begin();
			while ( posNList != posGList->nList.end() ) {
				if ( posNList->get_nID() == nid ) {
					return *posNList;
				}
				else {
					++posNList;
				}
			}
		}
		++posGList;
	}
	cout << "Can't DFGs::findAndAccessNodeInGList !! \n";	// show the notice if return dummy Node
	exit(0);	// Error, exit program !!
//	return *posNList;	// dummy, must avoid this return
}
bool DFGs::findNodeInGList( int gid, int nid ) { 
	list<Graph>::iterator		posGList;
	list<Node>::iterator		posNList;
	posGList = gList.begin();
	while ( posGList != gList.end() ) {
		if ( posGList->get_gID() == gid ) {
			posNList = posGList->nList.begin();
			while ( posNList != posGList->nList.end() ) {
				if ( posNList->get_nID() == nid ) {
					return true;
				}
				else {
					++posNList;
				}
			}
		}
		++posGList;
	}
	return false;
} 
bool DFGs::findAndUpdataEdgeInGList( Edge e ) {
	list<Graph>::iterator			iterGList;
	list<Edge>::iterator			iterEList;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		iterEList = iterGList->eList.begin();
		if ( iterGList->get_gID() == e.get_gID() ) {
			while ( iterEList != iterGList->eList.end() ) { 
				if ( iterEList->get_eID() == e.get_eID() ) { 
					*iterEList = e;
					return true;
				}
				++iterEList;
			}
		}
		++iterGList;
	}
	return false;
}
Edge& DFGs::findAndAccessEdgeInGList( Edge& e ) { 
	list<Graph>::iterator			iterGList;
	list<Edge>::iterator			iterEList;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		iterEList = iterGList->eList.begin();
		if ( iterGList->get_gID() == e.get_gID() ) {
			while ( iterEList != iterGList->eList.end() ) { 
				if ( iterEList->get_eID() == e.get_eID() ) { 
					return *iterEList;
				}
				++iterEList;
			}
		}
		++iterGList;
	}
	cout << "Can't DFGs::findAndAccessEdgeInGList !! \n"; 
	exit(0);	// Error, exit program !!
}
Edge& DFGs::findAndAccessEdgeInGList( int gid, int eid ) {
	list<Graph>::iterator			iterGList;
	list<Edge>::iterator			iterEList;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		iterEList = iterGList->eList.begin();
		if ( iterGList->get_gID() == gid ) {
			while ( iterEList != iterGList->eList.end() ) { 
				if ( iterEList->get_eID() == eid ) { 
					return *iterEList;
				}
				++iterEList;
			}
		}
		++iterGList;
	}
	cout << "Can't DFGs::findAndAccessEdgeInGList !! \n"; 
	exit(0);	// Error, exit program !!
}
Edge& DFGs::findAndAccessEdgeInGList( int gid, int from_nID, int to_nID ) { 
	list<Graph>::iterator			iterGList;
	list<Edge>::iterator			iterEList;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		if ( iterGList->get_gID() == gid ) {
			iterEList = iterGList->eList.begin();
			while ( iterEList != iterGList->eList.end() ) { 
				if ( iterEList->get_from_nID() == from_nID && 
					iterEList->get_to_nID() == to_nID ) { 
					return *iterEList;
				}
				++iterEList;
			}
		}
		++iterGList;
	}
	cout << "Can't DFGs::findAndAccessEdgeInGList !! \n"; 
	exit(0);	// Error, exit program !!
}
// access nList functions
// sysopt modified_20070405
void DFGs::constructNodeList ( int intArr[] ) {
	
	Node tempN ( 
		intArr[0],	// NITEM_GID
		intArr[1],	// NITEM_NID
		intArr[2],	// NITEM_CID
		intArr[3],	// NITEM_DATAIN
		intArr[4],	// NITEM_DATAOUT
		intArr[5],	// NITEM_BUSINCYCLES, 
		intArr[6],	// NITEM_TC_IN,       
		intArr[7],	// NITEM_BUSOUTCYCLES,
		intArr[8] 	// NITEM_TC_OUT,      
		);
	//cout<<"bus cycles = "<<intArr[5]<<"   "<<tempN.busCycles<<endl;
	
	//if ( !gList.back().find_nList( tempN ) ) {
		gList.back().addNode_nList( tempN );	// call by value, tempN is temp variable 
	//}						// (construct only one time, because of list.back() !) 
	//else { 
	//	cout << "Add Node error ! Node is already in the nList !\n";
	//}
}

void DFGs::setNodeProdecessor() {
	gList.back().setNodePredList();	// pass throught nList to set predecessor(only one time !)
}
void DFGs::setNumOfNodeInGraph() {
	list<Graph>::iterator	posGraph;
	posGraph = gList.begin();
	while ( posGraph != gList.end() ) {
		posGraph->set_nTotalNum();
		++posGraph;
	} 
} 

bool DFGs::DFGsNotEmpty()  // added by Josh
{
	list<Graph>::iterator	posGraph;
	posGraph = gList.begin();
	while ( posGraph != gList.end() )
	{
		if ( posGraph->get_nTotalNum() == 0 )
		{
			++posGraph;
		}
		else
		{
			return true; // DFGs is not empty
		}
	}
	return false;
}
// access eList functions
void DFGs::constructEdgeList ( int intArr[], string eNote ) {
	Edge tempE ( 
		intArr[0],	// EITEM_GID
		intArr[1],	// EITEM_EID
		intToEdgeType( intArr[2] ), // EITEM_ETYPE
		intArr[3],	// EITEM_FROMNID
		intArr[4]	// EITEM_TONID
		);
	if ( tempE.get_eType() == INP || tempE.get_eType() == OUT ) { 
		tempE.set_edgeNote( eNote ); 
	} 
	if ( !gList.back().find_eList( tempE ) ) {
		gList.back().addEdge_eList( tempE );	// call by value, tempE is temp variable 
	}											// (construct only one time, bocause of list.back() ) 
	else {
		cout << "Add Edge error ! Edge is already in the eList !\n";
	}
}
void DFGs::setEdgeLifeTime() {
	list<Graph>::iterator	iterGList;
	list<Node>::iterator	posNode;
	list<Edge>::iterator	posEdge;
	iterGList = gList.begin();	// initial iterator for gList
	while ( iterGList != gList.end() ) {
		// set Edge life time
		posEdge = iterGList->eList.begin(); // initial iterator for eList
		while ( posEdge != iterGList->eList.end() ) {
	 		posNode = iterGList->nList.begin(); // reset iterator for nList
		 	for ( int idxFrom=1; idxFrom<posEdge->from_nID; ++idxFrom) {
				++posNode;
			}
			posEdge->set_beginTStep( posNode->tStep);
			posNode = iterGList->nList.begin(); // reset iterator for nList
			for ( int idxTo=1; idxTo<posEdge->to_nID; ++idxTo) {
				++posNode;
			}
			posEdge->set_endTStep( posNode->tStep );
			++posEdge;
		}// End of set Edge life time
		++iterGList;
	}
}
// rsConstraint functions
void DFGs::init_rsConstraint() {
	int rsNumOfType = NumOfNodeType;
	for( int i=0; i < rsNumOfType; ++i)
		rsConstraint.push_back(-1);	// No resource constraints will store "-1 "
}
void DFGs::set_rsConstraint( NodeType nt, int num ) {
	rsConstraint[nt] = num;
}
bool DFGs::check_rsConstraint( NodeType nt, int num ) { 
	if ( rsConstraint[nt] == -1 ) {
		return true;
	}
	else {
		if ( num <= rsConstraint[nt] ) { return true; }
		else { return false; }
	}
}
bool DFGs::check_rsConstraint() {
	list<Graph>::iterator		iterGList;
	list<Node>::iterator		iterNList;
	vector<int>					tempRSUsage;
	int							rsNumOfType = NumOfNodeType;
	int							tempTimeStep;
	// initial the tempRSUsage
	for( int i=0; i < rsNumOfType; ++i) {
		tempRSUsage.push_back(0);
	}
	// In one by one time step, scan and check resource usage 
	tempTimeStep = 1;	// initialize time step as 1
	while ( tempTimeStep != tConstraint ) {
		// reset the tempRSUsage
		for( int i=0; i < rsNumOfType; ++i) {
			tempRSUsage[i] = 0;
		}
		// scan resource usage in this time step
		iterGList = gList.begin();
		while ( iterGList != gList.end() ) {
			iterNList = iterGList->nList.begin();
			while ( iterNList != iterGList->nList.end() ) {
				if ( iterNList->get_tStep() == tempTimeStep ) {
					++tempRSUsage[ iterNList->get_nType() ];
				}
				++iterNList;
			}
			++iterGList;
		}
		// check rs violation
		for ( int idx=0; idx<rsNumOfType; ++idx ) {
			if ( rsConstraint[idx] != -1 ) {
				if ( rsConstraint[idx] < tempRSUsage[idx] ) {
					cout << "rsConstraint violation !!\n";
					system("Pause"); 
					return false;	// false represents rs violation
				}
			}
		}
		++tempTimeStep;
	}
	return true;	// true represents no violation
}
void DFGs::show_rsConstraint() {
	cout << "Show_rsConstraint : " << endl;
	copy ( rsConstraint.begin(), rsConstraint.end(), 
		   ostream_iterator<int>(cout," ") );
	cout << endl;
}
// run ASAP/ALAP
//void DFGs::runASAP() {
//	list<Graph>::iterator	iterGList;
//	iterGList = gList.begin();
//	for( int i=0; i<numOfGraph; ++i) {
//		ASAP( *iterGList );
//		++iterGList;
//	}
//}
//void DFGs::runALAP() {
//	list<Graph>::iterator	iterGList;
//	iterGList = gList.begin();
//	for( int i=0; i<numOfGraph; ++i) {
//		ALAP( *iterGList, tConstraint );
//		++iterGList;
//	}
//}
// other functions
int DFGs::returnWeightBetweenNodes( Node& s, Node& d ) {
	list<Graph>::iterator		iterGList;
	list<Edge>::iterator		iterEList;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		if ( s.get_gID() == iterGList->get_gID() ) {
			iterEList = iterGList->eList.begin();
			while ( iterEList != iterGList->eList.end() ) {
				if ( (iterEList->get_from_nID() == s.get_nID()) && 
					(iterEList->get_to_nID() == d.get_nID()) ) {
					return iterEList->get_eWeight();
				}
				++iterEList;
			}
		}
		++iterGList;
	}
	cout << "Warning, can't find the DFGs::returnWeightBetweenNodes !!" << endl;
	system("Pause");	// for pause system
	return -1;
}
void DFGs::updataEdgeLiftTime() {
	list<Graph>::iterator		iterGList;
	list<Edge>::iterator		iterEList; 
	Node						tempNode;
	// select one edge and search its fromNode and toNode time step
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		iterEList = iterGList->eList.begin();
		while ( iterEList != iterGList->eList.end() ) {
			// fromNode
			tempNode = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_from_nID() );
			iterEList->set_beginTStep( tempNode.get_tStep() );
			// toNode
			tempNode = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_to_nID() );
			iterEList->set_endTStep( tempNode.get_tStep() );
			// totLifeT
			iterEList->set_totLifeT();
			++iterEList;
		}
		++iterGList;
	}
}
void DFGs::updataGraphEndTS() {
	list<Graph>::iterator		iterGList;
	list<Node>::iterator		iterNList;
	int							maxTS;
	// search each graph maxTS and updata each graph maxTS
	maxTS = 0;
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) {
		iterNList = iterGList->nList.begin();
		while ( iterNList != iterGList->nList.end() ) {
			if ( maxTS < iterNList->get_tStep() ) {
				maxTS = iterNList->get_tStep();
			}
			++iterNList;
		}
		iterGList->set_endTStep( maxTS );
		++iterGList;
	}
}
//void DFGs::setIsCutAndCalEdgeCutNum() {
//	list<Graph>::iterator		iterGList;
//	list<Edge>::iterator		iterEList;
//	Node						tempNodeFrom;
//	Node						tempNodeTo;
//	int							tempEdgeCutNum;
//	// select one edge and search its fromNode and toNode time step 
//	tempEdgeCutNum = 0;
//	iterGList = gList.begin();
//	while ( iterGList != gList.end() ) {
//		iterEList = iterGList->eList.begin();
//		while ( iterEList != iterGList->eList.end() ) {
//			// fromNode
//			tempNodeFrom = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_from_nID() );
//			// toNode
//			tempNodeTo = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_to_nID() );
//			if ( (tempNodeFrom.get_nType() != READ) && (tempNodeFrom.get_nType() != DOUT) &&
//				(tempNodeTo.get_nType() != READ) && (tempNodeTo.get_nType() != DOUT) ) {
//				if ( tempNodeFrom.get_partiID() != tempNodeTo.get_partiID() ) {
//					iterEList->set_isCut( true );
//					++tempEdgeCutNum;
//				} 
//				else {		// -- 2005.11.21 修正成可以變回 isCut = false -- 
//					iterEList->set_isCut( false );
//				} 
//			}
//			++iterEList;
//		}
//		++iterGList;
//	}
//	edgeCutNum = tempEdgeCutNum;
//}
//void DFGs::setIsCutAndCalEdgeCutNumForKWay() {	// -- 2005.12.01 修正成可以用在 kWayParti -- 
//	list<Graph>::iterator		iterGList;
//	list<Edge>::iterator		iterEList;
//	Node						tempNodeFrom;
//	Node						tempNodeTo;
//	int							tempEdgeCutNum;
//	bool						findFromNode; 
//	bool						findToNode; 
//	// select one edge and search its fromNode and toNode time step 
//	tempEdgeCutNum = 0;
//	iterGList = gList.begin();
//	while ( iterGList != gList.end() ) {
//		iterEList = iterGList->eList.begin();
//		while ( iterEList != iterGList->eList.end() ) {
//			findFromNode = false; 
//			findToNode = false; 
//			// fromNode
//			findFromNode = findNodeInGList( iterEList->get_gID(), iterEList->get_from_nID() ); 
//			if ( findFromNode ) { 
//				tempNodeFrom = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_from_nID() ); 
//			} 
//			// toNode
//			findToNode = findNodeInGList( iterEList->get_gID(), iterEList->get_to_nID() );
//			if ( findToNode ) { 
//				tempNodeTo = findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_to_nID() );
//			} 
//			if ( findFromNode && findToNode ) { 
//				if ( (tempNodeFrom.get_nType() != READ) && (tempNodeFrom.get_nType() != DOUT) &&
//					(tempNodeTo.get_nType() != READ) && (tempNodeTo.get_nType() != DOUT) ) {
//					if ( tempNodeFrom.get_partiID() != tempNodeTo.get_partiID() ) {
//						iterEList->set_isCut( true );
//						++tempEdgeCutNum;
//					} 
//					else {		// -- 2005.11.21 修正成可以變回 isCut = false -- 
//						iterEList->set_isCut( false );
//					} 
//				}
//			}
//			else { 
//				// if not find, the edge must already be cutEdge, so do nothing 
//			} 
//			++iterEList;
//		}
//		++iterGList;
//	}
//	edgeCutNum = tempEdgeCutNum;
//}
int DFGs::getClusterSize( int cID ) { 
	int							tempSize; 
	list<Graph>::iterator		iterGList;
	list<Node>::iterator		iterNList;
	list<int>					assIDList;
	// 
	tempSize = 0; 
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) { 
		iterNList = iterGList->nList.begin();
		while ( iterNList != iterGList->nList.end() ) { 
			if ( iterNList->get_clusID() == cID ) { 
				if ( find(assIDList.begin(), assIDList.end(), iterNList->get_assignID() ) 
					== assIDList.end() ) { 
					assIDList.push_back( iterNList->get_assignID() ); 
					tempSize = tempSize + iterNList->get_nSize(); 
				}
			}
			++iterNList;  
		}
		++iterGList;
	}
	return tempSize; 
}
int DFGs::getNumOfNodeInCluster( int cID ) { 
	int							tempSize; 
	list<Graph>::iterator		iterGList;
	list<Node>::iterator		iterNList;
	// 
	tempSize = 0; 
	iterGList = gList.begin();
	while ( iterGList != gList.end() ) { 
		iterNList = iterGList->nList.begin();
		while ( iterNList != iterGList->nList.end() ) { 
			if ( iterNList->get_clusID() == cID ) { 
				++tempSize; 
			}
			++iterNList; 
		}
		++iterGList;
	}
	return tempSize; 
}
//bool DFGs::checkPartitionSizeForContinueFMPartiOrNot() { 
//	list<Graph>::iterator			iterGList; 
//	list<Node>::iterator			iterNList; 
//	vector<int>						partiIDVect; 
//	vector<int>::iterator			iterPartiIDVect; 
//	vector<int>						sizeVect; 
//	list<int>						assignIDList; 
//	int								tempPartiID; 
//	int								partiIDSize; 
//	// -- start --	(return true: no violation and continue, false: violation and stop) 
//	tempPartiID = 0; 
//	partiIDSize = 0; 
//	iterGList = gList.begin(); 
//	while ( iterGList != gList.end() ) { 
//		iterNList = iterGList->nList.begin(); 
//		while ( iterNList != iterGList->nList.end() ) { 
//			if ( iterNList->get_nType() != READ && iterNList->get_nType() != DOUT ) { 
//				tempPartiID = iterNList->get_partiID(); 
//				iterPartiIDVect = find( partiIDVect.begin(), partiIDVect.end(), tempPartiID ); 
//				if ( iterPartiIDVect == partiIDVect.end() ) { 
//					partiIDVect.push_back( tempPartiID ); 
//					sizeVect.push_back( 0 ); 
//				} 
//			}
//			++iterNList; 
//		} 
//		++iterGList; 
//	} 
////copy(partiIDVect.begin(),partiIDVect.end(),ostream_iterator<int>(cout," ")); cout << endl; 
//	iterPartiIDVect = partiIDVect.begin(); 
//	while ( iterPartiIDVect != partiIDVect.end() ) { 
//		iterGList = gList.begin(); 
//		while ( iterGList != gList.end() ) { 
//			iterNList = iterGList->nList.begin(); 
//			while ( iterNList != iterGList->nList.end() ) { 
//				if ( iterNList->get_partiID() == *iterPartiIDVect ) { 
//					if ( find(assignIDList.begin(), assignIDList.end(), iterNList->get_assignID()) 
//						== assignIDList.end() ) { 
//						assignIDList.push_back( iterNList->get_assignID() ); 
//						sizeVect[*iterPartiIDVect] = sizeVect[*iterPartiIDVect] + NodeSize[iterNList->get_nType()]; 
//					} 
//					else { 
//						// don't add the node size 
//					} 
//				} 
//				++iterNList; 
//			} 
//			++iterGList; 
//		}
////copy(sizeVect.begin(),sizeVect.end(),ostream_iterator<int>(cout," "));  cout << endl; 
//		++iterPartiIDVect; 
//		assignIDList.clear(); 
//		partiIDSize = sizeVect[*iterPartiIDVect];	// final partiID size of nodes
//		if ( (partiIDSize/2) < clusSizeConstraint ) { 
//			return false; 
//		} 
//	} 
//	return true; 
//} 
// End of Class DFGs


// *****************************************************************
// 因為 ASAP/ALAP 會去偵測 schedule list 還有沒有 node 未排程, 
// 所以必需要用各別 graph 當輸入才不會造成還沒 all nodes 排完就跳出
// *****************************************************************
// ****** comparison and return maxima ******
template <class T1>
T1 maxPositiveNumber( T1 a, T1 b ) {
	if ( a >= b ) {
		return a;
	}
	else {
		return b;
	}
}// ****** End of comparison and return maxima ******


// ****** comparison and return minima ******
template <class T2>
T2 minPositiveNumber( T2 a, T2 b ) {
	if ( a >= 0 && b >= 0 ) {
		if ( a <= b ) {
			return a;
		}
		else {
			return b;
		}
	}
	else {
		if ( a < 0 && b >= 0 ) { 
			return b; 
		}
		else if ( b < 0 && a >= 0 ) {
			return a; 
		}
		else { 
			cout << "No positive numbers !!" << endl; 
			return -1; 
		}
	}
}// ****** End of comparison and return minima ******

/*
// ****** ASAP ******
void ASAP( Graph& g ) {
	int						t_j;	// time step of successor ( t_j = t_i + t_iDelay )
	int						t_iDelay;	// delay time of predecessor
	int						t_i;	// time step of predecessor
	int						t_jPreASAP;	// time step of node by ASAP ( t_j' )
	int						t_jASAP;	// time step of node by ASAP ( t_j" = max{t_j, t_j'} )
	int						tc_ASAP;	// timing constraint of ASAP for the ending result
	list<int>				scheduleList;	// schedule list stores scheduled candidate nodes 
	list<int>				succTempList;	// successor list temporary stores success nodes
	list<Node>::iterator	posNode;	// iterator for nList
	int						idxScheduleList;	// index of the scheduleList
	list<int>::iterator		posPred;	// iterator for nList's predecessor
	list<Node>::iterator	posNodePred;	// iterator for nList's predecessor node
	list<Edge>::iterator	posEdge;	// iterator for eList
	
	// initial schedule_list
	tc_ASAP = 0;	// initialize the max tASAP as 0
	t_jASAP = 1;	// initial time step for READ
	// Initial schedule_list by READ nodes
	posNode = g.nList.begin();	// initial the nList index for searching READ node
	while ( posNode != g.nList.end() ) {
		if ( posNode->get_nType() == READ ) {
			posNode->set_tASAP( t_jASAP );
			posNode->set_tStep( t_jASAP );	// initial node schedule time step by ASAP
			succTempList = posNode->successor;
			scheduleList.insert( scheduleList.end(), succTempList.begin(), succTempList.end() );
			++posNode;
		}
		else {
			++posNode;
		}
	}// End of initial schedule_list
	
	// Computing ASAP
	while ( !scheduleList.empty() ) {	// check the scheduleList empty or not, empty -> finish ASAP
		idxScheduleList = scheduleList.front();	// choose one node for scheduling
		scheduleList.pop_front();	// pop out the front node of scheduleList
		// set the iterator (posNode) of current schedule node
		posNode = g.nList.begin();	// initial nList iterator
		while ( posNode->get_nID() != idxScheduleList ) {	
			++posNode;
		}
		// search the max tASAP for current schedule node
		posPred = posNode->predecessor.begin();	// initial iterator of posNode's predecessor
		while ( posPred != posNode->predecessor.end() ) {	
			// set the iterator (posNodePred) of current schedule node
			posNodePred = g.nList.begin();	// initial nList iterator
			while ( posNodePred->get_nID() != *posPred ) {
				++posNodePred;
			}
			// search the max tASAP for the schedule node and whole graph
			t_i = posNodePred->get_tASAP();
			t_iDelay = posNodePred->get_dTime();
			t_j = t_i + t_iDelay;
			t_jPreASAP = posNode->get_tASAP();
			t_jASAP = maxPositiveNumber( t_j, t_jPreASAP );
			tc_ASAP = maxPositiveNumber( tc_ASAP, t_jASAP );
			posNode->set_tASAP( t_jASAP );
			posNode->set_tStep( t_jASAP );	// initial node schedule time step by ASAP
			// increase index
			++posPred;
		}
		// put the successors of current schedule node into scheduleList
		succTempList = posNode->successor;
		scheduleList.insert( scheduleList.end(), succTempList.begin(), succTempList.end() );
	}// End of computing ASAP

	// Set initial Edge life time
	posEdge = g.eList.begin();
	while ( posEdge != g.eList.end() ) {
		posNode = g.nList.begin();
		for ( int idxFrom=1; idxFrom<posEdge->get_from_nID(); ++idxFrom) {
			++posNode;
		}
		posEdge->set_beginTStep( posNode->get_tStep() );
		posNode = g.nList.begin();
		for ( int idxTo=1; idxTo<posEdge->get_to_nID(); ++idxTo) {
			++posNode;
		}
		posEdge->set_endTStep( posNode->get_tStep() );
		++posEdge;
	}// End of set Edge life time

	// Set Graph begin and end time step
	g.set_beginTStep( 1 ) ;	// the begin schedule time step of g
	g.set_endTStep( tc_ASAP );	// the end schedule time step of g
}// ****** End of ASAP ******


// ****** ALAP ******
void ALAP( Graph& g, int tc ) {
	int						t_j;	// time step of successor 
	int						t_iDelay;	// delay time of predecessor
	int						t_i;	// time step of predecessor ( t_i = t_j - t_iDelay )
	int						t_iPreALAP;	// time step of node by ALAP ( t_i' )
	int						t_iALAP;	// time step of node by ASAP ( t_i" = min{t_i, t_i'} )
	int						tc_ALAP;	// timing constraint of ALAP (User-defined timing constraints)
	list<int>				scheduleList;	// schedule list stores scheduled candidate nodes 
	list<int>				predTempList;	// predecessor list temporary stores success nodes
	list<Node>::iterator	posNode;	// iterator for nList
	int						idxScheduleList;	// index of the scheduleList
	list<int>::iterator		posSucc;	// iterator for nList's successor
	list<Node>::iterator	posNodeSucc;	// iterator for nList's successor node
	bool					reCalculate;	// re-calculate the succ node ALAP if this succ node has not been ALAP

	// initial schedule_list
	tc_ALAP = tc;
	t_j = tc;		// because DOUT is a node, so t_j start at tc
	// Initial schedule_list by DOUT nodes
	posNode = g.nList.begin();	// initial the nList index for searching DOUT node
	while ( posNode != g.nList.end() ) {
		if ( posNode->get_nType() == DOUT ) {
			posNode->set_tALAP( tc );
			posNode->set_mobility();
			predTempList = posNode->predecessor;
			scheduleList.insert( scheduleList.end(), predTempList.begin(), predTempList.end() );
			++posNode;
		}
		else {
			++posNode; 
		}
	}// End of initial schedule_list

	// computing ALAP
	reCalculate = false; 
	while ( !scheduleList.empty() ) {	// check the scheduleList empty or not, empty -> finish ALAP
		reCalculate = false;
		idxScheduleList = scheduleList.front();	// choose one node for scheduling
//copy(scheduleList.begin(),scheduleList.end(),ostream_iterator<int>(cout," "));
//cout << endl;
		scheduleList.pop_front();	// pop out the front node of scheduleList
		// set the iterator (posNode) of current schedule node
		posNode = g.nList.begin();	// initial nList iterator
		while ( posNode->get_nID() != idxScheduleList ) {	
			++posNode;
		}
		// search the min tALAP for current schedule node
//cout << *posNode;  
		posSucc = posNode->successor.begin();	// initial iterator of posNode's successor
		while ( posSucc != posNode->successor.end() ) {	
			// set the iterator (posNodeSucc) of current schedule node
			posNodeSucc = g.nList.begin();	// initial nList iterator
			while ( posNodeSucc->get_nID() != *posSucc ) {
				++posNodeSucc;
			}
			if ( posNodeSucc->get_tALAP() == -1 ) {	// the succNode has not been scheduled by ALAP, so can not be a rederence
				scheduleList.push_front( idxScheduleList );
				scheduleList.push_front( *posSucc );
				posSucc = posNode->successor.end();
				reCalculate = true; 
			}
			else {
				// search the max tALAP for the schedule node and whole graph
				t_j = posNodeSucc->get_tALAP();
				t_iDelay = posNode->get_dTime();	// t_i delay time
				t_i = t_j - t_iDelay;
				t_iPreALAP = posNode->get_tALAP();
				if ( t_iPreALAP != -1 ) {
					t_iALAP = minPositiveNumber( t_i, t_iPreALAP );
				}
				else {
					t_iALAP = t_i;
				}
				// set tALAP
				posNode->set_tALAP( t_iALAP );
				posNode->set_mobility();
				// increase index
				++posSucc;
			}
		}
//cout << *posNode; 
		if ( reCalculate == false ) {
			// put the successors of current schedule node into scheduleList
			predTempList = posNode->predecessor;
			scheduleList.insert( scheduleList.end(), predTempList.begin(), predTempList.end() );
		}
	}// End of computing ALAP

	// check that time step can't be zero or negative
	posNode = g.nList.begin();
	while ( posNode != g.nList.end() ) {
		if ( posNode->get_tALAP() <= 0 ) {
			cout << "Warning, the tALAP is zero or negative number !! -- ";
			cout << "(gID, nID)= (" << posNode->get_gID() << ", " << posNode->get_nID() << ") , tALAP= " 
				 << posNode->get_tALAP() << endl;
			system("Pause");
			exit(0);	// exit program
		}
		else if ( posNode->get_mobility() < 0 ) {
			cout << "Warning, the mobility is negative number after tALAP !!\n";
			cout << "(gID, nID)= (" << posNode->get_gID() << ", " << posNode->get_nID() << ") , mobility= " 
				 << posNode->get_mobility() << endl;
			system("Pause");
			exit(0);	// exit program
		}
		++posNode;
	}
}// ****** End of ALAP ******
*/

