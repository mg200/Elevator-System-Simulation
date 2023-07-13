#pragma once
#include<iostream>
using namespace std;
class Pickable
{
	char type;	// P for Patient, V for visitor, C for Cargo
	int sourceFloor;
	int targetFloor;
	int pickableID;
	/// <summary>
	/// TODO: 
	/// </summary>
	int AT;		  //same timestep as the event 
	int PT;		  //timestep the pass/cargo enters the elevator
	int WT;	//incremented each timestep while still on waiting list, or PT-AT
	//if we want to increment something outside its class then we need to return a reference to it, NOT its value
	int TT;	// a one-time set value when it reaches the floor
	int ST; //TT-PT
public:
	string direction;
	int StairsCount = 0;
	int waitinglistpriority = 0;
	Pickable(char type, int srcFloor, int trFloor,int pickableid, int timestep=1);
	//Setters and getters
	void setType(char type);
	void SetStairsCount(int S) {
		StairsCount = S * abs(this->getSourceFloor() - this->getTargetFloor());
	}
	void setSourceFloor(int src_floor);
	void setTargetFloor(int trgt_floor);
	char getType()const;
	int getSourceFloor()const;
    int getTargetFloor()const;
    int getPickableId()const;
    void printPickable();
	//int getWaitedTime()const;
	void UpdateWaitedTime();
	void SetPT(int t);	
	void SetTT(int t);	
	int getAT()const;
	int getPT()const;
	int getWT()const;
	int getTT()const;
	int getST()const;
	bool Waiting = false;
	bool Left = false;
	bool UsedStairs = false;
	bool Completed = false;
	int Waitedfor = 0;
	bool MovedMorethanAutoS = false;
	bool ContagiousCase;
	void printParameters() {
		cout << this->type<<"ID#"<<this->pickableID<< " "<<this->getTT() << " " << this->getPickableId() << " " << this->getAT() << " " << this->getWT() << " " << ST << endl;

	}
	
};					

