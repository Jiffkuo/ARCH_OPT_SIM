// ********************************************************************
// SharedMemory.h
// Jiff (Tzu-Chi, Kuo) by 20070330
// jiffkuo@gmail.com
// Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
// ********************************************************************
#ifndef _SharedMemory_h_
#define _SharedMemory_h_

enum MemoryItem {
	NITEM_SHAREDMEMORY_ID,
	NITEM_SHAREDMEMORY_LATENCY,
	NITEM_DEDICATEDBUS_NUM,
	NITEM_DEDICATEDBUS
};

class DedicatedBus;
class SharedMemory;
class SharedMemorySystem;

class DedicatedBus
{
	private:
		int dID;
		int SharedMemoryID;
		
	public:
		// constructors
		DedicatedBus(): dID(-1), SharedMemoryID(-1) {}
		DedicatedBus(int did, int smid) { dID=did; SharedMemoryID=smid; }
		// destructor
		~DedicatedBus() {}
		// member functions
		inline void set_dID(int did) { dID = did; }
		inline int get_dID() { return dID; }
		inline void set_SharedMemoryID(int smid) { SharedMemoryID = smid; }
		inline int get_SharedMemoryID() { return SharedMemoryID; }
		friend ostream& operator<<( ostream& os, const DedicatedBus& db ) 
		{
			os <<"SharedMemory ID: "<<db.SharedMemoryID<<", DedicatedBus ID: "<<db.dID<<endl;
			return os;
		}
};

class SharedMemory
{
	private:
		int SharedMemoryID;
		int SharedMemoryLatency;
		int numOfDedicatedBus;
		bool write_flag;
	public:
		list<DedicatedBus> dList;
		// constructor
		SharedMemory(): SharedMemoryID(-1), numOfDedicatedBus(-1) {}
		SharedMemory(int smid, int nd) { SharedMemoryID=smid; numOfDedicatedBus=nd; }
		// destructor
		~SharedMemory() {}
		// member functions
		       void set_SharedMemoryID (int smid);
		inline int get_SharedMemoryID() { return SharedMemoryID; }
		inline void set_SharedMemoryLatency (int sml) { SharedMemoryLatency = sml; }
		inline int get_SharedMemoryLatency() { return SharedMemoryLatency; }
		inline void set_numOfDedicatedBus (int nd) { numOfDedicatedBus = nd; }
		inline void set_numOfDedicatedBus() { numOfDedicatedBus = dList.size(); }
		inline int get_numOfDedicatedBus() { return numOfDedicatedBus; }
		inline void set_write_flag (bool wf) { write_flag = wf; }
		inline bool get_write_flag() { return write_flag; }
		
		friend ostream& operator<<( ostream& os, const SharedMemory& SMemory ) 
		{
		 	os << "\nSharedMemoryID = " << SMemory.SharedMemoryID ;
		 	os << "\nSharedMemoryLatency = " << SMemory.SharedMemoryLatency ;
		 	if(SMemory.write_flag)
		 		os << "; numOfDedicatedBus(writing) = " << SMemory.numOfDedicatedBus;
		 	else
		 		os << "; numOfDedicatedBus(reading) = " << SMemory.numOfDedicatedBus;
		 	os << "\n----------\n";
		 	os << "connected DedicatedBus : " << "\n";
		 	copy( SMemory.dList.begin(), SMemory.dList.end(), ostream_iterator<DedicatedBus>(os,"") );
		 	os << "==============================\n\n";
		 	return os;
	 	}
};

class SharedMemorySystem
{
	private:
		int numOfSharedMemory;
		bool write_flag;
	
	public:
		list<SharedMemory> smList;
		// constructor
		SharedMemorySystem(): numOfSharedMemory(-1) {}
		SharedMemorySystem(int nsm) { numOfSharedMemory=nsm; }
		// destructor
		~SharedMemorySystem() {}
		// member fuctions
		inline void set_numOfSharedMemory(int nsm) { numOfSharedMemory = nsm; }
		inline void set_numOfSharedMemory() { numOfSharedMemory = smList.size(); }
		inline int get_numOfSharedMemory() { return numOfSharedMemory; }
		bool findDedicatedBusInSharedMemoryID( int SharedMemoryID, int dID );
		int getDedicatedBusSharedMemoryID( int dID );
		bool findDedicatedBusInSharedMemory( int dID );
		inline bool get_write_flag() { return write_flag; }
		void set_write_flag (bool wf);
		int getLatencyBySMid (int smid);
		int getSMidByDBid (int dbid);
		friend ostream& operator<<( ostream& os, const SharedMemorySystem& SMSystem ) 
		{
		 	os << "SharedMemorySystem : ";
		 	if(SMSystem.write_flag)
		 		os << "write\n";
		 	else
		 		os << "read\n";
		 	os << "numOfSharedMemory : " << SMSystem.numOfSharedMemory;
		 	os << "\n----------\n";
		 	copy( SMSystem.smList.begin(), SMSystem.smList.end(), ostream_iterator<SharedMemory>(os,"") );
		 	os << "\n======= End of SharedMemorySystem =======\n\n\n";
		 	return os;
	 	}
};

void SharedMemorySystem::set_write_flag (bool wf)
{
	write_flag = wf;
	list<SharedMemory>::iterator posSharedMemory;   
	posSharedMemory = smList.begin();        
	while ( posSharedMemory != smList.end() )
	{
		posSharedMemory->set_write_flag(wf);       
		++posSharedMemory;                  
	}
}

void SharedMemory::set_SharedMemoryID(int smid)
{
	SharedMemoryID = smid;
	list<DedicatedBus>::iterator posDedicatedBus;   
	posDedicatedBus = dList.begin();        
	while ( posDedicatedBus != dList.end() )
	{
		posDedicatedBus->set_SharedMemoryID(smid);       
		++posDedicatedBus;                  
	}
}
		
int SharedMemorySystem::getDedicatedBusSharedMemoryID( int dID )
{
	list<SharedMemory>::iterator posSMList;
	list<DedicatedBus>::iterator posDList;
	posSMList = smList.begin();
	while ( posSMList != smList.end() ) 
	{
		posDList = posSMList->dList.begin();
		while ( posDList != posSMList->dList.end() ) 
		{
			if ( posDList->get_dID() == dID ) 
			{
				return	posDList->get_SharedMemoryID();
			}
			else
				++posDList;
		}
		++posSMList;
	}
	cout << "Error !! This dedicated bus is not connected to any shared memory !! " << endl << "Exit program !! "; 
	system("pause"); 
	exit(0); 
}

bool SharedMemorySystem::findDedicatedBusInSharedMemoryID( int SharedMemoryID, int dID ) 
{ 
	list<SharedMemory>::iterator posSMList;
	list<DedicatedBus>::iterator posDList;
	posSMList = smList.begin();
	while ( posSMList != smList.end() ) 
	{
		if ( posSMList->get_SharedMemoryID() == SharedMemoryID ) 
		{
			posDList = posSMList->dList.begin();
			while ( posDList != posSMList->dList.end() ) 
			{
				if ( posDList->get_dID() == dID ) 
				{
					return true;
				}
				else 
				{
					++posDList;
				}
			}
		}
		++posSMList;
	}
	return false;
} 

bool SharedMemorySystem::findDedicatedBusInSharedMemory( int dID )
{
	list<SharedMemory>::iterator posSMList;
	list<DedicatedBus>::iterator posDList;
	posSMList = smList.begin();
	while ( posSMList != smList.end() ) 
	{
		posDList = posSMList->dList.begin();
		while ( posDList != posSMList->dList.end() ) 
		{
			if ( posDList->get_dID() == dID )
				return true;
			else
				++posDList;
		}
		++posSMList;
	}
	return false;
}

int SharedMemorySystem::getSMidByDBid (int dID)
{
	list<SharedMemory>::iterator posSMList;
	list<DedicatedBus>::iterator posDList;
	posSMList = smList.begin();
	while ( posSMList != smList.end() ) 
	{
		posDList = posSMList->dList.begin();
		while ( posDList != posSMList->dList.end() ) 
		{
			if ( posDList->get_dID() == dID )
				return posSMList->get_SharedMemoryID();
			else
				++posDList;
		}
		++posSMList;
	}
	return 0;
}
	
	
int SharedMemorySystem::getLatencyBySMid (int smid)
{
	list<SharedMemory>::iterator posSMList;
	posSMList = smList.begin();
	while ( posSMList != smList.end() ) 
	{
		if (posSMList->get_SharedMemoryID() == smid)
			return posSMList->get_SharedMemoryLatency();
		else
			++posSMList;
	}
	return 0;
}


#endif	// End of _SharedMemory_h_