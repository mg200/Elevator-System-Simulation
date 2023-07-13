#pragma once
#include"Pickable.h"
#include"PQ.h"
#include"Queue.h"
#include"Node.h"
#include<string>
class Hospital;
class Floor
{
	const int ID;
	static int id;
	Queue<Pickable*>UpVisitors;
	Queue<Pickable*>DownVisitors;
	Queue<Pickable*>UpCargos;
	Queue<Pickable*>DownCargos;
	PQ<Pickable*>UpPatients;
	PQ<Pickable*>DownPatients;
public:
	bool hasContagiousCase;
	Floor(int id);
	int getFloorID()const;
	Queue<Pickable*>*getUpVisitors();
	Queue<Pickable*>*getDownVisitors();
	Queue<Pickable*>*getUpCargos();
	Queue<Pickable*>*getDownCargos();
	PQ<Pickable*>*getUpPatients();
	PQ<Pickable*>*getDownPatients();
	string getIDs(Queue<Pickable*>* list);
	string getIDs(PQ<Pickable*>* list);
	
 //phase 1 testing functions
	void PrintPatients();
	void printFloorsPickables(char Type=0, int=0,Hospital*p=nullptr);//1 up, 0 down
	void printEachPickable(int which) {//1 visUp,2 visDo, 3 PatUp, 4PatDo, 5 Carup,6cardown
		//to avoid "cout" and "cin" here, we might need to write this in the UI class with a Floor* as a parameter
		if (which == 1) {
			cout<<"["<<getIDs(getUpVisitors())<<"]";
		}
		else if (which == 2) {
			cout << "[" << getIDs(getDownVisitors()) << "]";

		}
		else if (which == 3) {
			cout << "[" << getIDs(getUpPatients()) << "]";

		}
		else if (which == 4) {
			cout << "[" << getIDs(getDownPatients()) << "]";

		}
		else if (which == 5) {
			cout << "[" << getIDs(getUpCargos()) << "]";

		}
		else if (which == 6) {
			cout << "[" << getIDs(getDownCargos()) << "]";

	  }
	}
};

