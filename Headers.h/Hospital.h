#pragma once
#ifndef	HOSPITAL_H
#define HOSPITAL_H
#define MAX_FLOOR 100
#define MAX_SERVICE 120
#include<iostream>
//#include"Hospital.h"
#include"Queue.h"
#include"Stack.h"
#include"Floor.h"
#include"PQ.h"
#include"Node.h"
#include"Elevator.h"
#include"LinkedList.h"
#include"UI.h"



//class UI;
class Event;
class Hospital
{		
	int VisitedEvents;
	int timestep;
	int maxtimestep;
	UI* UIPtr;
	int numberFloors;

	Floor* floors[MAX_FLOOR]; //we assume that we will max have 100 floors
	Queue<Pickable*>stairs;
	PQ<Event*>Events;
	PQ<Event*>CurrentEvents;//a PQ to store the events that should be done at a given timestep
	Queue <Pickable*>CompletedList;
	LinkedList<Pickable*> InService;// [MAX_SERVICE] ;
	int InserviceCount;
	int numofCurrentEvents=0;//different from one below
	Elevator Velevator;
	Elevator Pelevator;
	Elevator Celevator;
	int S, autoS;
public:
	Hospital();
	void SimpleSimulator();
	void SimpleSimulator2();
	void SimpleSimulator3();
	void RunSimulation(int t);
	void Loading(int t);

	void FillMyStats(Pickable*);
	int CalcMyAvgs();
	
	void RemoveifMovedMorethanAutoS(bool dir, int floor,int t);
	int countlist(PQ<Pickable*>* p);
	bool shouldsimulationEnd();
	void AutoSCase(int);
	void UpdateStairs(int t);
    void ReachedbyStairs(int t);
	int countlist(PQ<Event*>* list, char type = 0);
	double autoSpercentage = 0;
	double autoScount = 0;
	Queue<Pickable*>leaving;
	int currNumEvents;	
	void CreateEvent(Event*);
	void CreateFloors(int);
	
	//Hospital(ifstream&inputfile);
	Floor**getListOfFloors();
	Queue<Pickable*>*getStairs();
	Floor* getFloorwithID(int ID);
	//Setters and Getters
	void setNumFloors(int numfloors);
	int getS() {
		return this->S;
	}
	void UpdateWaitingLists() {
		Node<Pickable*>* t1;
		PQNode<Pickable*>* t2;
		for (int i = 0; i < numberFloors; i++) {
			/*	t1 = getFloorwithID(i)->getDownCargos()->getTop();
				while (t1) {
					t1->getData()->Waitedfor++;
					t1 = t1->next;
				}
				t1 = getFloorwithID(i)->getUpCargos()->getTop();
				while (t1) {
					t1->getData()->Waitedfor++;
					t1 = t1->next;
				}*/
			t1 = getFloorwithID(i)->getUpVisitors()->getTop();
			while (t1) {
				t1->getData()->Waitedfor++;
				t1 = t1->next;
			}
			t1 = getFloorwithID(i)->getDownVisitors()->getTop();
			while (t1) {
				t1->getData()->Waitedfor++;
				t1 = t1->next;
			}
			/*t2 = getFloorwithID(i)->getUpPatients()->getFront();
			while (t2) {
				t2->getData()->Waitedfor++;
				t2 = t2->next;
			}
			t2 = getFloorwithID(i)->getDownPatients()->getFront();
			while (t2) {
				t2->getData()->Waitedfor++;
				t2 = t2->next;
			}*/
		}
	}

	int getNumFloors()const;
	void setElevatorType(char type, int which);	  //mlha4 lazma t2rebn
	char getElevatorType()const;
	Elevator*getVisitorElevator();
	Elevator*getPatientElevator();
	Elevator*getCargoElevator();
	void setSandautoS(int s, int autos);
	PQ<Event*>*getEvents();
	PQ<Event*>*getCurEvents();
	void PrintEvents();
	void Phase_2_TestFunction();
	int getMaxTimeStep() {return maxtimestep;}
	void PrintFloors();
	void PopulateCurrentEvents(int curtimestep);
	void LoadPassengers(string dir);//1 for visitor elevators, 2 for patient elevators, 3 for cargo	
	void UnloadPassengers();
	void putinCompletedList(Pickable*);
	int totalWaiting();
	
	
	bool WriteOutputFile(); 


	//Phase 1 
	void moveToCompletedList();


	Queue<Pickable*>* getCompletedList() {
		return &CompletedList;
	}
	int& getCountOfCompleted() {
		return countCom;
	}
	int countCom = 0, SandL = 0;
	int totalCargos = 0;
	int totalPassengers = 0;
	int avgWait = 0;
	int avgSerV = 0;
	//Testing Functions
	void printspeed();
	void TraverseEventsIDs();
	void updateElevators(int = 0);
	int CountElev();
	int readNumEvents;
	int nLeave = 0;
	void InsertionBlock(Elevator&, Queue<Pickable*>&);
	void InsertionBlock(Elevator&, PQ<Pickable*>&);
	void CascadeRequest(Pickable*, Elevator* e);
	void printCompletedlist() {
		Node<Pickable*>* temp = CompletedList.getTop();
		while (temp) {
			temp->getData()->printPickable();
			temp = temp->getNext();
		}
	}
	void printStairs(Hospital*);
	~Hospital() {
		
	}
	int PCount = 0;
	int VCount = 0;
	int CCount = 0;
	void countIndividual() {
		Pickable* p = nullptr;
		Node<Pickable*>*t = CompletedList.getTop();
		while (t) {
			if (t->getData()->getType() == 'P')PCount++;
			if (t->getData()->getType() == 'V')VCount++;
			if (t->getData()->getType() == 'C')CCount++;
			t = t->getNext();
		}
	}

	void setElevatorsData();
	bool AllCargos();
	void Unload(int);
	void LoadPatients();
	void LoadVisitors();
	void LoadCargos();
	void Load(int);
	void InsertIntoInService(Pickable*p) {
		InService.InsertStack(p);
		p->Waiting = true;
	}
	void UpdateInservice() {
		Node<Pickable*>* temp = InService.getHead();
		while (temp) {
			//temp->getData()->UpdateWaitedTime();
			temp = temp->getNext();
		   }

	}
	bool RemoveFromInservice(int id) {
		Node<Pickable*>*temp = InService.getHead();
		if (!temp)return false;
		while (temp) {
			if (temp->getData()->getPickableId() == id) {
				InService.Delete(temp);	
				return true;
		 }
			temp = temp->getNext();
		}
	}

	
};
#endif
