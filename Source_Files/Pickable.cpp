#include "Pickable.h"
Pickable::Pickable(char type, int srcFloor, int trFloor, int pickableid, int Etimestep) {
	this->setType(type);
	this->setSourceFloor(srcFloor);
	this->setTargetFloor(trFloor);
	this->pickableID = pickableid;
	this->AT=Etimestep;
	PT = 0;
	TT = 0;
	ST = 0;
	if (srcFloor > trFloor) direction = "DOWN";
	else if(srcFloor<trFloor) direction = "UP";
	MovedMorethanAutoS = false;
	ContagiousCase = false;
}

//void Pickable::UpdateWaitedTime() {
//	this->WT++;
// }
void Pickable::SetPT(int t) {
	this->PT = t;
	WT = PT - AT;
}

void Pickable::SetTT(int t) {
	this->TT = t;
	ST = TT - PT;
}

void  Pickable::setType(char type) {
	this->type = type;
}
void  Pickable::setSourceFloor(int src_floor) {
	this->sourceFloor = src_floor;
}
void  Pickable::setTargetFloor(int trgt_floor) {
	this->targetFloor = trgt_floor;
}
char  Pickable::getType()const {
	return type;
}
int  Pickable::getSourceFloor()const {
	return sourceFloor;
}
int Pickable::getTargetFloor()const {
	return targetFloor;
}
int  Pickable::getPickableId()const { return pickableID; }
void  Pickable::printPickable() {
	cout << this->getType() << " " << this->getPickableId() << " " << this->getSourceFloor() << " " << this->getTargetFloor() << std::endl;
}

int Pickable::getAT()const {
	return AT;
}
int Pickable::getPT()const {
	return PT;
}
int Pickable::getWT()const { 
	return WT;
}
int Pickable::getTT()const {
	return TT;
}
int Pickable::getST()const {
	return ST;
}