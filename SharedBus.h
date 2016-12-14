// ********************************************************************
// SharedBus.h
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************
#ifndef _SharedBus_h_
#define _SharedBus_h_

//跟讀入的sharedbue.cfg格式相同
enum BusItem {
	NITEM_SHAREDBUS_ID,
	NITEM_BURSTSIZE,
	NITEM_BANDWIDTH,
	NITEM_COMPONENT_NUM,
	NITEM_COMPONENT
};

class Component;
class SharedBus;
class SharedBusSystem;

class Component
{
	private:
		int cID;
		int SharedBusID;
		
	public:
		// constructors
		Component(): cID(-1), SharedBusID(-1) {}                    //一般建構子,設定初始值
		Component(int cid, int sbid) { cID=cid; SharedBusID=sbid; } 
		// destructor
		~Component() {}
		// member functions
		inline void set_cID(int cid) { cID = cid; }
		inline int get_cID() { return cID; }
		inline void set_SharedBusID(int sbid) { SharedBusID = sbid; }
		inline int get_SharedBusID() { return SharedBusID; }
		friend ostream& operator<<( ostream& os, const Component& comp ) 
		{
			os <<"SharedBus ID: "<<comp.SharedBusID<<", Component ID: "<<comp.cID<<endl;
			return os;
		}
};

class SharedBus
{
	private:
		int SharedBusID;
		int numOfComponent;
		int BurstSize;
		int BandWidth;		
		
	public:
		list<Component> cList;         //宣告儲存Component用的list
		// constructor
		SharedBus(): SharedBusID(-1), numOfComponent(-1) {}
		SharedBus(int sbid, int nc) { SharedBusID=sbid; numOfComponent=nc; }
		// destructor
		~SharedBus() {}
		// member functions
		inline void set_SharedBusId(int sID) { SharedBusID = sID; }
		inline int get_SharedBusID() { return SharedBusID; }
		inline void set_BurstSize(int bs) { BurstSize = bs; }
		inline int get_BurstSize() { return BurstSize; }
		inline void set_BandWidth(int bw) { BandWidth = bw; }
		inline int get_BandWidth() { return BandWidth; }
		inline void set_numOfComponent(int nc) { numOfComponent = nc; }
		inline void set_numOfComponent() { numOfComponent = cList.size(); }
		inline int get_numOfComponent() { return numOfComponent; }
		void set_SharedBusID(int sbid);
		friend ostream& operator<<( ostream& os, const SharedBus& SBus ) 
		{
		 	os << "\nSharedBusID = " << SBus.SharedBusID << "; numOfComponent = " << SBus.numOfComponent 
			   << "; BurstSize = " << SBus.BurstSize << "; BandWidth = "<< SBus.BandWidth;
		 	os << "\n----------\n";
		 	os << "connected components : " << "\n";
		 	copy( SBus.cList.begin(), SBus.cList.end(), ostream_iterator<Component>(os,"") );  //把cList.begin到cList.end之間的element複製到ostream_iterator<Component>(os,""),
		 	os << "==============================\n\n";                                        //我想此行的目的純粹想要看有沒有Bus上面的component全都有讀到
		 	return os;
	 	}
};

class SharedBusSystem
{
	private:
		int numOfSharedBus;
	
	public:
		list<SharedBus> sbList;
		// constructor
		SharedBusSystem(): numOfSharedBus(-1) {}
		SharedBusSystem(int nsb) { numOfSharedBus=nsb; }
		// destructor
		~SharedBusSystem() {}
		// member fuctions
		inline void set_numOfSharedBus(int nsb) { numOfSharedBus = nsb; }
		inline void set_numOfSharedBus() { numOfSharedBus = sbList.size(); }
		inline int get_numOfSharedBus() { return numOfSharedBus; }
		bool findComponentInSharedBus( int SharedBusID, int cID );
		int getComponentSharedBusID( int cID );
		int get_ComponentSBbusBurst( int cID );
		int get_SBbusBandWidth( int sbID );
		friend ostream& operator<<( ostream& os, const SharedBusSystem& SBSystem ) 
		{
		 	os << "SharedBusSystem : \n";
		 	os << "numOfSharedBus : " << SBSystem.numOfSharedBus;
		 	os << "\n----------\n";
		 	copy( SBSystem.sbList.begin(), SBSystem.sbList.end(), ostream_iterator<SharedBus>(os,"") );//同上,看BusSystem上面的Bus有沒有全部讀到
		 	os << "\n======= End of SharedBusSystem =======\n\n\n";
		 	return os;
	 	}
};


void SharedBus::set_SharedBusID(int sbid)
{
	SharedBusID = sbid;
	list<Component>::iterator posComponent;   
	posComponent = cList.begin();        
	while ( posComponent != cList.end() )
	{
		posComponent->set_SharedBusID(sbid);       
		++posComponent;                  
	}
}

//我覺得是為了能讓SharedBusSystem上的Component能夠跟在SharedBus上的component相結合,才能代表同一樣東西
//或是已componentID為根據,來找出SharedBusID, line 138:return posCList->get_SharedBusID();
int SharedBusSystem::getComponentSharedBusID( int cID )
{
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	posSBList = sbList.begin();
	while ( posSBList != sbList.end() ) 
	{
		posCList = posSBList->cList.begin();    //讓在System的<component> posCList能指向SharedBus中list<component>的cList
		while ( posCList != posSBList->cList.end() ) 
		{
			if ( posCList->get_cID() == cID ) 
			{
				return	posCList->get_SharedBusID();
			}
			else
				++posCList;
		}
		++posSBList;
	}
	cout << "Error !! This component is not connected to any shared bus !! " << endl << "Exit program !! "; 
	system("pause"); 
	exit(0); 
}

int SharedBusSystem::get_ComponentSBbusBurst( int cID )
{
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	posSBList = sbList.begin();
	while ( posSBList != sbList.end() ) 
	{
		posCList = posSBList->cList.begin();
		while ( posCList != posSBList->cList.end() ) 
		{
			if ( posCList->get_cID() == cID ) 
			{
				return posSBList->get_BurstSize();
			}
			else
				++posCList;
		}
		++posSBList;
	}
	cout << "Error !! This component is not connected to any shared bus !! No BurstSize is found. " << endl << "Exit program !! "; 
	system("pause"); 
	exit(0); 
}

int SharedBusSystem::get_SBbusBandWidth( int sbID )
{
	list<SharedBus>::iterator posSBList;
	posSBList = sbList.begin();
	while ( posSBList != sbList.end() ) 
	{
		if ( posSBList->get_SharedBusID() == sbID ) 
			return posSBList->get_BandWidth();
		else
			++posSBList;
	}
	cout << "Error !! This shared bus is not in the BusSystem !! No Bus BandWidth is found. " << endl << "Exit program !! "; 
	system("pause"); 
	exit(0); 
}

bool SharedBusSystem::findComponentInSharedBus( int SharedBusID, int cID ) 
{ 
	list<SharedBus>::iterator posSBList;
	list<Component>::iterator posCList;
	posSBList = sbList.begin();
	while ( posSBList != sbList.end() ) 
	{
		if ( posSBList->get_SharedBusID() == SharedBusID ) 
		{
			posCList = posSBList->cList.begin();
			while ( posCList != posSBList->cList.end() ) 
			{
				if ( posCList->get_cID() == cID ) 
				{
					return true;
				}
				else 
				{
					++posCList;
				}
			}
		}
		++posSBList;
	}
	return false;
} 
#endif	// End of _SharedBus_h_