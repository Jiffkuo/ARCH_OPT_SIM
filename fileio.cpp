// ********************************************************************
// fileio.cpp 
//     1. Read files to setup the DFGs. (Read)
//     2. Generate the output file for DFGShow format. (Write)
//     3. Generate the output file to record the DFGs cout. (Write) 
// ********************************************************************
// System Optimization used key word: sysopt
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************
#include "fileio.h"


// *** Class DFRSid *** 
void DFRSid::preset_dfrsName() { 
	char*				buffer = new char [10]; 
	_itoa( dfrsID, buffer, 10 ); 
	dfrsName = DFRSName[dfrsType] + buffer; 
	delete buffer; 
} 
// operator functions 
void DFRSid::operator=( const DFRSid& aDFRSid ) { 
	dfrsType = aDFRSid.dfrsType; 
	dfrsID = aDFRSid.dfrsID; 
	dfrsName = aDFRSid.dfrsName; 
} 
bool DFRSid::operator==( DFRSid& aDFRSid ) { 
	if ( dfrsType == aDFRSid.get_dfrsType() && 
		dfrsID == aDFRSid.get_dfrsID() && 
		dfrsName == aDFRSid.get_dfrsName() ) { 
		return true; 
	} 
	else { 
		return false; 
	} 
} 
bool DFRSid::operator!=( DFRSid& aDFRSid ) { 
	if ( dfrsType == aDFRSid.get_dfrsType() && 
		dfrsID == aDFRSid.get_dfrsID() && 
		dfrsName == aDFRSid.get_dfrsName() ) { 
		return false; 
	} 
	else { 
		return true; 
	} 
} 
// *** End of Class DFRSid *** 

// *** Class CtrlSignal *** 
// operator functions 
void CtrlSignal::operator=( CtrlSignal& aCS ) { 
	ctrlID = aCS.get_ctrlID(); 
	ctrlType = aCS.get_ctrlType(); 
	ctrlName = aCS.get_ctrlName(); 
	accessNum = aCS.get_accessNum(); 
	muxConfType = aCS.get_muxConfType(); 
	eIDList.clear();
	eIDList.insert( eIDList.end(), aCS.eIDList.begin(), aCS.eIDList.end() ); 
	cstepList.clear(); 
	cstepList.insert( cstepList.end(), aCS.cstepList.begin(), aCS.cstepList.end() );
	confList.clear(); 
	confList.insert( confList.end(), aCS.confList.begin(), aCS.confList.end() ); 
} 
// other functions 
void CtrlSignal::clear_CtrlSignal() { 
	ctrlID = -1; 
	ctrlType = DFRS_NONE; 
	ctrlName = "none";  
	accessNum = -1; 
	muxConfType = MUX_NONE; 
	cstepList.clear(); 
	confList.clear(); 
	eIDList.clear();
} 
// *** End of Class CtrlSignal *** 

// *** Class InterLine *** 
// operator functions 
void InterLine::operator=( InterLine& aIL ){ 
	ilID = aIL.get_ilID(); 
	ilName = aIL.get_ilName(); 
	bitNum = aIL.get_bitNum(); 
	ilNote = aIL.get_ilNote(); 
	ilType = aIL.get_ilType(); 
	eIDList.clear(); 
	eIDList.insert( eIDList.end(), aIL.eIDList.begin(), aIL.eIDList.end() ); 
} 
bool InterLine::searchEidOfInterLine( int eid ) { 
	if ( find( eIDList.begin(), eIDList.end(), eid ) == eIDList.end() ) { 
		return false; 
	} 
	else { 
		return true; 
	} 
} 
// other functions 
void InterLine::clear_InterLine() { 
	ilID = -1; 
	ilName = "NONE"; 
	bitNum = -1; 
	ilNote = "NONE"; 
	ilType = IL_NONE; 
	eIDList.clear(); 
} 
// *** End of Class InterLine *** 

// *** Class DataFlowRS *** 
// operator functions 
void DataFlowRS::operator=( DataFlowRS& aDFRS ) { 
	theDFRSid = aDFRS.get_theDFRSid(); 
	theFUType = aDFRS.get_theFUType(); 
	theFUSubType = aDFRS.get_theFUSubType(); 
	muxConfType = aDFRS.get_muxConfType(); 
	bitNum = aDFRS.get_bitNum();	
	inputNum = aDFRS.get_inputNum(); 
	outputNum = aDFRS.get_outputNum(); 
	neIDList.clear();
	neIDList.insert( neIDList.end(), aDFRS.neIDList.begin(), aDFRS.neIDList.end() ); 
	aCtrlSignal = aDFRS.get_aCtrlSignal(); 
	inputILList.clear(); 
	inputILList.insert( inputILList.end(), aDFRS.inputILList.begin(), aDFRS.inputILList.end() ); 
	outputInterLine = aDFRS.get_outputInterLine(); 
} 
// other functions 
void DataFlowRS::clear_DataFlowRS() {
	DFRSid				newDFRSid;
	CtrlSignal			newCtrlSignal; 
	InterLine			newInterLine; 
	theDFRSid = newDFRSid; 
	theFUType = NONE;
	theFUSubType = FU_NONE;
	muxConfType = MUX_NONE;
	bitNum = -1;
	inputNum = -1;
	outputNum = -1;	
	neIDList.clear(); 
	aCtrlSignal = newCtrlSignal;
	inputILList.clear(); 
	outputInterLine = newInterLine;
} 
bool DataFlowRS::searchNEIDofNEIDList( int id ) {	// only for REG & MUX, FU need to use the other one 
	list<int>::iterator			iterIDList; 
	iterIDList = neIDList.begin(); 
	while ( iterIDList != neIDList.end() ) { 
		if ( *iterIDList == id ) { 
			return true; 
		} 
		++iterIDList; 
	} 
	return false; 
} 
bool DataFlowRS::searchNEIDofEIDListofOutputInterLine( int id ) {	// only for FU, REG & MUX need to use the other one 
	list<int>::iterator			iterIDList; 
	iterIDList = outputInterLine.eIDList.begin(); 
	while ( iterIDList != outputInterLine.eIDList.end() ) { 
		if ( *iterIDList == id ) { 
			return true; 
		} 
		++iterIDList; 
	} 
	return false; 
} 
// *** End of Class DataFlowRS *** 

// *** Class RTLStruc *** 
// other functions 
DataFlowRS& RTLStruc::findAndAccessDFRS( DFRSid& aDFRSid ) { 
	list<DataFlowRS>::iterator			iterDataFlowRS; 
	list<DataFlowRS>::iterator			iterTargetDFRS; 
	if ( aDFRSid.get_dfrsType() == DFRS_FU ) { 
		iterDataFlowRS = aFUList.begin(); 
		while ( iterDataFlowRS != aFUList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				iterTargetDFRS = iterDataFlowRS; 
				iterDataFlowRS = aFUList.end(); 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
	} 
	else if ( aDFRSid.get_dfrsType() == DFRS_MUX ) { 
		iterDataFlowRS = aMuxList.begin(); 
		while ( iterDataFlowRS != aMuxList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				iterTargetDFRS = iterDataFlowRS; 
				iterDataFlowRS = aMuxList.end(); 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
	} 
	else if ( aDFRSid.get_dfrsType() == DFRS_REG ) { 
		iterDataFlowRS = aRegList.begin(); 
		while ( iterDataFlowRS != aRegList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				iterTargetDFRS = iterDataFlowRS; 
				iterDataFlowRS = aRegList.end(); 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
	} 
	else { 
		cout << "Warning ! Can't find RTLStruc::findAndAccessDFRS DFRSType ! \n"; 
	} 
	return *iterTargetDFRS; 
} 
bool RTLStruc::findAndUpdataDFRS( DataFlowRS& aDFRS ) { 
	list<DataFlowRS>::iterator			iterDataFlowRS; 
	DFRSid								tempDFRSid; 
	DFRSType							tempDFRSType; 
	tempDFRSid = aDFRS.get_theDFRSid(); 
	tempDFRSType = tempDFRSid.get_dfrsType(); 
	if ( tempDFRSType == DFRS_FU ) { 
		iterDataFlowRS = aFUList.begin(); 
		while ( iterDataFlowRS != aFUList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRS.get_theDFRSid() ) { 
				*iterDataFlowRS = aDFRS; 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	} 
	else if ( tempDFRSType == DFRS_MUX ) { 
		iterDataFlowRS = aMuxList.begin(); 
		while ( iterDataFlowRS != aMuxList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRS.get_theDFRSid() ) { 
				*iterDataFlowRS = aDFRS; 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	} 
	else if ( tempDFRSType == DFRS_REG ) { 
		iterDataFlowRS = aRegList.begin(); 
		while ( iterDataFlowRS != aRegList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRS.get_theDFRSid() ) { 
				*iterDataFlowRS = aDFRS; 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	} 
	else { 
		cout << "Warning ! Can't find RTLStruc::findAndUpdataDFRS DFRSType ! \n"; 
		return false; 
	} 
} 
bool RTLStruc::searchDFRS( DFRSid& aDFRSid ) { 
	list<DataFlowRS>::iterator			iterDataFlowRS;
	DFRSType							tempDFRSType; 
	tempDFRSType = aDFRSid.get_dfrsType(); 
	if ( tempDFRSType == DFRS_FU ) { 
		iterDataFlowRS = aFUList.begin(); 
		while ( iterDataFlowRS != aFUList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	} 
	else if ( tempDFRSType == DFRS_MUX ) { 
		iterDataFlowRS = aMuxList.begin(); 
		while ( iterDataFlowRS != aMuxList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	} 
	else if ( tempDFRSType == DFRS_REG ) { 
		iterDataFlowRS = aRegList.begin(); 
		while ( iterDataFlowRS != aRegList.end() ) { 
			if ( iterDataFlowRS->get_theDFRSid() == aDFRSid ) { 
				return true; 
			} 
			else { 
				++iterDataFlowRS; 
			}
		} 
		return false; 
	}  
	else { 
		return false; 
	} 
} 
InterLine& RTLStruc::findAndAccessIL( ILType ailtp, int eid ) { 
	list<InterLine>::iterator			iterILList; 
	list<InterLine>::iterator			iterTargetILList; 
	iterILList = aInterLineList.begin(); 
	while ( iterILList != aInterLineList.end() ) { 
		if ( iterILList->get_ilType() == ailtp ) { 
			if ( iterILList->searchEidOfInterLine( eid ) ) { 
				iterTargetILList = iterILList; 
				iterILList = aInterLineList.end(); 
			} 
			else { 
				++iterILList;
			} 
		} 
		else { 
			++iterILList; 
		} 
	} 
	return *iterTargetILList;
} 
InterLine& RTLStruc::findAndAccessIL( string& aILName ) { 
	list<InterLine>::iterator			iterILList; 
	list<InterLine>::iterator			iterTargetILList; 
	iterILList = aInterLineList.begin(); 
	while ( iterILList != aInterLineList.end() ) { 
		if ( iterILList->get_ilName() == aILName ) { 
			iterTargetILList = iterILList; 
			iterILList = aInterLineList.end(); 
		} 
		else { 
			++iterILList; 
		} 
	} 
	return *iterTargetILList;
} 
bool RTLStruc::findAndUpdataIL( InterLine& aIL ) { 
	list<InterLine>::iterator			iterILList;
	iterILList = aInterLineList.begin(); 
	while ( iterILList != aInterLineList.end() ) { 
		if ( iterILList->get_ilName() == aIL.get_ilName() ) { 
			*iterILList = aIL; 
			return true; 
		} 
		++iterILList; 
	}
	return false; 
} 
void RTLStruc::detailConnectingFuRegMux() { 
	list<DataFlowRS>::iterator		iterFUList; 
	list<DataFlowRS>::iterator		iterMuxList; 
	list<DataFlowRS>::iterator		iterRegList; 
	list<InterLine>::iterator		iterInterLineList; 
	CtrlSignal						tempCtrlSignalList; 
	InterLine						tempMuxInterLine; 
	list<int>::iterator				iterEIDList; 
	list<InterLine>					tempILList; 
	// deal with the aMuxList 
	iterMuxList = aMuxList.begin(); 
	while ( iterMuxList != aMuxList.end() ) { 
		tempMuxInterLine = iterMuxList->get_outputInterLine(); 
		if ( tempMuxInterLine.get_ilType() == IL_MUXOUT_TO_FU ) {	// the previous inputInterLine is a regout_to_mux 
			iterEIDList = tempMuxInterLine.eIDList.begin(); 
			while ( iterEIDList != tempMuxInterLine.eIDList.end() ) { 
				iterRegList = aRegList.begin(); 
				while ( iterRegList != aRegList.end() ) { 
					if ( iterRegList->searchNEIDofNEIDList( *iterEIDList ) ) { 
						tempILList.push_back( iterRegList->get_outputInterLine() ); 
					} 
					++iterRegList; 
				} 
				++iterEIDList; 
			} 
//copy(tempILList.begin(),tempILList.end(),ostream_iterator<InterLine>(cout,""));	cout << "**\n"; 
		} 
		else if ( tempMuxInterLine.get_ilType() == IL_MUXOUT_TO_REG ) {	// the previous inputInterLine is a muxout_to_fu 
			iterEIDList = tempMuxInterLine.eIDList.begin(); 
			while ( iterEIDList != tempMuxInterLine.eIDList.end() ) { 
				iterFUList = aFUList.begin(); 
				while ( iterFUList != aFUList.end() ) { 
					if ( iterFUList->searchNEIDofEIDListofOutputInterLine( *iterEIDList ) ) { 
						tempILList.push_back( iterFUList->get_outputInterLine() ); 
					} 
					++iterFUList; 
				} 
				++iterEIDList; 
			} 
//copy(tempILList.begin(),tempILList.end(),ostream_iterator<InterLine>(cout,""));	cout << "**\n"; 
		} 
		else { 
			// other type won't be deal with 
		} 
		iterMuxList->copyin_inputILList( tempILList ); 
		iterMuxList->set_inputNum( iterMuxList->inputILList.size() ); 
		tempCtrlSignalList = iterMuxList->get_aCtrlSignal(); 
		tempCtrlSignalList.copyin_eIDList( iterMuxList->neIDList ); 
		tempCtrlSignalList.set_accessNum( iterMuxList->inputILList.size() ); 
		iterMuxList->set_aCtrlSignal( tempCtrlSignalList ); 
		findAndUpdataCS( tempCtrlSignalList );	// updata CS of aRTLStruc ! 
		tempCtrlSignalList.clear_CtrlSignal(); 
		tempILList.clear(); 
		++iterMuxList; 
	} 
	// deal with the aRegList (except the INP reg)
	iterRegList = aRegList.begin(); 
	while ( iterRegList != aRegList.end() ) { 
		if ( iterRegList->inputILList.empty() ) {	// inputILList is empty 
			iterMuxList = aMuxList.begin(); 
			while ( iterMuxList != aMuxList.end() ) { 
				tempMuxInterLine = iterMuxList->get_outputInterLine(); 
				if ( tempMuxInterLine.get_ilType() == IL_MUXOUT_TO_REG ) { 
					if ( checkTheSameIDList( iterRegList->neIDList, tempMuxInterLine.eIDList ) ) { 
						iterRegList->pushBack_inputILList( tempMuxInterLine ); 
						tempCtrlSignalList = iterRegList->get_aCtrlSignal(); 
						tempCtrlSignalList.copyin_eIDList( iterRegList->neIDList ); 
						tempCtrlSignalList.set_accessNum( tempMuxInterLine.eIDList.size() ); 
						iterRegList->set_aCtrlSignal( tempCtrlSignalList ); 
						findAndUpdataCS( tempCtrlSignalList );	// updata CS of aRTLStruc ! 
						tempCtrlSignalList.clear_CtrlSignal(); 
					} 
				} 
				++iterMuxList; 
			} 
			++iterRegList; 
		} 
		else {	// inputILList is not empty (INP reg) 
			tempCtrlSignalList = iterRegList->get_aCtrlSignal(); 
			tempCtrlSignalList.copyin_eIDList( iterRegList->neIDList ); 
			iterRegList->set_aCtrlSignal( tempCtrlSignalList ); 
			findAndUpdataCS( tempCtrlSignalList );	// updata CS of aRTLStruc ! 
			tempCtrlSignalList.clear_CtrlSignal(); 
			++iterRegList; 
		} 
	} 
} 
void RTLStruc::deleteRedundantMux() { 
	list<DataFlowRS>::iterator		iterFuList; 
	list<DataFlowRS>::iterator		iterMuxList; 
	list<DataFlowRS>::iterator		iterRegList; 
	list<InterLine>::iterator		iterInterLineList; 
	list<CtrlSignal>::iterator		iterCtrlSignalList; 
	list<DataFlowRS>::iterator		iterDFRSList_target; 
	InterLine						tempIL; 
	CtrlSignal						tempCS; 
	list<DataFlowRS>				tempMuxList; 
	list<DataFlowRS>				tempRegList; 
	list<InterLine>					tempInterLineList; 
	list<CtrlSignal>				tempCtrlSignalList; 
	list<InterLine>::iterator		iterInputIL_fu; 
	list<InterLine>::iterator		iterInputIL_mux; 
	list<InterLine>::iterator		iterInputIL_reg; 
	bool							noRedundantMux; 
	bool							findInRegList; 
	int								front_or_back;	// front: 0, the first input of FU; back: 1, the second input of FU
	// delete the MUX that has only one input, and updata whole aRTLStruc (1),(2),(3),(4) 
	noRedundantMux = false; 
	findInRegList = false; 
	front_or_back = -1;
	while ( !noRedundantMux ) { 
		noRedundantMux = true; 
		iterMuxList = aMuxList.begin(); 
		while ( iterMuxList != aMuxList.end() ) { 
//cout << *iterMuxList; 
			if ( iterMuxList->neIDList.size() == 1 ) { 
				// (1). modify the MUX's input to the REG's or FU's input 
				iterInputIL_mux = iterMuxList->inputILList.begin(); 
				tempIL = iterMuxList->get_outputInterLine(); 
				// REG 
				iterRegList = aRegList.begin(); 
				while ( iterRegList != aRegList.end() ) { 
//cout << *iterRegList; 
					iterInputIL_reg = iterRegList->inputILList.begin(); 
					if ( iterInputIL_reg->get_ilName() == tempIL.get_ilName() ) { 
						iterDFRSList_target = iterRegList; 
						iterRegList = aRegList.end(); 
						findInRegList = true;
					} 
					else { 
						++iterRegList; 
					} 
				} 
				if ( findInRegList == true ) { 
					iterDFRSList_target->popBack_inputILList(); 
					iterDFRSList_target->pushBack_inputILList( *iterInputIL_mux ); 
					tempIL.clear_InterLine(); 
				}
				else { 
				// FU 
					iterFuList = aFUList.begin(); 
					while ( iterFuList != aFUList.end() ) { 
//cout << *iterFuList; 
						iterInputIL_fu = iterFuList->inputILList.begin(); 
						front_or_back = 0; 
						while ( iterInputIL_fu != iterFuList->inputILList.end() ) { 
							if ( iterInputIL_fu->get_ilName() == tempIL.get_ilName() ) { 
								iterDFRSList_target = iterFuList; 
								iterFuList = aFUList.end(); 
								iterInputIL_fu = iterFuList->inputILList.end( ); 
							} 
							else { 
								++front_or_back; 
								++iterInputIL_fu; 
							}
						} 
						if ( iterFuList != aFUList.end() ) { 
							++iterFuList; 
						} 
					} 
					if ( front_or_back == 0 ) {
						iterDFRSList_target->popFront_inputILList(); 
						iterDFRSList_target->pushFront_inputILList( *iterInputIL_mux ); 
					}
					else { 
						iterDFRSList_target->popBack_inputILList(); 
						iterDFRSList_target->pushBack_inputILList( *iterInputIL_mux ); 
					}
					tempIL.clear_InterLine(); 
				} 
				// (2). delete the MUX's output of aILList 
				tempIL = iterMuxList->get_outputInterLine(); 
				iterInterLineList = aInterLineList.begin(); 
				while ( iterInterLineList != aInterLineList.end() ) { 
					if ( iterInterLineList->get_ilName() == tempIL.get_ilName() ) { 
						tempInterLineList.insert( tempInterLineList.end(), aInterLineList.begin(), iterInterLineList ); 
						++iterInterLineList; 
						tempInterLineList.insert( tempInterLineList.end(), iterInterLineList, aInterLineList.end() ); 
						iterInterLineList = aInterLineList.end(); 
					} 
					else { 
						++iterInterLineList; 
					} 
				} 
				aInterLineList.clear(); 
				aInterLineList.insert( aInterLineList.end(), tempInterLineList.begin(), tempInterLineList.end() ); 
				tempInterLineList.clear(); 
				tempIL.clear_InterLine(); 
				// (3). delete the MUX's sel of aCSList 
				tempCS = iterMuxList->get_aCtrlSignal(); 
				iterCtrlSignalList = aCtrlSignalList.begin(); 
				while ( iterCtrlSignalList != aCtrlSignalList.end() ) { 
					if ( iterCtrlSignalList->get_ctrlName() == tempCS.get_ctrlName() ) { 
						tempCtrlSignalList.insert( tempCtrlSignalList.end(), aCtrlSignalList.begin(), iterCtrlSignalList ); 
						++iterCtrlSignalList; 
						tempCtrlSignalList.insert( tempCtrlSignalList.end(), iterCtrlSignalList, aCtrlSignalList.end() ); 
						iterCtrlSignalList = aCtrlSignalList.end(); 
					} 
					else { 
						++iterCtrlSignalList; 
					} 
				} 
				aCtrlSignalList.clear(); 
				aCtrlSignalList.insert( aCtrlSignalList.end(), tempCtrlSignalList.begin(), tempCtrlSignalList.end() ); 
				tempCtrlSignalList.clear(); 
				tempCS.clear_CtrlSignal(); 
				// (4). delete the MUX of the aMuxList 
				tempMuxList.insert( tempMuxList.end(), aMuxList.begin(), iterMuxList ); 
				++iterMuxList; 
				tempMuxList.insert( tempMuxList.end(), iterMuxList, aMuxList.end() ); 
				iterMuxList = aMuxList.end(); 
				noRedundantMux = false; 
			} 
			else { 
				++iterMuxList; 
			} 
		} 
		if ( noRedundantMux == false ) { 
			aMuxList.clear(); 
			aMuxList.insert( aMuxList.end(), tempMuxList.begin(), tempMuxList.end() ); 
			tempMuxList.clear(); 
		} 
	}
	// re-assign the MUX id number and MUX name, and updata whole aRTLStruc ( 暫時未做 !! )
} 
CtrlSignal& RTLStruc::findAndAccessCS( string& aILName ) { 
	list<CtrlSignal>::iterator		iterCS; 
	list<CtrlSignal>::iterator		iterTargetCS; 
	iterCS = aCtrlSignalList.begin(); 
	while ( iterCS != aCtrlSignalList.end() ) { 
		if ( iterCS->get_ctrlName() == aILName ) { 
			iterTargetCS = iterCS; 
			iterCS = aCtrlSignalList.end(); 
		} 
		else { 
			++iterCS; 
		} 
	} 
	return *iterTargetCS; 
} 
bool RTLStruc::findAndUpdataCS( CtrlSignal& aCS ) { 
	list<CtrlSignal>::iterator			iterCSList;
	iterCSList = aCtrlSignalList.begin(); 
	while ( iterCSList != aCtrlSignalList.end() ) { 
		if ( iterCSList->get_ctrlName() == aCS.get_ctrlName() ) { 
			*iterCSList = aCS; 
			return true; 
		} 
		++iterCSList; 
	}
	return false; 
} 
void RTLStruc::detailConstructCtrlSignal( DFGs& aDFGs ) { 
	list<DataFlowRS>::iterator		iterFuList; 
	list<DataFlowRS>::iterator		iterMuxList; 
	list<DataFlowRS>::iterator		iterRegList; 
	list<InterLine>::iterator		iterInterLineList; 
	list<CtrlSignal>::iterator		iterCtrlSignalList; 
	InterLine						tempIL; 
	string							tempStrName; 
	CtrlSignal						tempCS; 
	Edge							tempEdge; 
	list<int>::iterator				iterIDList; 
	list<Graph>::iterator			iterGList; 
	// 1. deal with reg_XX_en ( according to REG/Edge beginTS )
	iterRegList = aRegList.begin(); 
	while ( iterRegList != aRegList.end() ) { 
		iterInterLineList = iterRegList->inputILList.begin(); 
		tempStrName = iterInterLineList->get_ilName(); 
		if ( tempStrName[0] == 'e' ) {	// if it is INP 
			tempCS = iterRegList->get_aCtrlSignal(); 
			tempCS.pushBack_cstepList( 0 ); 
			tempCS.pushBack_confList( 1 ); 
			iterRegList->set_aCtrlSignal( tempCS ); 
			findAndUpdataCS( tempCS );	// updata CS of aRTLStruc ! 
			tempCS.clear_CtrlSignal(); 
		} 
		else {	// if it is not INP 
			iterGList = aDFGs.gList.begin(); 
			tempCS = iterRegList->get_aCtrlSignal();
			iterIDList = iterRegList->neIDList.begin(); 
			while ( iterIDList != iterRegList->neIDList.end() ) { 
				tempEdge = aDFGs.findAndAccessEdgeInGList( iterGList->get_gID(), *iterIDList ); 
				tempCS.pushBack_cstepList( tempEdge.get_beginTStep() ); 
				tempCS.pushBack_confList( 1 ); 
				++iterIDList;
			}  
			iterRegList->set_aCtrlSignal( tempCS ); 
			findAndUpdataCS( tempCS );	// updata CS of aRTLStruc ! 
			tempCS.clear_CtrlSignal(); 
		} 
		++iterRegList; 
	} 
	// 2. deal with mux_XX_sel ( according to FU/OP tStep ) 
	// first part: the input port mux of the FU (sel cstep follow the FU, or endingTS)
	// second part: the input port mux of the REG (except INP reg) (sel cstep follow the REG, or beginTS) 
	iterMuxList = aMuxList.begin(); 
	while ( iterMuxList != aMuxList.end() ) { 
		tempIL = iterMuxList->get_outputInterLine(); 
		if ( tempIL.get_ilType() == IL_MUXOUT_TO_FU ) {		// (sel cstep follow the FU, or endingTS) 
			tempCS = iterMuxList->get_aCtrlSignal(); 
			iterIDList = tempCS.eIDList.begin(); 
			while ( iterIDList != tempCS.eIDList.end() ) { 
				tempEdge = aDFGs.findAndAccessEdgeInGList( iterGList->get_gID(), *iterIDList ); 
				tempCS.pushBack_cstepList( tempEdge.get_endTStep() ); 
				tempCS.pushBack_confList( 1 ); 
				++iterIDList; 
			} 
		} 
		else if ( tempIL.get_ilType() == IL_MUXOUT_TO_REG ) {	// (sel cstep follow the REG, or beginTS) 
			tempCS = iterMuxList->get_aCtrlSignal(); 
			iterIDList = tempCS.eIDList.begin(); 
			while ( iterIDList != tempCS.eIDList.end() ) { 
				tempEdge = aDFGs.findAndAccessEdgeInGList( iterGList->get_gID(), *iterIDList ); 
				tempCS.pushBack_cstepList( tempEdge.get_endTStep() ); 
				tempCS.pushBack_confList( 1 ); 
				++iterIDList; 
			} 
		} 
		else { 
			cout << "Warning ! No this iterMuxList->get_outputInterLine().get_ilType() matching ! \n"; 
		} 
		iterMuxList->set_aCtrlSignal( tempCS ); 
		findAndUpdataCS( tempCS );	// updata CS of aRTLStruc ! 
		tempCS.clear_CtrlSignal(); 
		++iterMuxList; 
	} 
} 
// *** End of Class RTLStruc *** 



// --------------------------------------------------------------------
// 1. Read files to setup the DFGs. (Read) --> (1), (2) 
// Function definition
// sysopt_(Step 1)
void readFileToSetDFGs( char* filename, DFGs& aDFGs ) {
	vector<string>	filenameArray;	// for storing filenames read from xxx.all 
	int		numOfFile;	// accumulate the number of files need to extract info. 
	// ----	
	string 		fName;		// temp store file name of the xxx.all
	string		baseName;	// for storing the basename of the filename 
	string		extName;	// for storing the extname of the filename  
	FileTag		fLabel;		// for extName format to choose case 
	// ---- 
	
	// (1). Read xxx.all, extracting which setup files need to be read, 
	//      setup aDFGs.set_numOfGraph(), setup aDFGs.addGraph_gList() 
	preReadFile( filename, aDFGs, filenameArray );

	// (2). Construct graphs, nodes, and edges from those files (.dfg, .edge, .tc, .rc, .str)
	numOfFile = filenameArray.size();
	//cout<<numOfFile<<endl;
	for (int j=0; j<numOfFile; j++) {
		fName = filenameArray[j];
		fileSplit( fName, baseName, extName );	// split fimename into baseName & extName 
		fLabel = fileFormat ( extName );	// set file format label
		switch ( fLabel ) {
			// ------- .dfg -------
			case F_DFG:
				// (2)-dfg. Read xxx.dfg, 
				//          setup aDFGs.constructNodeList(), aDFGs.setNodeProdecessor()
				readDfgFile( fName, aDFGs );
				break;
			// ------- .tc -------
			case F_TC:
				// (2)-tc. Read xxx.tc, 
				//         setup aDFGs.set_tConstraint(), setup aDFGs.setNumOfNodeInGraph() 
				readTcFile( fName, aDFGs );
				break;
			// ------- .rc -------
			case F_RC:
				// (2)-rc. Read xxx.rc, 
				//         setup aDFGs.set_rsActNumOfType(), setup aDFGs.init_rsConstraint(), setup aDFGs.set_rsConstraint()
				readRcFile( fName, aDFGs );
				break;
			// ------- .edge -------
			case F_EDGE:
				// (2)-edge. Read xxx.edge, setup aDFGs.constructEdgeList()
				readEdgeFile( fName, aDFGs );
				break;
			// ------- .str -------
			case F_STR:
				// (2)-tc. Read xxx.tc, setup strength (yet)
				readStrFile( fName, aDFGs );
				break;
			default: 
				cout << "File is not found !" << endl;
		}
	}// End of Construct graphs, nodes, and edges
	//aDFGs.set_clusSizeConstraint(); 
}


// ** (1). Read xxx.all
void preReadFile( char* filename, DFGs& aDFGs, vector<string>& filenameArray ) { 
	ifstream	fin; 
	char*		cstrLine = new char [LINE_SIZE];
	bool		isFirstLine;	// first line of xxx.all is number of graphs
	int		numOfDfgs;	// number of graphs
	string		fName;		// temp store file name of the xxx.all
	int		numOfFile;	// accumulate the number of files need to extract info. 
	//
	isFirstLine = true;		// initial isFirstLine as true
	numOfDfgs = 0;	// initial numOfDfgs as 0
	numOfFile = 0;	// initial numOfFile as 0
	fin.open( filename ); 
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << filename << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while( !fin.eof() ) {
		fin.getline( cstrLine, LINE_SIZE );
		if( isFirstLine ) { 
			numOfDfgs = stringNumberToIntNumber( cstrLine );
			isFirstLine = false;
			aDFGs.set_numOfGraph( numOfDfgs );		// % set number of graph in DFGs %
			for ( int k=0; k<numOfDfgs; ++k ) {		// % construct graph list in DFGs %
				aDFGs.addGraph_gList( k );
			}
		}
		else {
			fName = cstrLine;		// char* can be assigned to string ! 
			rmWhitespace( fName );		// Remove whitespace at head/tail of a string
			if ( !fName.empty() ) {		// check fName is valid
				++numOfFile;		// accumulate the number of files need to extract info.
				filenameArray.push_back( fName );		// put the filename into the fileNameArray
//				copy ( filenameArray.begin(), filenameArray.end(),										// --for test
//					   ostream_iterator<string>(cout," ") );	// cout the fileNameArray to screen		// --for test
//				cout << " " << numOfFile << endl;														// --for test
			}
		}
	}
	delete cstrLine; 	// release new char[] 
	fin.clear();
	fin.close();	// close file xxx.all
}


// ** Filename split into basename(prefix) and extname(suffix)
void fileSplit( string& fName, string& baseName, string& extName ) {
	string::size_type			idxStr;		// string index of the fName
	// 
	idxStr = fName.find('.');
	baseName = fName.substr(0, idxStr);
	extName = fName.substr(idxStr+1);
}
// End of Filename split


// ** File format label for construct graphs
FileTag fileFormat( const string& extn ) {
	if ( extn == SUBNAME_DFG )			return F_DFG;
	else if ( extn == SUBNAME_TC )		return F_TC;
	else if ( extn == SUBNAME_RC )		return F_RC;
	else if ( extn == SUBNAME_EDGE )	return F_EDGE;
	else if ( extn == SUBNAME_STR )		return F_STR;
	else return F_NONE;
}// End of File format label for construct graphs


// ** Node type label for setting resource constraints
NodeType setNodeType( const string& op ) {
	/* 20070405
	if ( op == NodeTypeName[0] || op == "UVLC" )			    	return UVLC;
	else if ( op == NodeTypeName[1] || op == "Lum16DC" )	    	return Lum16DC;
	else if ( op == NodeTypeName[2] || op == "ChrDC" )		    	return ChrDC;
	else if ( op == NodeTypeName[3] || op == "Lum16AC" )	    	return Lum16AC;
	else if ( op == NodeTypeName[4] || op == "Luma" )		    	return Luma;
	else if ( op == NodeTypeName[5] || op == "ChrAC" )		    	return ChrAC;
	else if ( op == NodeTypeName[10] || op == "Lum16DC_Hada_IQ" )	return Lum16DC_Hada_IQ;
	else if ( op == NodeTypeName[11] || op == "ChrDC_Hada_IQ" )		return ChrDC_Hada_IQ;
	else if ( op == NodeTypeName[12] || op == "Lum16AC_IQ" )		return Lum16AC_IQ;
	else if ( op == NodeTypeName[13] || op == "Luma_IQ" )			return Luma_IQ;
	else if ( op == NodeTypeName[14] || op == "ChrAC_IQ" )			return ChrAC_IQ;
	else if ( op == NodeTypeName[15] || op == "Luma_IDCT" )			return Luma_IDCT;
	else if ( op == NodeTypeName[16] || op == "Chroma_IDCT" )		return Chroma_IDCT;
	else if ( op == NodeTypeName[17] || op == "none" )				return NONE;
	else if ( op == NodeTypeName[18] || op == "nop" )				return NOP;
	else if ( op == NodeTypeName[19] || op == "SB_CM_read")         return SB_CM_read;
	else if ( op == NodeTypeName[20] || op == "SB_CM_write")        return SB_CM_write;
	else if ( op == NodeTypeName[21] || op == "DB_CM_read")         return DB_CM_read;
	else if ( op == NodeTypeName[22] || op == "DB_CM_write")        return DB_CM_write;
	else { 
		cout << "Error: can't set node type !" << endl;  
		return NOP; 
	}
	*/
	return NOP;
}// End of Node type label for setting resource constraints


// ** (2)-dfg. Read xxx.dfg
// sysopt_(Step 2)
void readDfgFile( string& fName, DFGs& aDFGs ) { 
	ifstream	fin;
	int		tempInt;			// temp store int number value
	char*		cstrLine = new char [LINE_SIZE];
	string		tempStr;			// temp storing cstrLine for dealing with read data
	string		tempNumStr;			// for dealing with read number values
	bool		conti;				// for control extract of the nItem 
	NodeItem	nItem;				// enum declaration, node item for constructing node
	int		nItemIdx;			// for nItem index increasing
	int		intArr[ARR_SIZE] = { 0 };	// temp intArr for construct nodes & edges
	//  
	tempInt = 0;	// clear for tempInt
	fin.open( fName.c_str() );	// can't use string, convert to char* by .c_str() !
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << fName << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) {	
		fin.getline( cstrLine, LINE_SIZE );
		tempStr = cstrLine;
		rmWhitespace( tempStr );	// Remove whitespace at head/tail of a string
		rmSharpComment( tempStr );	// Remove sharp(#) label
		if ( !tempStr.empty() ) {	// check tempStr is valid
			conti = true;		// reset the flag to extract word
			nItem = NITEM_GID;	// reset node item for constructing node
			nItemIdx = 0;		// for nItem index increasing 
			while ( conti ) {	// extract data from sLine(tempStr)
				tempNumStr = extractANumStr( tempStr, conti );	// extract number and remove number from tempStr
				tempInt = stringNumberToIntNumber( tempNumStr.c_str() );	// convert to int number
				switch ( nItemIdx ) {	// for node item index increasing
					case 0: nItem = NITEM_GID;		++nItemIdx;	intArr[0] = tempInt;	break;
					case 1: nItem = NITEM_NID;		++nItemIdx;	intArr[1] = tempInt;	break;
					case 2: nItem = NITEM_CID;		++nItemIdx;	intArr[2] = tempInt;	break;
					case 3: nItem = NITEM_DATAIN;		++nItemIdx;	intArr[3] = tempInt;	break;
					case 4: nItem = NITEM_DATAOUT;		++nItemIdx;	intArr[4] = tempInt;	break;
					case 5: nItem = NITEM_BUSINCYCLES;	++nItemIdx;	intArr[5] = tempInt;	break;
					case 6: nItem = NITEM_TC_IN;		++nItemIdx;	intArr[6] = tempInt;	break;
					case 7: nItem = NITEM_BUSOUTCYCLES;	++nItemIdx;	intArr[7] = tempInt;	break;
					case 8: nItem = NITEM_TC_OUT;				intArr[8] = tempInt;	break;
					default: cout  << "Error, no match node item index! \n";
				}
			}
			//cout<<intArr[4]<<" "<<intArr[5]<<endl;
			aDFGs.constructNodeList( intArr );	// % construct node in DFGs %
		}
	}
	//aDFGs.setNodeProdecessor();	// % set node prodecessor list in DFGs %
	//aDFGs.setNumOfNodeInGraph();
	delete cstrLine;	// release new char[] 
	fin.clear();	// clear fin flag
	fin.close();	// close fin
}


// ** (2)-tc. Read xxx.tc
void readTcFile( string& fName, DFGs& aDFGs ) {
	ifstream					fin;
	int							tempInt;
	// 
	tempInt = 0;	// clear for tempInt
	fin.open( fName.c_str() );	// can't use string, convert to char* by .c_str() !
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << fName << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) {		// Bug: only for one timing constraint !
		fin >> tempInt;		// the whole system only has one timing constraints 
	}
	aDFGs.set_tConstraint( tempInt );	// % set timing constraint %
	//aDFGs.setNumOfNodeInGraph();	// % set total number of nodes in each graph %
//	cout << tempInt << endl;																	// --for test
	fin.clear();	// clear fin flag
	fin.close();	// close fin
}


// ** (2)-rc. Read xxx.rc
void readRcFile( string& fName, DFGs& aDFGs ) {
	ifstream					fin;
	int							tempInt;	// temp store int number value
	char*						cstrLine = new char [LINE_SIZE];
	string						tempStr;	// temp storing cstrLine for dealing with read data
	bool						isFirstLine;	// first line shows the number of resource types have resource constraints
	string::size_type			indexStr;	// string index for string 
	string						strNodeType;	// store the extract of node type string
	string						strNodeRCNum;	// store the extract of node rc number string
	NodeType					nt;	// node type for set rc 
	//
	tempInt = 0;	// clear for tempInt
	isFirstLine = true;		// reset for first line flag
	fin.open( fName.c_str() );
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << fName << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) {	
		fin.getline( cstrLine, LINE_SIZE );
		if( isFirstLine ) { 
		tempInt = stringNumberToIntNumber( cstrLine );
		isFirstLine = false;
		aDFGs.set_rsActNumOfType( tempInt );	// % set number of graph in DFGs % 
		aDFGs.init_rsConstraint();		// % initialize resource constraints % (only one time) !
		}
		else {
			tempStr = cstrLine;
			rmWhitespace( tempStr );	// Remove whitespace at head/tail of a string
			if ( !tempStr.empty() ) {		// check tempStr is valid
				indexStr = tempStr.find_first_of(' ');		// detect break point
				strNodeType = tempStr.substr(0, indexStr);		// node type
				strNodeRCNum = tempStr.substr(indexStr+1);	// type resource number
				nt = setNodeType( strNodeType );		// set node type to nt
				tempInt = stringNumberToIntNumber( strNodeRCNum.c_str() );	// set number of resource
				aDFGs.set_rsConstraint( nt, tempInt );	// % set resource consraint %
			}
		}
	}
//	aDFGs.show_rsConstraint();																	// --for test
//	cout << aDFGs.get_rsActNumOfType() << endl;													// --for test
	delete cstrLine;	// release new char[] 
	fin.clear();	// clear fin flag
	fin.close();	// close fin
}


// ** (2)-edge. Read xxx.edge
void readEdgeFile( string& fName, DFGs& aDFGs ) {
	ifstream					fin;
	int							tempInt;	// temp store int number value
	char*						cstrLine = new char [LINE_SIZE];
	string						tempStr;	// temp storing cstrLine for dealing with read data
	string						tempNumStr;	// for dealing with read number values
	bool						conti;	// for control extract of the nItem 
	EdgeItem					eItem;	// enum declaration, edge item for constructing edge
	int							eItemIdx;	// for eItem index increasing
	int							intArr[ARR_SIZE] = { 0 };	// temp intArr for construct nodes & edges
	string						noteStr; // the edge note of INP & OUT 
	string::size_type			strIdx;	 // for erase the '#' symbol 
	//
	tempInt = 0;	// clear for tempInt
	fin.open( fName.c_str() );	// can't use string, convert to char* by .c_str() !
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << fName << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) {	
		fin.getline( cstrLine, LINE_SIZE );
		tempStr = cstrLine;
		rmWhitespace( tempStr );	// Remove whitespace at head/tail of a string
		// extract the note # info. ------- // ** 2005-10-03 演算法第二版, v2.3 !! ** 修改的 -- start 
		strIdx = tempStr.find_first_of( '#' ); 
		++strIdx; 
		noteStr.assign( tempStr, strIdx, tempStr.size() ); 
		rmSharpComment( tempStr );	// Remove sharp(#) label
		// extract the note # info. ------- // ** 2005-10-03 演算法第二版, v2.3 !! ** 修改的 -- end  
		if ( !tempStr.empty() ) {	// check tempStr is valid
			conti	= true;		// reset the flag to extract word
			eItemIdx = 0;		// 
			eItem = EITEM_GID;	// 
			while ( conti ) {
				tempNumStr = extractANumStr( tempStr, conti );	// extract number string and remove number from tempStr
				tempInt = stringNumberToIntNumber( tempNumStr.c_str() );	// convert to int number
				switch ( eItemIdx ) {	// for node item index increasing
					case 0: eItem = EITEM_GID;	++eItemIdx;		intArr[0] = tempInt;	break;
					case 1: eItem = EITEM_EID;	++eItemIdx;		intArr[1] = tempInt;	break;
					case 2: eItem = EITEM_ETYPE;	++eItemIdx;		intArr[2] = tempInt;	break;
					case 3: eItem = EITEM_FROMNID;	++eItemIdx;		intArr[3] = tempInt;	break;
					case 4: eItem = EITEM_TONID;	intArr[4] = tempInt;	break;
					default: cout  << "Error, no match edge item index! \n";
				}
			}
			aDFGs.constructEdgeList( intArr, noteStr );	// % construct node in DFGs %
		} 
	}
	delete cstrLine;	// release new char []
	fin.clear();	// clear fin flag
	fin.close();	// close fin 
}


// ** (2)-str. Read xxx.str
void readStrFile( string& fName, DFGs& aDFGs ) {
	ifstream					fin;
	int							tempInt;
	// 
	tempInt = 0;	// clear for tempInt
	fin.open( fName.c_str() );	// can't use string, convert to char* by .c_str() !
	if ( fin.fail() ) { 
	// The file does not exist ... 
		cout << "Error !! " << fName << " file does not exist !! " << endl << "Exit program !! "; 
		system("pause"); 
		exit(0); 
	} 
	while ( !fin.eof() ) {		// Bug: only for one timing constraint !
		fin >> tempInt;		// the whole system only has one timing constraints 
	}
//	cout << tempInt << endl;																	// --for test
	fin.clear();	// clear fin flag
	fin.close();	// close fin
}
// --------------------------------------------------------------------

/*
// --------------------------------------------------------------------
// 2. Generate the output file for DFGShow format. (Write) 
// Function definition
void opf_DFGShow( DFGs& aDFGs ) {
	ofstream					fout;
	list<Graph>::iterator		itergList;
	list<Node>::iterator		iternList;
	int							tempgID;
	int							maxTS;
	int							maxNS;
	int							tc;
	int							cutSize;
	string						tempStr;
	int							tempNum;
	list<int>					nsList;
	list<int>::iterator			iternsList;
	list<int>::iterator			iterSuccList;
	// Open and write file
	fout.open( OUTPUTFORMAT_DFGSHOW.c_str() );
	// format
	fout << "#-- " << "gID" 
		 << setw(8) << "maxH" 
		 << setw(8) << "maxW" 
		 << setw(10) << "cutSize" 
		 << setw(6) << "tc" 
		 << setw(6) << "rc" << "\n";
	fout << "#   "<< "nID" 
		 << setw(8) << "tASAP" 
		 << setw(8) << "opType" 
		 << setw(10) << "TimeStep" 
		 << setw(9) << "partiID" 
		 << setw(10) << "assignID" 
		 << setw(10) << "succList" << "\n";
	// Start extract info
	itergList = aDFGs.gList.begin();
	while ( itergList != aDFGs.gList.end() ) {
		tempgID = itergList->get_gID();		// set the gID
		maxTS = itergList->get_endTStep();	// set maxTS
		maxNS = 0;							// reset maxNS
		tc = aDFGs.get_tConstraint();		// set tc 
		cutSize = aDFGs.get_edgeCutNum();	// set cutSize
		// Set & write the Graph level info 
		// -- record the number of nodes in each time step 
		nsList.clear();
		for ( int iter=1; iter<=maxTS; ++iter) {
			nsList.push_back( 0 );
		}
		iternList = itergList->nList.begin();
		while ( iternList != itergList->nList.end() ) {
			if ( iternList->get_nType() != READ &&
				iternList->get_nType() != DOUT ) {
				iternsList = nsList.begin();
				for ( int idx=1; idx<iternList->get_tStep(); ++idx) {
					++iternsList;
				}
				*iternsList = *iternsList + 1;
			}
			++iternList;
		}
		// -- search the max number of nodes
		iternsList = nsList.begin();
		while ( iternsList != nsList.end() ) {
			if ( maxNS < *iternsList ) {
				maxNS = *iternsList;
			}
			++iternsList;
		}
		fout << "--  " << tempgID << setw(8) << maxTS << setw(8) << maxNS;
		fout << setw(8) << cutSize << setw(8) << tc << "    ";	// wrtie graph info ~~
		// -- set the rcStraing
		for ( int idxRs=0; idxRs < NumOfNodeType; ++idxRs ) {
			if ( aDFGs.rsConstraint[idxRs] != -1 ) {
				tempStr = NodeTypeName[idxRs];
				tempNum = aDFGs.rsConstraint[idxRs];
				tempStr.append(1, ' ');
				fout << tempStr << tempNum << ' ';	// wrtie graph info ~~
			}
		}
		fout << endl;	// wrtie graph info ~~

		// Set & write the Node level info 
		iternList = itergList->nList.begin();
		while ( iternList != itergList->nList.end() ) {
			if ( iternList->get_nType() != READ &&
				iternList->get_nType() != DOUT ) {
				fout << setw(5) << iternList->get_nID() 
					 << setw(8) << iternList->get_tASAP() 
					 << setw(8) << iternList->get_nType() 
					 << setw(8) << iternList->get_tStep() 
//					 << setw(8) << iternList->get_clusID()		// for test rsPartitionng result !
					 << setw(8) << iternList->get_partiID() 
					 << setw(8) << iternList->get_assignID() 
					 << setw(8);	// wrtie node info ~~
				// -- node successor
				iterSuccList = iternList->successor.begin();
				while ( iterSuccList != iternList->successor.end() ) {
					fout << *iterSuccList << setw(4);
					++iterSuccList;
				}
				fout << endl;	// write endl ~~
			}
			++iternList;
		}
		fout << "*****" << endl;	// write break graph ~~
		++itergList;
	}
	// ---- show the current time information ---- 
	time_t				ltime;		// for show the current time 
	time( &ltime ); 
	fout << "The running program at " << ctime( &ltime );
	// ---- End of  show the current time information ---- 
	// ---- show the execution time until now ---- 
	fout << "The total execution time : " << (clock() / CLOCKS_PER_SEC) << " sec !! " << endl; 
	// ---- End of show the execution time until now ---- 
	fout << "*****" << endl;
	fout.clear();
	fout.close();
}// End of "2. Generate the output file for DFGShow format. (Write)"
// --------------------------------------------------------------------

/*
// --------------------------------------------------------------------
// 3. Generate the output file to record the DFGs cout. (Write) 
void opf_DFGsLog( DFGs& aDFGs, char * fileName ) { 
	ofstream			ffout;
	string				tempStr = fileName; 
	string				dfgStr; 
	string::size_type	idx; 
	// Write out the data into the file 
	idx = tempStr.find_first_of( '.' ); 
	dfgStr = tempStr.substr( 0, idx ); 
	ffout.open( OUTPUTFORMAT_DFG.c_str() ); 
	ffout << "############################################################################### " << endl 
		  << "Program Configuration .... " << dfgStr << endl 
		  << "PARTI_MODE = " << PARTI_MODE << '\t' << "(0:no, 1:2-way, 2:k-way ...) " << endl 
		  << "K_WAY = " << K_WAY << '\t' << "(no:1, 2-way:2, k-way:k ... ) " << endl 
		  << "FMPARTI_MODE = " << FMPARTI_MODE << '\t' << "(0: run one time, 1: run N times to get one best ...) " << endl 
		  << "REG_ASS_MODE = " << REG_ASS_MODE << '\t' << "(0: left edge algorithm, 1: my modified algorithm ...) " << endl 
		  << "INP_REG_USED_MODE = " << INP_REG_USED_MODE << '\t' << "(0: can be re-used, 1: can't be re-used ...) " << endl 
		  << "FM_ITERATIVE_RUN_CRITERIA = " << FM_ITERATIVE_RUN_CRITERIA << endl 
		  << "############################################################################### " << endl; 
	ffout << aDFGs; 
	ffout << "******" << endl;
	// ---- show the current time information ---- 
	time_t				ltime;		// for show the current time 
	time( &ltime ); 
	ffout << "The running program at " << ctime( &ltime );
	// ---- End of  show the current time information ---- 
	// ---- show the execution time until now ---- 
	ffout << "The total execution time : " << (clock() / CLOCKS_PER_SEC) << " sec !! " << endl; 
	// ---- End of show the execution time until now ---- 
	ffout << "******" << endl;
	ffout.clear(); 
	ffout.close();
}// End of "3. Generate the output file to record the DFGs cout. (Write)"
// --------------------------------------------------------------------


// --------------------------------------------------------------------
// 4. Generate the output file for VDHL generator. (Write)
void opf_DFGsToVHDLGen( char* fileCstr, DFGs& aDFGs ) { 
	RTLStruc						aRTLStruc; 
	DataFlowRS						tempDataFlowRS; 
	InterLine						tempInterLine; 
	CtrlSignal						tempCtrlSignal; 
	DFRSid							tempDFRSid; 
	list<DataFlowRS>::iterator		iterDataFlowRS; 
	ofstream						fout; 
	string							VHDLGenFormatVersion = "TBL 4.01"; 
	string							tempFilename;	// for storing the filename from fileCstr 
	string							baseName;	// for storing the basename of the filename 
	string							extName;	// for storing the extname of the filename  
	string							output_filename;	// the output filename 
	list<Graph>::iterator			iterGList; 
	int								numNode;
	int								numEdge;
	int								maxcstep; 
	int								numConf = 1;	// pre-assign as 1 
	int								conf; 
	ifstream						fin;	// for extract the edge note 
	string							edgeFilename; 
	char*							cstrLine = new char [LINE_SIZE]; 
	string							tempStr; // for deal with the cstrLine 
	string							tempValStr;	// for deal with the tempStr to get the number value 
	int								currENum;	// current edge number for the input edge 
	int								currMuxNum;	// current mux number for constructing the mux 
	int								currRegNum;	// current reg number for constructing the reg 
	int								currFUOutNum;	// current fu output number for constructing the fu 
	int								tempFUNum;	// for constructing the fu id 
	int								tempFUNum_local; // for constructing opAssID_to_fuName
	list<Node>::iterator			iterNList; 
	list<Edge>::iterator			iterEList; 
	NodeType						tempNType; 
	int								tempOPAssID; // op assID 
	string							fuName;		 // fu name 
	list<int>						opAssIDList; // the corresponding fuAssIDList for search back to DFGs data 
	list<int>						fuAssIDList;
	list<int>::iterator				iter_opAssIDList;
	int								totRsNum; 
	vector<string>					opAssID_to_fuName;	// the opAssID mapping to the fu Name by the opAssID number ordering 
	vector<int>						fuAssIDTable; 
	string							tempStrName; // for set the InterLine ilName 
	char							c_strNum[10]; 
	string							strNum;
	list<int>::iterator				iterPredIDList_a;	// of FU 
	list<int>::iterator				iterPredIDList_b;	// of FU 
	Edge							tempEdge_in_a;		// of FU 
	Edge							tempEdge_in_b;		// of FU 
	int								tempEID;			// of FU
	list<int>::iterator				iterSuccIDList;		// of FU 
	EdgeType						tempEType;		// for REG 
	int								tempMRegID;		// for REG 
	list<int>						eMRegIDList;	// for REG, the corresponding rsRegIDList for search back to DFGs data 
	list<int>						rsRegIDList;	// for REG, the corresponding eMRegIDList for search back to DFGs data 
	Node							tempNode;	// for search the fu output edge name in the output InterLine 
	list<InterLine>::iterator		iterILList;		// the aInterLineList iterator of aRTLStruc, or the inputILList of DataFlowRS 
	list<int>						eMRegIDListForMux;	// for MUX, the corresponding rsMuxIDListForMux for search back to DFGs data 
	list<int>						rsMuxIDListForMux;	// for MUX, the corresponding eMRegIDListForMux for search back to DFGs data 
	list<int>::iterator				iterNEIDList;		// for MUX 
	int								tempPartiID;		//	// -- 2005.11.21 added the rsPartiID -- 
	list<int>						fuInMuxPartiIDList; // construct from 3.op, using on 6.mux  // -- 2005.11.21 added the rsPartiID -- 
	list<int>::iterator				iterMuxPartiIDList; //  // -- 2005.11.21 added the rsPartiID -- 
	// Write out the data into the file 
	// 1. write some dfg graph info into the output filename 
	numNode = 0;
	numEdge = 0;
	maxcstep = 0;
	conf = 1; 
	tempFilename = fileCstr; 
	fileSplit( tempFilename, baseName, extName ); 
	output_filename = baseName + "." + OUTPUTFORMAT_SUBNAME_DFG_TO_VHDLGEN; 
	fout.open( output_filename.c_str() ); 
	fout << "# " << output_filename << endl;	// write the output filename in this file 
	fout << "# " << "Format = " << VHDLGenFormatVersion << endl;	// write the format version in this file 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		numNode = numNode + iterGList->nList.size();
		numEdge = numEdge + iterGList->eList.size();
		++iterGList;
	}
	fout << "# " << "numNode = " << numNode << endl;	// write the number of nodes in this file 
	fout << "# " << "numEdge = " << numEdge << endl;	// write the number of edges in this file 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		if ( iterGList->get_endTStep() > maxcstep ) {
			maxcstep = iterGList->get_endTStep();
		}
		++iterGList;
	}
	fout << "-- " << "maxcstep = " << maxcstep << endl;	// write the number of csteps in this file 
	fout << "-- " << "numConf = " << numConf << endl;	// write the number of configurations in this file
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) {	// write the cstep of each configuration in this file 
		fout << "-- " << "Conf " << conf << " steps = " << iterGList->get_endTStep() << endl;	
		++iterGList;
	} 
	fout << "-- " << "numDataWidth = " << (INOUT_DATA_BITWIDTH - 1) << endl;	// write the number of DataWidth in this file
	// 2. set the initial structure id number 
	currENum = EDGE_START_ID_REGION + 1; 
	currMuxNum = 0;		// id start at 0 
	currRegNum = 0;		// id start at 0 
	currFUOutNum = 0;	// id start at 0 
	tempFUNum = 0;		// id start at 0 
	tempFUNum_local = 0; 
	tempNType = NONE; 
	tempOPAssID = 0; 
	// 3. construct the op sructure 
	// pre-assign the fuName and updata the opAssID_to_fuName, and construct the mapping opAssIDList
	totRsNum = 0; 
	for ( int m = aDFGs.rsConstraint.size(); m > 0; m-- ) { 
		fuAssIDTable.push_back( 0 );	// initial the fuAssIDTable of each fu type
		if ( aDFGs.rsConstraint[m] > 0 ) { 
			totRsNum = totRsNum + aDFGs.rsConstraint[m];	// total number of resources 
		} 
	} 
	for ( int n = 0; n < totRsNum; ++n ) { 
		opAssID_to_fuName.push_back( "NULL" );	// record the opAssID and its corresponding fuName 
	} 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterNList = iterGList->nList.begin(); 
		while ( iterNList != iterGList->nList.end() ) { 
			tempNType = iterNList->get_nType(); 
			if ( tempNType != READ && tempNType != DOUT ) { 
				tempOPAssID = iterNList->get_assignID(); 
				iter_opAssIDList = find( opAssIDList.begin(), opAssIDList.end(), tempOPAssID ); 
				if ( iter_opAssIDList == opAssIDList.end() ) {	// if not find 
					opAssIDList.push_back( tempOPAssID ); 
					fuAssIDList.push_back( tempFUNum ); 
					++tempFUNum; 
					tempFUNum_local = fuAssIDTable[ tempNType ]; 
					fuAssIDTable[ tempNType ] = fuAssIDTable[ tempNType ] + 1; 
					_itoa( tempFUNum_local, c_strNum, 10 ); 
					strNum = c_strNum;
					fuName = NodeTypeName[iterNList->get_nType()] + strNum; 
					opAssID_to_fuName[ tempOPAssID-1 ] = fuName;	// set the opAssID_to_fuName according to its opAssID ! 
				}
			}
			++iterNList;
		} 
		++iterGList; 
	} 
//copy(opAssID_to_fuName.begin(), opAssID_to_fuName.end(), ostream_iterator<string>(cout," "));	cout << endl;
	// deal with the total DFGs 
	tempEID = -1; 
	opAssIDList.clear(); 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterNList = iterGList->nList.begin(); 
		while ( iterNList != iterGList->nList.end() ) { 
			tempNType = iterNList->get_nType(); 
			if ( tempNType != READ && tempNType != DOUT ) { 
				tempOPAssID = iterNList->get_assignID(); 
				iter_opAssIDList = find( opAssIDList.begin(), opAssIDList.end(), tempOPAssID ); 
				if ( iter_opAssIDList == opAssIDList.end() ) {	// if not find 
					opAssIDList.push_back( tempOPAssID ); 
					// construct a fu --> theDFRSid, theFUType, theFUSubType, 1 * neIDList, 2 * inputILList, 1 * outputInterLine 
					fuName = findTheMappingFUNameByOPAssID( opAssID_to_fuName, opAssIDList, tempOPAssID ); 
					tempDFRSid.set_dfrsID( findTheMappingFUNumByOPAssID( opAssIDList, fuAssIDList, tempOPAssID ) ); 
					tempDFRSid.set_dfrsType( DFRS_FU ); 
					tempDFRSid.set_dfrsName( fuName );	// don't use preset_dfrsName() 
					tempDataFlowRS.set_theDFRSid( tempDFRSid ); 
					tempDataFlowRS.set_theFUType( tempNType ); 
					tempDataFlowRS.set_rsPartiID( iterNList->get_partiID() );	// -- 2005.11.21 added the rsPartiID -- 
					if ( tempNType == MUL ) { 
						tempDataFlowRS.set_theFUSubType( FU_MUL_NBW ); 
					} 
					else if ( tempNType == ADD ) { 
						tempDataFlowRS.set_theFUSubType( FU_ADD_CLA ); 
					} 
					else if ( tempNType == SUB ) { 
						tempDataFlowRS.set_theFUSubType( FU_SUB_CLA ); 
					} 
					else if ( tempNType == COMP ) { 
						tempDataFlowRS.set_theFUSubType( FU_COMP_CLA ); 
					} 
					else { 
						cout << "Warning !! the fu sub-type can't be found !! \n"; 
						system("pause"); 
						exit(0);	// exit this program 
					} 
					tempDataFlowRS.pushBack_neIDList( iterNList->get_nID() ); 
					// input line * 2 ( mux_XX_out )
					iterPredIDList_a = iterNList->predecessor.begin(); 
					iterPredIDList_b = iterPredIDList_a; 
					++iterPredIDList_b; 
					if ( *iterPredIDList_a < *iterPredIDList_b ) {	// smaller is the 被X數, larger is the X數 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_a, iterNList->get_nID() ); 
						tempEdge_in_b = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_b, iterNList->get_nID() ); 
					} 
					else { 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_b, iterNList->get_nID() ); 
						tempEdge_in_b = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_a, iterNList->get_nID() ); 
					} 
					tempEID = tempEdge_in_a.get_eID(); 
					for ( int i = 0; i < 2; ++i ) { 
						_itoa( currMuxNum, c_strNum, 10 ); 
						strNum = c_strNum;
						tempStrName = "mux_" + strNum + "_out"; 
						tempInterLine.set_ilID( currMuxNum ); 
						tempInterLine.set_ilType( IL_MUXOUT_TO_FU ); 
						tempInterLine.set_ilName( tempStrName ); 
						tempInterLine.pushBack_eIDList( tempEID ); 
						tempEID = tempEdge_in_b.get_eID();
						fuInMuxPartiIDList.push_back( iterNList->get_partiID() );	// -- 2005.11.21 added the rsPartiID -- 
						++currMuxNum; 
						tempDataFlowRS.pushBack_inputILList( tempInterLine ); 
						aRTLStruc.pushBack_aInterLineList( tempInterLine );	// add IL into aRTLStruc ! 
						tempInterLine.clear_InterLine(); 
					} 
					// output line ( MULX_out ) 
					tempEID = -1; 
					currFUOutNum = findTheMappingFUNumByOPAssID( opAssIDList, fuAssIDList, tempOPAssID ); 
					tempInterLine.set_ilID( currFUOutNum ); 
					tempInterLine.set_ilType( IL_FUOUT_TO_MUX ); 
					tempStrName = fuName + "_out"; 
					tempInterLine.set_ilName( tempStrName ); 
					iterSuccIDList = iterNList->successor.begin();
					while ( iterSuccIDList != iterNList->successor.end() ) { 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), iterNList->get_nID(), 
							*iterSuccIDList ); 
						tempEID = tempEdge_in_a.get_eID(); 
						tempInterLine.pushBack_eIDList( tempEID ); 
						++iterSuccIDList; 
					} 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					aRTLStruc.pushBack_aInterLineList( tempInterLine );		// add IL into aRTLStruc ! 
					tempInterLine.clear_InterLine(); 
					tempDataFlowRS.set_inputNum( 2 ); 
					tempDataFlowRS.set_outputNum( 1 ); 
					aRTLStruc.pushBack_aFUList( tempDataFlowRS );	// add FU into aRTLStruc ! 
					tempDataFlowRS.clear_DataFlowRS(); 
				} 
				else {	// if find 
					// construct a fu --> theDFRSid, theFUType, theFUSubType, 1 * neIDList, 2 * inputILList, 1 * outputInterLine 
//copy(opAssIDList.begin(),opAssIDList.end(),ostream_iterator<int>(cout," "));	cout << endl;
//copy(fuAssIDList.begin(),fuAssIDList.end(),ostream_iterator<int>(cout," "));	cout << endl;
					fuName = findTheMappingFUNameByOPAssID( opAssID_to_fuName, opAssIDList, tempOPAssID ); 
					tempDFRSid.set_dfrsID( findTheMappingFUNumByOPAssID( opAssIDList, fuAssIDList, tempOPAssID ) ); 
					tempDFRSid.set_dfrsType( DFRS_FU ); 
					tempDFRSid.set_dfrsName( fuName ); 
					tempDataFlowRS = aRTLStruc.findAndAccessDFRS( tempDFRSid ); 
					tempDataFlowRS.pushBack_neIDList( iterNList->get_nID() ); 
					// input line * 2 ( mux_XX_out )
					iterPredIDList_a = iterNList->predecessor.begin(); 
					iterPredIDList_b = iterPredIDList_a; 
					++iterPredIDList_b; 
					if ( *iterPredIDList_a < *iterPredIDList_b ) {	// smaller is the 被X數, larger is the X數 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_a, iterNList->get_nID() ); 
						tempEdge_in_b = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_b, iterNList->get_nID() ); 
					} 
					else { 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_b, iterNList->get_nID() ); 
						tempEdge_in_b = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), *iterPredIDList_a, iterNList->get_nID() ); 
					} 
					tempEID = tempEdge_in_a.get_eID(); 
					iterILList = tempDataFlowRS.inputILList.begin(); 
					while ( iterILList != tempDataFlowRS.inputILList.end() ) {	// only 2 input IL 
						iterILList->pushBack_eIDList( tempEID ); 
						aRTLStruc.findAndUpdataIL( *iterILList );		// updata aRTLStruc's aInterLineList ! 
						tempEID = tempEdge_in_b.get_eID(); 
						++iterILList; 
					} 
					// output line ( MULX_out ) 
					tempEID = -1; 
					tempInterLine = tempDataFlowRS.get_outputInterLine(); 
					iterSuccIDList = iterNList->successor.begin(); 
					while ( iterSuccIDList != iterNList->successor.end() ) { 
						tempEdge_in_a = aDFGs.findAndAccessEdgeInGList( iterNList->get_gID(), iterNList->get_nID(), *iterSuccIDList ); 
						tempEID = tempEdge_in_a.get_eID(); 
						tempInterLine.pushBack_eIDList( tempEID ); 
						++iterSuccIDList; 
					} 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					aRTLStruc.findAndUpdataIL( tempInterLine );		// updata aRTLStruc's aInterLineList ! 
					tempInterLine.clear_InterLine(); 
					aRTLStruc.findAndUpdataDFRS( tempDataFlowRS );	// updata aRTLStruc's aFUList ! 
					tempDataFlowRS.clear_DataFlowRS(); 
				} 
			} 
			++iterNList; 
		} 
		++iterGList;
	} 
//copy(opAssID_to_fuName.begin(), opAssID_to_fuName.end(), ostream_iterator<string>(cout," "));	cout << endl;
//cout << aRTLStruc << endl; 
	// 4. construct the I/O port of this design 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == INP || tempEType == OUT ) { 
				tempInterLine.set_ilID( currENum ); 
				tempInterLine.set_bitNum( INOUT_DATA_BITWIDTH ); 
				tempInterLine.set_ilNote( iterEList->get_edgeNote() ); 
				tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
				if ( tempEType == INP ) {	// if the edge is INP 
					_itoa( currENum, c_strNum, 10 ); 
					strNum = c_strNum;
					tempStrName = "e" + strNum; 
					tempInterLine.set_ilName( tempStrName ); 
					tempInterLine.set_ilType( IL_INPUT ); 
				} 
				else {		// if the edge is OUT 
					tempNode = aDFGs.findAndAccessNodeInGList( iterEList->get_gID(), iterEList->get_from_nID() ); 
					tempStrName = findTheMappingFUNameByOPAssID( opAssID_to_fuName, opAssIDList, tempNode.get_assignID() ); 
					tempStrName = tempStrName + "_out"; 
					tempInterLine.set_ilName( tempStrName ); 
					tempInterLine.set_ilType( IL_OUTPUT ); 
				} 
				++currENum;		// increase the E id number 
				aRTLStruc.pushBack_aInterLineList( tempInterLine );	// add IL into aRTLStruc ! 
				tempInterLine.clear_InterLine(); 
			} 
			++iterEList; 
		} 
		++iterGList;
	} 
//cout << aRTLStruc << endl; 
	// 5. construct the REG structure 
	// preset the regID mapping table with the real regNum 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == INP ) { 
				eMRegIDList.push_back( convertRegIDToMRegID(*iterEList) ); 
				rsRegIDList.push_back( currRegNum ); 
				++currRegNum; 
			} 			
			++iterEList; 
		} 
		++iterGList;
	} 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == FW ) { 
				tempMRegID = convertRegIDToMRegID( *iterEList ); 
				if ( find(eMRegIDList.begin(),eMRegIDList.end(),tempMRegID) == eMRegIDList.end() ) { 
					eMRegIDList.push_back( tempMRegID ); 
					rsRegIDList.push_back( currRegNum ); 
					++currRegNum; 
				}
			} 			
			++iterEList; 
		} 
		++iterGList;
	} 
//copy(eMRegIDList.begin(),eMRegIDList.end(),ostream_iterator<int>(cout," "));	cout << endl;
//copy(rsRegIDList.begin(),rsRegIDList.end(),ostream_iterator<int>(cout," "));	cout << endl;
	// first part: assign the input edge reg 
	currRegNum = 0; 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			tempPartiID = -1; 
			if ( tempEType == INP ) { 
				tempMRegID = convertRegIDToMRegID( *iterEList ); 
				currRegNum = findTheMappingRSRegIDByMRegID( eMRegIDList, rsRegIDList, tempMRegID ); 
				tempDFRSid.set_dfrsType( DFRS_REG ); 
				tempDFRSid.set_dfrsID( currRegNum ); 
				tempDFRSid.preset_dfrsName(); 
				tempDataFlowRS.set_theDFRSid( tempDFRSid ); 
				tempDataFlowRS.set_bitNum( INOUT_DATA_BITWIDTH ); 
				tempDataFlowRS.set_inputNum( 1 ); 
				tempDataFlowRS.set_outputNum( 1 );  
				tempPartiID = searchPartiIDBySuccessorOfEdge( aDFGs, *iterEList );  // -- 2005.11.21 added the rsPartiID -- 
				tempDataFlowRS.set_rsPartiID( tempPartiID );						// -- 2005.11.21 added the rsPartiID -- 
				iterEList->set_partiID( tempPartiID ); 								// -- 2005.11.21 added the rsPartiID -- 
				tempDataFlowRS.pushBack_neIDList( iterEList->get_eID() ); 
				// ctrl line ( reg_XX_en ) 
				_itoa( currRegNum, c_strNum, 10 ); 
				strNum = c_strNum; 
				tempStrName = CtrlTypeNameBeg[DFRS_REG] + "_" + strNum + "_" + CtrlTypeNameEnd[DFRS_REG]; 
				tempCtrlSignal.set_ctrlType( DFRS_REG ); 
				tempCtrlSignal.set_ctrlName( tempStrName ); 
				tempCtrlSignal.set_accessNum( 1 );	// because it is a input data reg 
				tempDataFlowRS.set_aCtrlSignal( tempCtrlSignal ); 
				aRTLStruc.pushBack_aCtrlSignalList( tempCtrlSignal );	// add REG's CS into aRTLStruc ! 
				tempCtrlSignal.clear_CtrlSignal(); 
				// input line ( eXXXX ) 
				tempInterLine = aRTLStruc.findAndAccessIL( IL_INPUT, iterEList->get_eID() ); 
				tempDataFlowRS.pushBack_inputILList( tempInterLine ); 
				tempInterLine.clear_InterLine(); 
				// output line ( reg_XX_out ) 
				_itoa( currRegNum, c_strNum, 10 ); 
				strNum = c_strNum;
				tempStrName = ILLineNameBeg[DFRS_REG] + "_" + strNum + "_" + ILLineNameEnd; 
				tempInterLine.set_ilID( currRegNum ); 
				tempInterLine.set_ilType( IL_REGOUT_TO_MUX ); 
				tempInterLine.set_bitNum( INOUT_DATA_BITWIDTH ); 
				tempInterLine.set_ilName( tempStrName ); 
				tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
				tempDataFlowRS.set_outputInterLine( tempInterLine ); 
				aRTLStruc.pushBack_aInterLineList( tempInterLine );	// add IL into aRTLStruc ! 
				tempInterLine.clear_InterLine(); 
				aRTLStruc.pushBack_aRegList( tempDataFlowRS );	// add REG into aRTLStruc ! 
				tempDataFlowRS.clear_DataFlowRS(); 
			} 
			else if ( tempEType == OUT ) {											// -- 2005.11.21 added the rsPartiID -- 
				tempPartiID = searchPartiIDBySuccessorOfEdge( aDFGs, *iterEList );  // -- 2005.11.21 added the rsPartiID -- 
				iterEList->set_partiID( tempPartiID ); 								// -- 2005.11.21 added the rsPartiID -- 
			}																		// -- 2005.11.21 added the rsPartiID -- 
			++iterEList; 
		} 
		++iterGList;
	} 
	// second part: assign the op output through the mux to the reg 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == FW ) { 
				tempMRegID = convertRegIDToMRegID( *iterEList ); 
				currRegNum = findTheMappingRSRegIDByMRegID( eMRegIDList, rsRegIDList, tempMRegID ); 
				tempDFRSid.set_dfrsType( DFRS_REG ); 
				tempDFRSid.set_dfrsID( currRegNum ); 
				tempDFRSid.preset_dfrsName(); 
				if ( !aRTLStruc.searchDFRS( tempDFRSid ) ) {	// if not find 
					tempDataFlowRS.set_theDFRSid( tempDFRSid ); 
					tempDataFlowRS.set_bitNum( INOUT_DATA_BITWIDTH ); 
					tempDataFlowRS.set_inputNum( 1 ); 
					tempDataFlowRS.set_outputNum( 1 );  
					tempDataFlowRS.set_rsPartiID( iterEList->get_partiID() );		// -- 2005.11.21 added the rsPartiID -- 
					tempDataFlowRS.pushBack_neIDList( iterEList->get_eID() ); 
					// ctrl line ( reg_XX_en ) 
					_itoa( currRegNum, c_strNum, 10 ); 
					strNum = c_strNum;
					tempStrName = CtrlTypeNameBeg[DFRS_REG] + "_" + strNum + "_" + CtrlTypeNameEnd[DFRS_REG]; 
					tempCtrlSignal.set_ctrlType( DFRS_REG ); 
					tempCtrlSignal.set_ctrlName( tempStrName ); 
					tempDataFlowRS.set_aCtrlSignal( tempCtrlSignal ); 
					aRTLStruc.pushBack_aCtrlSignalList( tempCtrlSignal );	// add REG's CS into aRTLStruc ! 
					tempCtrlSignal.clear_CtrlSignal(); 
					// input line 未, 目前資料不夠放到後面再做 ! 
					// output line ( reg_XX_out ) 
					_itoa( currRegNum, c_strNum, 10 ); 
					strNum = c_strNum;
					tempStrName = ILLineNameBeg[DFRS_REG] + "_" + strNum + "_" + ILLineNameEnd; 
					tempInterLine.set_ilID( currRegNum ); 
					tempInterLine.set_ilType( IL_REGOUT_TO_MUX ); 
					tempInterLine.set_bitNum( INOUT_DATA_BITWIDTH ); 
					tempInterLine.set_ilName( tempStrName ); 
					tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					aRTLStruc.pushBack_aInterLineList( tempInterLine );	// add IL into aRTLStruc ! 
					tempInterLine.clear_InterLine(); 
					aRTLStruc.pushBack_aRegList( tempDataFlowRS );	// add REG into aRTLStruc ! 
					tempDataFlowRS.clear_DataFlowRS();
				} 
				else {	// if find 
					tempDataFlowRS = aRTLStruc.findAndAccessDFRS( tempDFRSid ); 
					tempInterLine = tempDataFlowRS.get_outputInterLine(); 
					tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					tempDataFlowRS.pushBack_neIDList( iterEList->get_eID() ); 
					aRTLStruc.findAndUpdataIL( tempInterLine ); 	// updata IL of aRTLStruc ! 
					tempInterLine.clear_InterLine(); 
					aRTLStruc.findAndUpdataDFRS( tempDataFlowRS );	// updata REG of aRTLStruc ! 
					tempDataFlowRS.clear_DataFlowRS(); 
				} 
			} 
			++iterEList; 
		} 
		++iterGList;
	} 
//cout << aRTLStruc << endl;
	// 6. construct the MUX structure 
	// first part: the input port mux of the FU 
	iterMuxPartiIDList = fuInMuxPartiIDList.begin();							// -- 2005.11.21 added the rsPartiID -- 
	for ( int j = 0; j < currMuxNum; ++j ) {	// currMuxNum = the number that the previous FU assignment calculates the mux number 
		tempDFRSid.set_dfrsType( DFRS_MUX ); 
		tempDFRSid.set_dfrsID( j ); 
		tempDFRSid.preset_dfrsName(); 
		tempDataFlowRS.set_theDFRSid( tempDFRSid ); 
		tempDataFlowRS.set_muxConfType( MUX_C ); 
		tempDataFlowRS.set_outputNum( 1 ); 
		tempDataFlowRS.set_rsPartiID( *iterMuxPartiIDList );					// -- 2005.11.21 added the rsPartiID -- 
		++iterMuxPartiIDList;													// -- 2005.11.21 added the rsPartiID -- 
		// ctrl line ( mux_XX_sel ) 
		_itoa( j, c_strNum, 10 ); 
		strNum = c_strNum; 
		tempStrName = CtrlTypeNameBeg[DFRS_MUX] + "_" + strNum + "_" + CtrlTypeNameEnd[DFRS_MUX]; 
		tempCtrlSignal.set_ctrlType( DFRS_MUX ); 
		tempCtrlSignal.set_ctrlName( tempStrName ); 
		tempCtrlSignal.set_muxConfType( MUX_C ); 
		tempDataFlowRS.set_aCtrlSignal( tempCtrlSignal ); 
		aRTLStruc.pushBack_aCtrlSignalList( tempCtrlSignal );	// add MUX's CS into aRTLStruc ! 
		tempCtrlSignal.clear_CtrlSignal(); 
		// input line ( unknow ) 未?! 
		// output line ( mux_XX_out ) 
		_itoa( j, c_strNum, 10 ); 
		strNum = c_strNum; 
		tempStrName = ILLineNameBeg[DFRS_MUX] + "_" + strNum + "_" + ILLineNameEnd; 
		tempInterLine = aRTLStruc.findAndAccessIL( tempStrName ); 
		tempDataFlowRS.set_outputInterLine( tempInterLine ); 
		iterNEIDList = tempInterLine.eIDList.begin(); 
		while ( iterNEIDList != tempInterLine.eIDList.end() ) { 
			tempDataFlowRS.pushBack_neIDList( *iterNEIDList ); 
			++iterNEIDList; 
		} 
		tempInterLine.clear_InterLine(); 
		aRTLStruc.pushBack_aMuxList( tempDataFlowRS );	// add MUX into aRTLStruc ! 
		tempDataFlowRS.clear_DataFlowRS(); 
	} 
	// second part: the input port mux of the REG (except INP reg) 
	// preset the eMRegIDListForMux and rsMuxIDListForMux for mapping edge and Mux 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == FW ) { 
				tempMRegID = convertRegIDToMRegID( *iterEList ); 
				if ( find(eMRegIDListForMux.begin(),eMRegIDListForMux.end(),tempMRegID) == eMRegIDListForMux.end() ) { 
					eMRegIDListForMux.push_back( tempMRegID ); 
					rsMuxIDListForMux.push_back( currMuxNum ); 
					++currMuxNum; 
				}
			} 
			++iterEList; 
		} 
		++iterGList; 
	} 
	// assign FW edge to Mux 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			tempEType = iterEList->get_eType(); 
			if ( tempEType == FW ) { 
				tempMRegID = convertRegIDToMRegID( *iterEList ); 
				currMuxNum = findTheMappingRSMuxIDByMRegID( eMRegIDListForMux, rsMuxIDListForMux, tempMRegID ); 
				tempDFRSid.set_dfrsType( DFRS_MUX ); 
				tempDFRSid.set_dfrsID( currMuxNum ); 
				tempDFRSid.preset_dfrsName(); 
				if ( !aRTLStruc.searchDFRS( tempDFRSid ) ) {	// if not find 
					tempDataFlowRS.set_theDFRSid( tempDFRSid ); 
					tempDataFlowRS.set_muxConfType( MUX_C ); 
					tempDataFlowRS.set_outputNum( 1 ); 
					tempDataFlowRS.set_rsPartiID( iterEList->get_partiID() );		// -- 2005.11.21 added the rsPartiID -- 
					tempDataFlowRS.pushBack_neIDList( iterEList->get_eID() ); 
					// ctrl line ( mux_XX_sel ) 
					_itoa( currMuxNum, c_strNum, 10 ); 
					strNum = c_strNum;
					tempStrName = CtrlTypeNameBeg[DFRS_MUX] + "_" + strNum + "_" + CtrlTypeNameEnd[DFRS_MUX]; 
					tempCtrlSignal.set_ctrlType( DFRS_MUX ); 
					tempCtrlSignal.set_ctrlName( tempStrName ); 
					tempCtrlSignal.set_muxConfType( MUX_C ); 
					tempDataFlowRS.set_aCtrlSignal( tempCtrlSignal );  
					aRTLStruc.pushBack_aCtrlSignalList( tempCtrlSignal );	// add MUX's CS into aRTLStruc ! 
					tempCtrlSignal.clear_CtrlSignal(); 
					// input line ( unknow ) 未?! 
					// output line ( mux_XX_out ) 
					_itoa( currMuxNum, c_strNum, 10 ); 
					strNum = c_strNum; 
					tempStrName = ILLineNameBeg[DFRS_MUX] + "_" + strNum + "_" + ILLineNameEnd; 
					tempInterLine.set_ilID( currRegNum );  
					tempInterLine.set_ilName( tempStrName ); 
					tempInterLine.set_ilType( IL_MUXOUT_TO_REG );  
					tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					aRTLStruc.pushBack_aInterLineList( tempInterLine );	// add IL into aRTLStruc ! 
					tempInterLine.clear_InterLine(); 
					aRTLStruc.pushBack_aMuxList( tempDataFlowRS );	// add MUX into aRTLStruc ! 
					tempDataFlowRS.clear_DataFlowRS(); 
				} 
				else {	// if find 
					tempDataFlowRS = aRTLStruc.findAndAccessDFRS( tempDFRSid ); 
					tempInterLine = tempDataFlowRS.get_outputInterLine(); 
					tempInterLine.pushBack_eIDList( iterEList->get_eID() ); 
					tempDataFlowRS.set_outputInterLine( tempInterLine ); 
					tempDataFlowRS.pushBack_neIDList( iterEList->get_eID() ); 
					aRTLStruc.findAndUpdataIL( tempInterLine ); 	// updata IL of aRTLStruc ! 
					tempInterLine.clear_InterLine(); 
					aRTLStruc.findAndUpdataDFRS( tempDataFlowRS );	// updata REG of aRTLStruc ! 
					tempDataFlowRS.clear_DataFlowRS();
				} 
			} 
			++iterEList; 
		} 
		++iterGList; 
	} 
//cout << aRTLStruc << endl; 
	// 7. Integrate info. to construct detail InterLine between each other (在這之前所有IL和CS都已命名好, 只剩詳細連結和刪除多餘的 mux) 
	// construct detail InterLine between each other 
	aRTLStruc.detailConnectingFuRegMux(); 
//fout << aRTLStruc << endl; 
	// delete the redundant MUX & re-assign MUX id and do some necessary IL CS DFRS modification ( 暫時未做後面項 !! ) 
	aRTLStruc.deleteRedundantMux();	
	// set the final DFRS number 
	aRTLStruc.set_numOfFU(); 
	aRTLStruc.set_numOfMux(); 
	aRTLStruc.set_numOfReg(); 
//cout << aRTLStruc << endl; 
	// 8. Integrate info. to construct detail control signals for MUX and REG 
	aRTLStruc.detailConstructCtrlSignal( aDFGs ); 
//cout << aRTLStruc << endl; 
	// 9. write out the whole structure into the file 
	iterILList = aRTLStruc.aInterLineList.begin(); 
	while ( iterILList != aRTLStruc.aInterLineList.end() ) { 
		if ( iterILList->get_ilType() == IL_INPUT || iterILList->get_ilType() == IL_OUTPUT ) { 
			fout << *iterILList; 
		} 
		++iterILList; 
	} 
	fout << aRTLStruc << endl;  
	// Close the file 
	fout << endl; 
	fout.clear(); 
	fout.close(); 
	// --------------------------------------------------------------------------------------------------------
	fout.open( OUTPUT_SOCE_GROUP_CMD.c_str() );
	fout << "---- for SOCE group command ----\n";
	// 10. Write the SOCE group command into the file				// -- 2005.11.21 added the rsPartiID --  
	string						cmd_create = "createInstGroup";	// SOCE group command 
	string						cmd_add = "addInstToInstGroup";	// SOCE group command 
	string						groupName = "group_";			// ex: group_0 
	string::size_type			idx_groupName; 
	string						topModuleName;					// ex: fir7_all
	string						firComputingModuleName = "m0";	// ex: m0
	string						tempRSName;						// ex: MUL0
	string						wholeModuleName;				// ex: fir7_all/m0/MUL0 
	int							groupID;						// the same as the rsPartiID 
	topModuleName = baseName + "_" + extName; 
	groupID = 1;	// partiID start as 1 
	for ( int p = 0; p < aDFGs.get_clusNumConstraint(); p++ ) { 
		// ---- updata the group name ---- 
		idx_groupName = groupName.find( '_' ); 
		++idx_groupName;
		groupName = groupName.substr( 0, idx_groupName );
		_itoa( groupID, c_strNum, 10 ); 
		groupName = groupName + c_strNum; 
		// ------------------------------- 
		++groupID;
		fout << cmd_create << " " << groupName << endl;		// write out 
	}
	iterDataFlowRS = aRTLStruc.aFUList.begin(); 
	while ( iterDataFlowRS != aRTLStruc.aFUList.end() ) { 
		groupID = iterDataFlowRS->get_rsPartiID(); 
		// ---- updata the group name ---- 
		idx_groupName = groupName.find( '_' ); 
		++idx_groupName;
		groupName = groupName.substr( 0, idx_groupName );
		_itoa( groupID, c_strNum, 10 ); 
		groupName = groupName + c_strNum; 
		// -------------------------------
		tempRSName = iterDataFlowRS->get_theDFRSid().get_dfrsName(); 
		wholeModuleName = topModuleName + "/" + firComputingModuleName + "/" + tempRSName;
		fout << cmd_add << " " << groupName << " " << wholeModuleName << endl;	// write out
		++iterDataFlowRS; 
	} 
	iterDataFlowRS = aRTLStruc.aRegList.begin(); 
	while ( iterDataFlowRS != aRTLStruc.aRegList.end() ) { 
		groupID = iterDataFlowRS->get_rsPartiID(); 
		// ---- updata the group name ---- 
		idx_groupName = groupName.find( '_' ); 
		++idx_groupName;
		groupName = groupName.substr( 0, idx_groupName );
		_itoa( groupID, c_strNum, 10 ); 
		groupName = groupName + c_strNum; 
		// -------------------------------
		tempRSName = iterDataFlowRS->get_theDFRSid().get_dfrsName(); 
		wholeModuleName = topModuleName + "/" + firComputingModuleName + "/" + tempRSName;
		fout << cmd_add << " " << groupName << " " << wholeModuleName << endl;	// write out
		++iterDataFlowRS; 
	} 
	// ( PS. the SOCE do not take MUX as HInst, so mux needn't print out !! ) 
	iterDataFlowRS = aRTLStruc.aMuxList.begin(); 
	while ( iterDataFlowRS != aRTLStruc.aMuxList.end() ) { 
		groupID = iterDataFlowRS->get_rsPartiID(); 
		// ---- updata the group name ---- 
		idx_groupName = groupName.find( '_' ); 
		++idx_groupName;
		groupName = groupName.substr( 0, idx_groupName );
		_itoa( groupID, c_strNum, 10 ); 
		groupName = groupName + c_strNum; 
		// -------------------------------
		tempRSName = iterDataFlowRS->get_theDFRSid().get_dfrsName(); 
		wholeModuleName = topModuleName + "/" + firComputingModuleName + "/" + tempRSName;
		fout << cmd_add << " " << groupName << " " << wholeModuleName << endl;	// write out
		++iterDataFlowRS; 
	} 
	// Close the file 
	fout << endl; 
	fout.clear(); 
	fout.close(); 
	// --------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------------------
	fout.open( OUTPUT_SOCE_NETLIST_CAP.c_str() );
	fout << "---- for edge cut or local/global wire list ----\n"; 
	// 11. Write the local and global wire list 					// -- 2005.11.21 added the edge cut mark -- 
	list<Edge>						cutEdgeList; 
	list<int>::iterator				iterEIDList; 
	list<InterLine>					inout_ILList; 
	list<InterLine>					muxToFu_ILList; 
	list<InterLine>					muxToReg_ILList; 
	list<InterLine>					fuToMux_ILList; 
	list<InterLine>					regToMux_ILList; 
	ILType							tempILType; 
	list<InterLine>::iterator		iterILList_temp; 
	// get the cut edge list from aDFGs 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		iterEList = iterGList->eList.begin(); 
		while ( iterEList != iterGList->eList.end() ) { 
			if ( iterEList->get_isCut() ) { 
				cutEdgeList.push_back( *iterEList ); 
			} 
			++iterEList;
		} 
		++iterGList; 
	} 
//copy(cutEdgeList.begin(), cutEdgeList.end(), ostream_iterator<Edge>(cout,"")); cout << endl; 
	// divide ILList into five types ILlist 
	tempILType = IL_NONE; 
	iterILList = aRTLStruc.aInterLineList.begin(); 
	while ( iterILList != aRTLStruc.aInterLineList.end() ) { 
		tempILType = iterILList->get_ilType(); 
		if ( tempILType == IL_INPUT || tempILType == IL_OUTPUT ) { 
			inout_ILList.push_back( *iterILList ); 
		} 
		else if ( tempILType == IL_MUXOUT_TO_FU ) { 
			muxToFu_ILList.push_back( *iterILList );
		} 
		else if ( tempILType == IL_MUXOUT_TO_REG ) { 
			muxToReg_ILList.push_back( *iterILList );
		} 
		else if ( tempILType == IL_FUOUT_TO_MUX ) { 
			fuToMux_ILList.push_back( *iterILList );
		} 
		else if ( tempILType == IL_REGOUT_TO_MUX ) { 
			regToMux_ILList.push_back( *iterILList );
		} 
		else { 
			cout << "Can't find this ILType ( in opf_DFGsToVHDLGen() ) !! \n"; 
			system("pause"); 
			exit(0); 
		} 
		++iterILList; 
	} 
//copy(inout_ILList.begin(), inout_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
//copy(muxToFu_ILList.begin(), muxToFu_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
//copy(muxToReg_ILList.begin(), muxToReg_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
//copy(fuToMux_ILList.begin(), fuToMux_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
//copy(regToMux_ILList.begin(), regToMux_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
	// set the edge cut of the IL 
	iterILList = fuToMux_ILList.begin(); 
	while ( iterILList != fuToMux_ILList.end() ) { 
		iterEIDList = iterILList->eIDList.begin(); 
		while ( iterEIDList != iterILList->eIDList.end() ) { 
			iterEList = cutEdgeList.begin(); 
			while ( iterEList != cutEdgeList.end() ) { 
				if ( iterEList->get_eID() == *iterEIDList ) { 
					iterILList->set_isCut( true ); 
					iterEIDList = iterILList->eIDList.end(); 
					--iterEIDList; 
					iterEList = cutEdgeList.end(); 
				} 
				else { 
					++iterEList; 
				}
			} 	
			++iterEIDList; 
		}
		++iterILList; 
	}
//copy(fuToMux_ILList.begin(), fuToMux_ILList.end(), ostream_iterator<InterLine>(cout,"")); cout << "---------\n"; 
	// updata the edge cut IL of aRTLStruc.aInterLineList
	iterILList_temp = fuToMux_ILList.begin();
	while ( iterILList_temp != fuToMux_ILList.end() ) { 
		iterILList = aRTLStruc.aInterLineList.begin(); 
		while ( iterILList != aRTLStruc.aInterLineList.end() ) { 
			if ( iterILList_temp->get_ilName() == iterILList->get_ilName() ) { 
				if ( iterILList_temp->get_isCut() ) { 
					iterILList->set_isCut( true ); 
				} 
				else { 
					iterILList->set_isCut( false ); 
				}
				iterILList = aRTLStruc.aInterLineList.end(); 
			} 
			else { 
				++iterILList; 
			}
		} 
		++iterILList_temp; 
	}
//copy(aRTLStruc.aInterLineList.begin(), aRTLStruc.aInterLineList.end(), ostream_iterator<InterLine>(cout,"")); cout << endl; 
	// print out the global and local wire 
	fout << "## GLOBAL_WIRE : " << endl; 
	iterILList = fuToMux_ILList.begin(); 
	while ( iterILList != fuToMux_ILList.end() ) { 
		if ( iterILList->get_isCut() ) { 
			fout << iterILList->get_ilName() << endl; 
		} 
		++iterILList; 
	} 
	fout << "## END " << endl << endl;
	fout << "## LOCAL_WIRE : " << endl;
	iterILList = fuToMux_ILList.begin(); 
	while ( iterILList != fuToMux_ILList.end() ) { 
		if ( !iterILList->get_isCut() ) { 
			fout << iterILList->get_ilName() << endl; 
		} 
		++iterILList; 
	} 
	iterILList = muxToFu_ILList.begin(); 
	while ( iterILList != muxToFu_ILList.end() ) { 
		fout << iterILList->get_ilName() << endl; 
		++iterILList; 
	} 
	iterILList = muxToReg_ILList.begin(); 
	while ( iterILList != muxToReg_ILList.end() ) { 
		fout << iterILList->get_ilName() << endl; 
		++iterILList; 
	} 
	iterILList = regToMux_ILList.begin(); 
	while ( iterILList != regToMux_ILList.end() ) { 
		fout << iterILList->get_ilName() << endl; 
		++iterILList; 
	} 
	fout << "## END " << endl << endl; 
	fout << "## INOUT_WIRE : " << endl; 
	iterILList = inout_ILList.begin(); 
	while ( iterILList != inout_ILList.end() ) { 
		fout << iterILList->get_ilNote() << endl; 
		++iterILList; 
	} 
	fout << "## END " << endl << endl; 
	// Close the file 
	fout << endl; 
	fout.clear(); 
	fout.close(); 
	// --------------------------------------------------------------------------------------------------------
} 
*/
string& findTheMappingFUNameByOPAssID( vector<string>& aFuNameVect, list<int>& mappingOpAssIDList, int opAssID ) { 
	list<int>::iterator				iterOpAssIDList; 
	int								idx; 
	idx = 0; 
	iterOpAssIDList = mappingOpAssIDList.begin(); 
	while ( iterOpAssIDList != mappingOpAssIDList.end() ) { 
		if ( *iterOpAssIDList == opAssID ) { 
			iterOpAssIDList = mappingOpAssIDList.end(); 
		} 
		else { 
			++idx; 
			++iterOpAssIDList; 
		} 
	} 
	return aFuNameVect[idx]; 
} 

int findTheMappingFUNumByOPAssID( list<int>& opAssIDList, list<int>& fuAssIDList, int aOPAssID ) { 
	list<int>::iterator		iterOpAIDList; 
	list<int>::iterator		iterFuAIDList; 
	iterOpAIDList = opAssIDList.begin(); 
	iterFuAIDList = fuAssIDList.begin(); 
	while ( iterOpAIDList != opAssIDList.end() ) { 
		if ( *iterOpAIDList == aOPAssID ) { 
			return *iterFuAIDList;
		} 
		else { 
			++iterFuAIDList; 
			++iterOpAIDList; 
		} 
	} 
	cout << "Warning ! Can't find the findTheMappingFUNum() result !! \n"; 
	return -1; 
} 

int convertRegIDToMRegID( Edge& aEdge ) { 
	return ( aEdge.get_partiID() * 2 + aEdge.get_regID() ); 
} 

int findTheMappingRSRegIDByMRegID( list<int>& eMRegIDList, list<int>& rsRegIDList, int aMRegID ) { 
	list<int>::iterator		iterEMRegIDList; 
	list<int>::iterator		iterRSRegIDList; 
	iterEMRegIDList = eMRegIDList.begin(); 
	iterRSRegIDList = rsRegIDList.begin(); 
	while ( iterEMRegIDList != eMRegIDList.end() ) { 
		if ( *iterEMRegIDList == aMRegID ) { 
			return *iterRSRegIDList;
		} 
		else { 
			++iterEMRegIDList; 
			++iterRSRegIDList; 
		} 
	} 
	cout << "Warning ! Can't find the findTheMappingRSRegIDByMRegID() result !! \n"; 
	return -1; 
} 

int findTheMappingRSMuxIDByMRegID( list<int>& eMRegIDListForMux, list<int>& rsMuxIDListForMux, int aMRegID ) { 
	list<int>::iterator		iterEMRegIDListForMux; 
	list<int>::iterator		iterRSMuxIDListForMux; 
	iterEMRegIDListForMux = eMRegIDListForMux.begin(); 
	iterRSMuxIDListForMux = rsMuxIDListForMux.begin(); 
	while ( iterEMRegIDListForMux != eMRegIDListForMux.end() ) { 
		if ( *iterEMRegIDListForMux == aMRegID ) { 
			return *iterRSMuxIDListForMux;
		} 
		else { 
			++iterEMRegIDListForMux; 
			++iterRSMuxIDListForMux; 
		} 
	} 
	cout << "Warning ! Can't find the findTheMappingRSMuxIDByMRegID() result !! \n"; 
	return -1; 
} 
bool checkTheSameIDList( list<int>& aIntList, list<int>& bIntList ) { 
	list<int>::iterator			iterIntList_a; 
	list<int>::iterator			iterIntList_b; 
	if ( aIntList.size() == bIntList.size() ) { 
		iterIntList_a = aIntList.begin(); 
		iterIntList_b = bIntList.begin(); 
		while ( iterIntList_a != aIntList.end() ) { 
			if ( *iterIntList_a != *iterIntList_b ) { 
				return false; 
			} 
			++iterIntList_a; 
			++iterIntList_b; 
		} 
		return true; 
	} 
	else { 
		return false; 
	} 
} 
int searchPartiIDBySuccessorOfEdge( DFGs& aDFGs, Edge& tempEdge ) { 
	list<Graph>::iterator			iterGList; 
	list<Node>::iterator			iterNList; 
	int								tempPartiID; 
	tempPartiID = -1; 
	iterGList = aDFGs.gList.begin(); 
	while ( iterGList != aDFGs.gList.end() ) { 
		if ( tempEdge.get_gID() == iterGList->get_gID() ) {
			iterNList = iterGList->nList.begin(); 
			while ( iterNList != iterGList->nList.end() ) { 
				if ( iterNList->get_nID() == tempEdge.get_to_nID() ) { 
					tempPartiID = iterNList->get_partiID(); 
					iterNList = iterGList->nList.end(); 
					iterGList = aDFGs.gList.end(); 
				} 
				else {
					++iterNList; 
				}
			}
		} 
		else {
			++iterGList; 
		}
	} 
	return tempPartiID; 
} 
// --------------------------------------------------------------------


// --------------------------------------------------------------------
// Other useful functions in the fileio

// ** ASCII_number_string_to_INT_number 
// (with removing whitespace in the head/tail)
template <class Any>
Any myPow ( const Any& base, int ext ) {
	Any		value = base;
	if ( ext == 0) {
		return 1;
	}
	else {
		for ( int i=1; i<ext; ++i) {
			value *= base;
		}
		return value;
	}
}
// 
int asciiToDigit( const char  d ) { 
	int		value = 0;
	// 
	switch ( d ) {
		case '0': value = 0; break;
		case '1': value = 1; break;
		case '2': value = 2; break;
		case '3': value = 3; break;
		case '4': value = 4; break;
		case '5': value = 5; break;
		case '6': value = 6; break;
		case '7': value = 7; break;
		case '8': value = 8; break;
		case '9': value = 9; break;
		default: cout << "Can't convert !!" << endl;
	}
	return value;
}
// 
int stringNumberToIntNumber( const char* str ) {
	int			value = 0;
	int			tempValue = 0;
	string			tempStr;
	string::size_type	index;
	tempStr = str;
	// check and erase whilespace in the head/tail
	while( tempStr[0] == ' ' ) {
		index = 0;
		tempStr.erase( index, 1 );
	}
	while( tempStr[tempStr.size()-1] == ' ' ) {
		index = tempStr.size()-1;
		tempStr.erase( index, 1 );
	}// end of check and erase whilespace in the head/tail
	for( int i = tempStr.size(); i > 0; --i ) {
		tempValue = asciiToDigit( tempStr[(tempStr.size()-i)] ) * myPow( 10, (i-1) );
		value = value + tempValue;
	}
	return value;
}// End of ASCII_number_string_to_INT_number


// ** Remove whitespace at head/tail of a string
// sysopt_(Step 3) 
void rmWhitespace( string& str ) {
	string::size_type	index;
	// check and erase whilespace in the head/tail
	while( str[0] == ' ' ) {
		index = 0;
		str.erase( index, 1 );
	}
	while( str[str.size()-1] == ' ' ) {
		index = str.size()-1;
		str.erase( index, 1 );
	}
}// End of Remove whitespace at head/tail of a string


// ** Remove sharp(#) comment
// sysopt_(Step 4)
void rmSharpComment( string& tempStr ) {
	string::size_type	idx;
	// 
	idx = tempStr.find_first_of("#");
	if ( idx != string::npos ) {
		tempStr.erase(idx);
	}
}// End of Remove sharp(#) comment


// ** Extract a number string from the input string
string extractANumStr( string& str, bool& conti ) {
	string			tempNumStr;
	string::size_type	begIdx, endIdx;
	string::size_type	chkIdx;		// for check any word needs to extract 
	// 
	begIdx = str.find_first_not_of(" \t");
	endIdx = str.find_first_not_of("0123456789", begIdx);
	if ( begIdx != string::npos || endIdx != string::npos ) {
		tempNumStr.assign(str, begIdx, (endIdx-begIdx));
		str.replace(begIdx, (endIdx-begIdx), " ");
		chkIdx = str.find_first_of("0123456789", begIdx);
		if ( chkIdx != string::npos ) {
			conti = true;
		}
		else {
			conti = false;
		}
	} 
	else {
		conti = false;
	}
	return tempNumStr;
}

// End of Extract a number string from the input string
// -------------------------------------------------------------------- 

