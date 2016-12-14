/* File name : Analysis.h
   Data      : 2007.04.21
   Author    : Jiff (Tzu-Chi, Kuo)
   Mail      : jiffkuo@gmail.com
   Lab       : Low Power High Performance Lab
   Copyright (C) 2007, Jiff (Tzu-Chi, Kuo)
   Decription: For store the information about each component includes how many times Read and Write
               Total In/Out TC and Cycles in each componenet in the whole system
               
*/
#ifndef _Analysis_h_
#define _Analysis_h_

class Parameter;
class ComponentOnSharedBus;
class AnalyzeSystem;

class Parameter
{
	private:
		int ComponentID;
		int SBReadCnt;
		int SBWriteCnt;
		int DBReadCnt;
		int DBWriteCnt;
		int TcIn;
		int TcOut;
		int BusInCycles;
		int BusOutCycles;
		
	public:
		// constructors
		Parameter(): ComponentID(-1), SBReadCnt(0), SBWriteCnt(0), DBReadCnt(0), DBWriteCnt(0),
		             TcIn(0), TcOut(0),
		             BusInCycles(0), BusOutCycles(0) {} //一般建構子,設定初始值
		Parameter(int sbrc, int sbwc, int dbrc, int dbwc, int ti, int to, int bic, int boc) 
		{
			SBReadCnt=sbrc; SBWriteCnt=sbwc;
			DBReadCnt=dbrc; DBWriteCnt=dbwc;
			TcIn=ti; TcOut=to; BusInCycles=bic; BusOutCycles=boc;
		} 
		// destructor
		~Parameter() {}
		// member functions
		inline void set_ComponentID(int compid) { ComponentID = compid; }
		inline int get_ComponentID() { return ComponentID; }
		inline void set_SBReadCnt(int sbrc) { SBReadCnt=SBReadCnt+sbrc; }
		inline int get_SBReadCnt() { return SBReadCnt; }
		inline void set_SBWriteCnt(int sbwc) { SBWriteCnt=SBWriteCnt+sbwc; }
		inline int get_SBWriteCnt() { return SBWriteCnt; }
		inline void set_DBReadCnt(int dbrc) { DBReadCnt=DBReadCnt+dbrc; }
		inline int get_DBReadCnt() { return DBReadCnt; }
		inline void set_DBWriteCnt(int dbwc) { DBWriteCnt=DBWriteCnt+dbwc; }
		inline int get_DBWriteCnt() { return DBWriteCnt; }
		inline void set_TcIn(int ti) { TcIn=TcIn+ti; }
		inline int get_TcIn() { return TcIn; }
		inline void set_TcOut(int to) { TcOut=TcOut+to; }
		inline int get_TcOut() { return TcOut; }
		inline void set_BusInCycles(int bic) { BusInCycles=BusInCycles+bic; }
		inline int get_BusInCycles() { return BusInCycles; }
		inline void set_BusOutCycles(int boc) { BusOutCycles=BusOutCycles+boc; }
		inline int get_BusOutCycles() { return BusOutCycles; }
		friend ostream& operator<<( ostream& os, const Parameter& para ) 
		{
			os <<para.ComponentID
			   <<setw(10)<<para.SBReadCnt<<setw(10)<<para.SBWriteCnt
			   <<setw(10)<<para.DBReadCnt<<setw(10)<<para.DBWriteCnt
			   <<setw(10)<<para.TcIn<<setw(10)<<para.TcOut
			   <<setw(10)<<para.BusInCycles<<setw(10)<<para.BusOutCycles<<endl;
			return os;
		}
};

class ComponentOnSharedBus
{
	private:
		int SharedBusID;
		int numOfcomp;		
	public:
		list<Parameter> paraList;         //宣告儲存Parameter用的list
		// constructor
		ComponentOnSharedBus(): SharedBusID(-1){}
		ComponentOnSharedBus(int sbid) { SharedBusID=sbid; }
		// destructor
		~ComponentOnSharedBus() {}
		// member functions
		inline void set_SharedBusID(int sbid) { SharedBusID = sbid; }
		inline int get_SharedBusID() { return SharedBusID; }
		inline void set_numOfcomp() { numOfcomp = paraList.size(); }
		inline int get_numOfcomp() { return numOfcomp; }
		
		friend ostream& operator<<( ostream& os, const ComponentOnSharedBus& cosb ) 
		{
		 	os << "\n\nSharedBusID = " << cosb.SharedBusID <<"\n"<<"NumOfComponent = "<<cosb.numOfcomp<<"\n";
		 	os << "ComponentID SBReadCnt SBWriteCnt DBReadCnt DBWriteCnt TcIn TcOut BusInCycles BusOutCycles" << "\n";
		 	copy( cosb.paraList.begin(), cosb.paraList.end(), ostream_iterator<Parameter>(os,"") );
		 	return os;
	 	}
};

class AnalyzeSystem
{
	private:
		int numOfSharedBus;
	public:
		list<ComponentOnSharedBus> cosbList;
		// constructor
		AnalyzeSystem(): numOfSharedBus(-1) {}
		AnalyzeSystem(int nsb) { numOfSharedBus=nsb; }
		// destructor
		~AnalyzeSystem() {}
		// member fuctions
		inline void set_numOfSharedBus(int nsb) { numOfSharedBus = nsb; }
		inline int get_numOfSharedBus() { return numOfSharedBus; }

		friend ostream& operator<<( ostream& os, const AnalyzeSystem& anasys ) 
		{
		 	os << "\n======== Analyze ====================="<<endl;
		 	copy( anasys.cosbList.begin(), anasys.cosbList.end(), ostream_iterator<ComponentOnSharedBus>(os,"") );//同上,看BusSystem上面的Bus有沒有全部讀到
		 	os << "\n======= End of SharedBusSystem =======\n\n\n";
		 	return os;
	 	}
};

#endif	// End of _Analysis_h_
