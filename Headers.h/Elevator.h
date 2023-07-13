#pragma once
#include<iostream>
#include<string>
#include"PQ.h"
#include"Queue.h"
#include"Pickable.h"


//Forward Declarations
class Hospital;
class Request;
class Elevator
{
private:
	 char type;
	 int capacity;
	 int speed;
	 PQ<Request*>VisitList;
	 PQ<Request*>UpRequests;
	 PQ<Request*>DownRequests;
	 PQ<Pickable*>Passengers;//size of this list is the actual number of passengers inside the elevator  
	 //passengers are enqueued by the priority of their target floor
	 char state;//I Idle, M moving, U unloading, A available, O out_of_service
	 char previousState;
	 int previousfloor;
	 int targetfloor; //floor at the top of my VisitList 
	 string movingdirection;
	 bool ContainsFloor(int thefloorid);
	 bool isIdle = true;
	 int IdleCount = 3;	
	 int hasMoved = 0;
	 int  hospitalmaxfloor;
	 bool hasLoaded = false;//would probably need to be set to false by default	
	 bool hasUnloaded = false;

public:
	void SyncElevator(Hospital*,int);
	void SyncFinal(Hospital*,int);
	void Sync2Elevator(Hospital*,int);
	void Sync3Elevator(Hospital*,int);
	void SyncUPElevator(Hospital*,int);
	void SyncDOWNElevator(Hospital*,int);
	//void IdleinMaxMin();
	void Sync(Hospital*, int);
	bool CheckWaitingListsatfloor(Hospital*, int);
	bool hasSomeonetoUnload = false;
	int countmaxidle = 0;
	int NoRequest = 0;
	int Distance = 0;
	bool redflag = false;
	bool justTurned = false;
	bool sentfromU = false;
	bool sentfromM = false;
	bool ContCase = false;
	int ContagiousCount = 0;
	int ContagiousTargetfloor;
	bool TurnedDown=false;
	bool TurnedUp=false;
	bool NoStopInFloor = false;
	bool FirstMove = true;
	bool WasAVAIL = false;
	//Setters and Getters
	Elevator();
	void setHospialMaxFloor(int);
	void setType(char type);
	void setCapacity(int capacity);
	void setSpeed(int speed);
	char getType()const;
	int getCapacity()const;
	int CurrentlyIn()const { return this->Passengers.getCount(); }
	int getSpeed()const;
	void setState(char);
	char getElevatorState()const;
	void setCurrentfloor(int);
	int getCurElevatorfloor()const;
	int getPrevElevatorfloor()const;
	void setMovingDirection(string);
	string getMovingDirection()const;
	PQ<Request*>* getUpRequests();
	PQ<Request*>* getDownRequests();
	//setters and getters end
	bool putInIdleState(Hospital*);
	bool moveOutofIdleState(Hospital*);
	bool MoveElevator(Hospital*);
	bool handleMovingState();
	void UpdateElevator(int);
	void AddtoVisitList(int,int,string,int=0);
	//void InsertinVisitingList(int=0);
	void PrintmyRequests();
	bool floorIsOnMyList(int);
	int computeInsertionPriority(Pickable*);
	int LoadElevator(Hospital* Hptr,int which);
	void LoadPatients(Hospital*, PQ<Pickable*>*);
	void LoadElevator(Hospital*, char);
	void InsertionBlock(Queue<Pickable*>&, int t=100,Hospital * p = nullptr);
	void InsertionBlock(PQ<Pickable*>&, int t=100,Hospital*p=nullptr);
	void CascadeRequest(Pickable*);
	void UnloadElevator(Hospital*, int=-1);
	void VisitListHandle();

	//it returns the number that had been loaded
	// which: 1 V, 2 P, 3 C												 
	PQ<Pickable*>*getPassengers();
	void printPassengers() {
		PQNode<Pickable*>* temp = Passengers.getFront();
		while (temp) {
			temp->getData()->printPickable();
			temp = temp->getNext();
		}
	}
	PQ<Request*>* getWillVisit();
	void PrintElevatorInfo();
	void Synchronize(Hospital*,int);
	void Synchronize2(Hospital*,int);
	bool thereIsrequestUp(Hospital* p, int f);
	bool ElevatorwasINmax = false;
	bool goToIdleinmaxfloor = false;
	int nextfloor;
	int currentfloor;
	int Upstopfloor;
	int Downstopfloor;
	void UpdateStopFloors(Pickable* p);
	void UpdateStopFloorsAfterpicking(Pickable* p);
	
	void InsertIntoRequests(Pickable*);
	bool checkUnload(int f, Hospital*ptr) {
		PQNode<Pickable*>* pickPtr = this->Passengers.getFront();
		while (pickPtr) {
			if (pickPtr->getData()->getTargetFloor() == this->currentfloor)return true;
			pickPtr = pickPtr->getNext();
		}
		return false;
	}
};
//void AdjustUpStopFloor() {
//
//}
//void AdjustDownStopFloor() {
//
//}
