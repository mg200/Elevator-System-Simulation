#include "Elevator.h"
#include"Hospital.h"
#include"Request.h"
using namespace std;

Elevator::Elevator() {
	//for some reason the stair event didn't 
	currentfloor = 0;
	//add PrevStopFloor to handle case where passenger has left so we don't visit its target floor unnecessarily
	//but think how you handle this if there is another passenger who DIDn't Leave or use Stairs and has the same target floor
	Upstopfloor = -1;
	Downstopfloor = INFINITY;
	this->state = 'I';
	this->movingdirection = "UP";
	hasMoved = 0;
	hasLoaded = false;
	hasUnloaded = false;
}
bool Elevator::moveOutofIdleState(Hospital* h = nullptr) {
	IdleCount--;
	if (IdleCount == 0) {
		isIdle = false;
		this->previousState = 'I';
		NoRequest = 0;
		this->setState('A');
		IdleCount = 3;
	}
	return true;
}
void Elevator::Sync(Hospital* pHos, int t) {
	if (movingdirection == "UP")SyncUPElevator(pHos,t);
	if (movingdirection == "DOWN")SyncDOWNElevator(pHos,t);}

bool Elevator::putInIdleState(Hospital* h) {
	if ((this->getMovingDirection() == "UP" && (this->currentfloor == h->getNumFloors() - 1))
		|| (this->getMovingDirection() == "DOWN" && (this->currentfloor == 0))) {
		isIdle = true;
		this->setState('I');
		if (movingdirection == "UP")movingdirection = "DOWN";
		if (movingdirection == "DOWN")movingdirection = "UP";
	}
	if (NoRequest == 10 && this->getElevatorState() == 'A') {
		isIdle = true;
		this->setState('I');
		NoRequest = 0;
	}
	return true;
}
void Elevator::SyncDOWNElevator(Hospital*pHos, int t) {
	if (ContCase) {
		state = 'M';
		if (ContagiousCount) {
			ContagiousCount--;
			return;
		}
		ContCase = false;
		Pickable* cont=Passengers.dequeue();
		pHos->getCompletedList()->enqueue(cont);
		cont->Completed = true;
		this->currentfloor = ContagiousTargetfloor;
		this->state = 'A';
		if (this->currentfloor == 0 || this->currentfloor == hospitalmaxfloor - 1)state = 'I';
		if (currentfloor == 0)movingdirection = "UP";
		if (currentfloor == hospitalmaxfloor - 1)movingdirection = "DOWN";
		return;
	}
	if (currentfloor == hospitalmaxfloor - 1 && justTurned) {
		state = 'A';
		justTurned = false;

		return;
	}
	if (currentfloor == 0 && sentfromM) {
		this->state = 'I';
		sentfromM = false;
		return;
	}
	if (currentfloor == 0&&sentfromU) {
		sentfromU = false;
		if (Passengers.getFront())
			this->state = 'U';
		redflag = true;
		return;
			}
	if (redflag) {
		currentfloor = 0;
		this->state = 'I';
		this->movingdirection = "UP";
		redflag= false;
		return;
	}
	if (movingdirection != "DOWN")return;
	if (state == 'I' && Downstopfloor != INFINITY) {
		moveOutofIdleState();
		if (FirstMove)FirstMove = false;
		return;
	}
	if (currentfloor == Downstopfloor && movingdirection == "DOWN") {
		Downstopfloor = INFINITY;
		if (Upstopfloor != -1) {
			this->movingdirection = "UP";
			//this->state = 'I';
			TurnedUp = true;
			//cout <<type<< " TurnedUp = true\n";
		}
	}
	if (state == 'A' && Downstopfloor != INFINITY) {
		if (Downstopfloor != INFINITY) {
			this->state = 'M';
			currentfloor --;
			Distance = 0;
			return;
		}
	}

	if (state == 'M') {
		if (Distance != speed - 1) {
			Distance++;
			return;
		}
		else {
			{
				if (Passengers.getFront()) {
					if (Passengers.peek()->getTargetFloor() != currentfloor && !CheckWaitingListsatfloor(pHos, currentfloor)) {
						//nobody to Unload here and no passengers in the waiting list of my direction
						currentfloor--;
						Distance = 0;
						return;
					}
				}
				else if (!Passengers.getFront()) {
					if (!CheckWaitingListsatfloor(pHos, currentfloor)) {
						//nobody is in the elevator here and no passengers in the waiting list of my direction	of my type
						this->currentfloor--;
						if (currentfloor < 0) {
							this->currentfloor = 0;
							/*	this->state = 'I';*/
							sentfromM = true;
						}
						Distance = 0;
						return;
					}
				}
			}
			if (Passengers.getFront()) {
				if (Passengers.peek()->getTargetFloor() == currentfloor) {
					this->state = 'U';
					Distance = 0;
					return;
				}
			}
			if (CheckWaitingListsatfloor(pHos, currentfloor)) {
				this->state = 'A';
				Distance = 0;
				return;
			}
			if (!CheckWaitingListsatfloor(pHos, currentfloor) && VisitList.isEmpty()) {
				state = 'A';
				Distance = 0;
				return;
			}
		}
	}
	if (state == 'U') {
		
		if (!VisitList.isEmpty() && Downstopfloor != INFINITY) {
			this->state = 'M';
			currentfloor--;
			if (currentfloor < 0) {
				this->currentfloor = 0;
			/*	this->state = 'I';*/
				sentfromU = true;
			}								 
			return;
		}
		if (CheckWaitingListsatfloor(pHos, currentfloor)) {
			this->state = 'A';
			//should we add here distance =0 or distance =1;
			return;
		}
		else if (!CheckWaitingListsatfloor(pHos, currentfloor) && VisitList.isEmpty())
		{
			state = 'A';
			return;
		}

	}

}
void Elevator::SyncUPElevator(Hospital* pHos, int t) {	//Sync UP
	if (movingdirection != "UP")return;
	//contagious case handling
	if (ContCase) {
		state = 'M';
		if (ContagiousCount) {
			ContagiousCount--;
			return;
		}
		ContCase = false;
		Pickable* cont = Passengers.dequeue();
		pHos->getCompletedList()->enqueue(cont);
		cont->Completed = true;
		this->currentfloor = ContagiousTargetfloor;
		this->state = 'A';
		if (this->currentfloor == 0 || this->currentfloor == hospitalmaxfloor - 1)state = 'I';
		if (currentfloor == 0)movingdirection = "UP";
		if (currentfloor == hospitalmaxfloor - 1)movingdirection = "DOWN";

		return;
	}

	if (currentfloor == hospitalmaxfloor - 1) {
		if (Distance == speed - 1) {
			if (Passengers.getFront()) {
				this->state = 'U';
				goToIdleinmaxfloor = true;
				return;
			}
			this->state = 'I';
			countmaxidle++;
			if (countmaxidle != 3)return;
			   else {
				   countmaxidle = 0;
				   this->movingdirection = "DOWN";
				   this->state = 'A';
				   Distance = 0;
				   this->currentfloor = hospitalmaxfloor - 1;
				   justTurned = true;
				   return;
			   }
					 }
	}

	if (state == 'I' && Upstopfloor != -1) {
		moveOutofIdleState();
		FirstMove = false;
		return;
	}

	if (state == 'A' && Upstopfloor!=-1) {	  //if (state == 'A' && !VisitList.isEmpty()) 
		if (Upstopfloor != -1) {
			this->state = 'M';
			currentfloor += 1;
			Distance = 0;
			return;
		}
	}
	if (state == 'M') {
		if (Distance != speed - 1) {
			Distance++;
			return;
		}
		else {
			{
				if (Passengers.getFront()) {
					if (Passengers.peek()->getTargetFloor() != currentfloor && !CheckWaitingListsatfloor(pHos, currentfloor)) {
						//nobody to Unload here and no passengers in the waiting list of my direction
						currentfloor++;
						Distance = 0;
						return;
					}
				}
					else if (!Passengers.getFront()) 
	{
					//cout << "Currentfloor here is-------------------------<<" << currentfloor << endl;
					if (currentfloor<0) {
						this->currentfloor = 0;
						Distance = 0;
						putInIdleState(pHos);
						return;
					}
						if (!CheckWaitingListsatfloor(pHos, currentfloor)) {
							//nobody is in the elevator here and no passengers in the waiting list of my direction	of my type
							this->currentfloor++;
							Distance = 0;
							return;
						}
					}
			}
			if (Passengers.getFront()) {
				if (Passengers.peek()->getTargetFloor() == currentfloor) {
					this->state = 'U';
					Distance = 0;
					return;
				}
			}
			if (CheckWaitingListsatfloor(pHos, currentfloor)) {
				this->state = 'A';		  
				Distance = 0;
				return;
		 }
			if (!CheckWaitingListsatfloor(pHos, currentfloor) && VisitList.isEmpty()) {
				state = 'A';
				Distance = 0;
				return;
			}
		}
	}
	if (state == 'U') {
		if (!VisitList.isEmpty() && Upstopfloor != -1) {
			this->state = 'M';
			currentfloor++;
			return;
		}
		if (CheckWaitingListsatfloor(pHos, currentfloor)) {
			this->state = 'A';
			//should we add here distance =0 or distance =1;
			return;
		}
		else if (!CheckWaitingListsatfloor(pHos, currentfloor)&&VisitList.isEmpty()) 
		 {
				state = 'A';
				return;
		}		
	}
}

bool Elevator::thereIsrequestUp(Hospital* p, int f) {
	PQ<Pickable*>* Plist = nullptr;
	Queue<Pickable*>* VClist = nullptr;
	PQNode<Pickable*>* temp = nullptr;
	Node<Pickable*>* VCtemp = nullptr;
	if (type == 'P') {
		if (movingdirection == "UP") Plist = p->getFloorwithID(f)->getUpPatients();
		if (movingdirection == "DOWN") Plist = p->getFloorwithID(f)->getDownPatients();
		temp = Plist->getFront();
		while (temp) {
			if (temp->getData()->getTargetFloor() > this->Upstopfloor) {
				Upstopfloor = temp->getData()->getTargetFloor();
				return true;
			}
		}
		return false;
	}

	if (type == 'V') {
		if (movingdirection == "UP") VClist = p->getFloorwithID(f)->getUpVisitors();
		if (movingdirection == "DOWN") VClist = p->getFloorwithID(f)->getDownVisitors();
		VCtemp = VClist->getTop();
		while (temp) {
			if (temp->getData()->getTargetFloor() > this->Upstopfloor) {
				Upstopfloor = VCtemp->getData()->getTargetFloor();
				return true;
			}
		}
		return false;
	}

	if (type == 'C') {
		if (movingdirection == "UP") VClist = p->getFloorwithID(f)->getUpCargos();
		if (movingdirection == "DOWN") VClist = p->getFloorwithID(f)->getDownCargos();
		VCtemp = VClist->getTop();
		while (temp) {
			if (temp->getData()->getTargetFloor() > this->Upstopfloor) {
				Upstopfloor = VCtemp->getData()->getTargetFloor();
				return true;
			}
		}
		return false;
	}

}

void Elevator::SyncFinal(Hospital* h, int t) {
	if (movingdirection != "UP")return;
	if (Upstopfloor != -1 && state == 'I') {
		moveOutofIdleState(h);
		FirstMove = false;
		return;
	}
	if (state == 'A') {
		if (currentfloor == hospitalmaxfloor - 1) {
			putInIdleState(h);
			return;
		}
	}
}
bool Elevator::CheckWaitingListsatfloor(Hospital* p, int f) {
	if (type == 'P') {
		PQ<Pickable*>* patList;
		PQNode<Pickable*>* temp;
		if (movingdirection == "UP") { 
			patList = p->getFloorwithID(f)->getUpPatients(); 
			temp = patList->getFront();
			while (temp) {
				if (temp->getData()->getTargetFloor() > temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
		if (movingdirection == "DOWN") {
			patList = p->getFloorwithID(f)->getDownPatients();
			temp = patList->getFront();
			while (temp) {
				if (temp->getData()->getTargetFloor() < temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
	}

	if (type == 'V') {
		Queue<Pickable*>* patList;
		Node<Pickable*>* temp;
		if (movingdirection == "UP") {
			//cout << "---------------9999999999999999999999999999999999999999999999999999\n";
			patList = p->getFloorwithID(f)->getUpVisitors();
			temp = patList->getTop();
			while (temp) {
				if (temp->getData()->getTargetFloor() > temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
		if (movingdirection == "DOWN") {
			patList = p->getFloorwithID(f)->getDownVisitors();
			temp = patList->getTop();
			while (temp) {
				if (temp->getData()->getTargetFloor() < temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
	}

	if (type == 'C') {
		Queue<Pickable*>* patList;
		Node<Pickable*>* temp;
		if (movingdirection == "UP") {
			patList = p->getFloorwithID(f)->getUpCargos();
			temp = patList->getTop();
			while (temp) {
				if (temp->getData()->getTargetFloor() > temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
		if (movingdirection == "DOWN") {
			patList = p->getFloorwithID(f)->getDownCargos();
			temp = patList->getTop();
			while (temp) {
				if (temp->getData()->getTargetFloor() < temp->getData()->getSourceFloor())return true;
				temp = temp->getNext();
			}
			return false;
		}
	}

}


void Elevator::InsertionBlock(PQ<Pickable*>& l, int t, Hospital* p) {
	Pickable* pick = nullptr;
	if (this->getElevatorState() != 'A')return;
	if (p->getFloorwithID(currentfloor)->hasContagiousCase) {
		int srcCont = currentfloor, trgtCont = currentfloor;
		while (!Passengers.isEmpty()) {
			Pickable* pi = Passengers.dequeue();
			int sr = pi->getSourceFloor(), tr = pi->getTargetFloor();
			char pickType = pi->getType();
			if (pickType == 'P' && sr < tr)p->getFloorwithID(currentfloor)->getUpPatients()->enqueue(pi);
			if (pickType == 'P' && sr > tr)p->getFloorwithID(currentfloor)->getDownPatients()->enqueue(pi);
			if (pickType == 'V' && sr < tr)p->getFloorwithID(currentfloor)->getUpVisitors()->enqueue(pi);
			if (pickType == 'V' && sr > tr)p->getFloorwithID(currentfloor)->getDownVisitors()->enqueue(pi);
			if (pickType == 'C' && sr < tr)p->getFloorwithID(currentfloor)->getUpCargos()->enqueue(pi);
			if (pickType == 'C' && sr > tr)p->getFloorwithID(currentfloor)->getDownCargos()->enqueue(pi);
		}
		if (p->getFloorwithID(currentfloor)->getUpPatients()->getFront())
			if (p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->ContagiousCase) {
				srcCont = p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->getSourceFloor();
				trgtCont = p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->getTargetFloor();
				cout << " Contagious Priority was UP " << p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getPriority();
				Passengers.enqueue(p->getFloorwithID(currentfloor)->getUpPatients()->dequeue());
			}
		if (p->getFloorwithID(currentfloor)->getDownPatients()->getFront())
			if (p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->ContagiousCase) {
				srcCont = p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->getSourceFloor();
				trgtCont = p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->getTargetFloor();
				cout << " Contagious Priority was DOWN " << p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getPriority();
				Passengers.enqueue(p->getFloorwithID(currentfloor)->getDownPatients()->dequeue());
			}
		if (Passengers.getFront())
		{
			this->ContCase = true;
			if (srcCont < trgtCont)movingdirection = "UP";
			else movingdirection = "DOWN";
			p->getFloorwithID(currentfloor)->hasContagiousCase = false;
			ContagiousCount = speed * abs(Passengers.getFront()->getData()->getSourceFloor() - Passengers.getFront()->getData()->getTargetFloor());
			ContagiousTargetfloor = trgtCont;
		}
		return;
	}
	if (this->getElevatorState() == 'A') {
		while (this->CurrentlyIn() < this->getCapacity()) {
			if (l.getFront()) {
				pick = l.dequeue();
				this->getPassengers()->enqueue(pick, this->computeInsertionPriority(pick));
				//put it passengers list
				UpdateStopFloorsAfterpicking(pick);
				pick->SetPT(t);
				//WE call here the cascade function to remove the request from my request list
				this->CascadeRequest(pick);
			}
			else if (!l.getFront())return;
		}
	}
}

void Elevator:: UpdateStopFloors(Pickable* p) {
	int SrcFloor = p->getSourceFloor(), TrgtFloor = p->getTargetFloor();
	if (movingdirection == "UP") {
		if (SrcFloor > Upstopfloor)Upstopfloor = SrcFloor;
		if (SrcFloor < Downstopfloor)Downstopfloor = SrcFloor;
	}
	else if (movingdirection == "DOWN") {
		if (SrcFloor < Downstopfloor) Downstopfloor = SrcFloor;
		if (SrcFloor > Upstopfloor)Upstopfloor = SrcFloor;
	}
}
void Elevator::UpdateStopFloorsAfterpicking(Pickable* p) {
	int s = p->getSourceFloor(), t = p->getTargetFloor();
	if (this->movingdirection == "UP") if (t > this->Upstopfloor)Upstopfloor = t;
	if (this->movingdirection == "DOWN") if (t < this->Downstopfloor) {
		Downstopfloor = t;
	}
}
void Elevator::InsertionBlock(Queue<Pickable*>& l, int t, Hospital* p) {
	if (this->getElevatorState() != 'A')return;
	if (p->getFloorwithID(currentfloor)->hasContagiousCase) {
		int srcCont, trgtCont;
		while (!Passengers.isEmpty()) {
			Pickable* pi = Passengers.dequeue();
			int sr = pi->getSourceFloor(), tr = pi->getTargetFloor();
			char pickType = pi->getType();
			if (pickType == 'P' && sr < tr)p->getFloorwithID(currentfloor)->getUpPatients()->enqueue(pi);
			if (pickType == 'P' && sr > tr)p->getFloorwithID(currentfloor)->getDownPatients()->enqueue(pi);
			if (pickType == 'V' && sr < tr)p->getFloorwithID(currentfloor)->getUpVisitors()->enqueue(pi);
			if (pickType == 'V' && sr > tr)p->getFloorwithID(currentfloor)->getDownVisitors()->enqueue(pi);
			if (pickType == 'C' && sr < tr)p->getFloorwithID(currentfloor)->getUpCargos()->enqueue(pi);
			if (pickType == 'C' && sr > tr)p->getFloorwithID(currentfloor)->getDownCargos()->enqueue(pi);
		}

		if (p->getFloorwithID(currentfloor)->getUpPatients()->getFront())
			if (p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->ContagiousCase) {
				srcCont = p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->getSourceFloor();
				trgtCont = p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getData()->getTargetFloor();
				cout << " Contagious Priority was UP " << p->getFloorwithID(currentfloor)->getUpPatients()->getFront()->getPriority();
				Passengers.enqueue(p->getFloorwithID(currentfloor)->getUpPatients()->dequeue());
			}
		if (p->getFloorwithID(currentfloor)->getDownPatients()->getFront())
			if (p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->ContagiousCase) {
				srcCont = p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->getSourceFloor();
				trgtCont = p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getData()->getTargetFloor();
				cout << " Contagious Priority was DOWN " << p->getFloorwithID(currentfloor)->getDownPatients()->getFront()->getPriority();
				Passengers.enqueue(p->getFloorwithID(currentfloor)->getDownPatients()->dequeue());
			}
		if (Passengers.getFront()) {
			this->ContCase = true;
			if (srcCont < trgtCont) movingdirection = "UP";
			else movingdirection = "DOWN";
			p->getFloorwithID(currentfloor)->hasContagiousCase = false;
			ContagiousCount = speed * abs(Passengers.getFront()->getData()->getSourceFloor() - Passengers.getFront()->getData()->getTargetFloor());
			ContagiousTargetfloor = trgtCont;
		}
		return;
	}
	Pickable* pick = nullptr;
	if (this->getElevatorState() == 'A') {
		while (this->CurrentlyIn() < this->getCapacity()) {
			if (l.getTop()) {
				pick = l.dequeue();
				this->getPassengers()->enqueue(pick, this->computeInsertionPriority(pick));
				UpdateStopFloorsAfterpicking(pick);
				pick->SetPT(t);
				this->CascadeRequest(pick);
			}
			else if (!l.getTop())return;
		}

	}
	//cout << "----------------------------VERY BIG FLAG, LOADIING ELEVATOR NOT IN STATE AVAIL-----------------\n";

}
																								 
void Elevator::AddtoVisitList(int SourceFloor, int TargetFloor,string pickabledirection, int id) {

	//if(floorIsOnMyList(SourceFloor))return;
	//handle the case where the sourcefloor=this->currentfloor
	if (SourceFloor == this->currentfloor && this->getElevatorState() == 'M')return;
	int requestpriority = 0;
	Request* newrequest = nullptr;
	if (this->getMovingDirection() == "UP" && SourceFloor >= this->currentfloor) {
		requestpriority = abs(this->hospitalmaxfloor - SourceFloor);
		newrequest= new Request(SourceFloor, TargetFloor,requestpriority,pickabledirection,id);
		VisitList.enqueue(newrequest,requestpriority);
	}
	else if (this->getMovingDirection() == "UP" && SourceFloor <= this->currentfloor) {
		requestpriority = -1 * SourceFloor;
		newrequest = new Request(SourceFloor, TargetFloor, requestpriority, pickabledirection,id);
		VisitList.enqueue(newrequest, requestpriority);
	}
	else if (this->getMovingDirection() == "DOWN" && SourceFloor >= this->currentfloor) {
		requestpriority = -1 * SourceFloor;
  //lama nwsl l-a5r wa7d 3la t7t f etgahna t7t, lazm lma nbd2 ntl3 n-handle el priorities bta3t ely gom wna nazl
  //btre2a mo5tlfa
  // n-add 3la kol priority el hospitalmax floor
		newrequest = new Request(SourceFloor, TargetFloor, requestpriority, pickabledirection,id);
		VisitList.enqueue(newrequest, requestpriority);
	}
	else if (this->getMovingDirection() == "DOWN" && SourceFloor <= this->currentfloor) {
		requestpriority = SourceFloor;
		newrequest = new Request(SourceFloor, TargetFloor, requestpriority, pickabledirection,id);
		VisitList.enqueue(newrequest, requestpriority);
	}
	
}
void Elevator::PrintmyRequests() {
	PQNode<Request*>* temp = this->getWillVisit()->getFront();
	while (temp) {
		temp->getData()->PrintRequest();
		temp = temp->getNext();
}
}
void Elevator::UnloadElevator(Hospital* pHospital,int t) {
	if (state != 'U')return;
	PQNode<Pickable*>* temp = Passengers.getFront();//should be changed to peek
	Pickable* pick = nullptr;
	while (temp) { 
		// the passengers are sorted in the Passengers List by order of their nearness
		//to be unloaded
		if (temp->getData()->getTargetFloor() == currentfloor) {
			pick = Passengers.dequeue();
			pHospital->putinCompletedList(pick);
			pHospital->FillMyStats(pick);
			pick->Completed = true;
			pick->SetTT(t);
		}
		temp = temp->getNext();
	}
}
void Elevator::PrintElevatorInfo() {
	/*cout << this->getType() << " elevator: speed " << this->getSpeed() << ", capacity: " << getCapacity()
		<< ", current_state: " << this->getElevatorState() << ", current_floor: " <<
		this->getCurElevatorfloor() << ", trgt_floor: " << this->targetfloor << ", moving_direction: " << this->getMovingDirection()
		<< endl;*/
	cout << this->getType() << " elevator: speed " << this->getSpeed() << ", cap: " << getCapacity()
		<< ", cur_state: " << this->getElevatorState() << ", cur_floor: " <<
		this->getCurElevatorfloor() <<    ", UpStop: " << this->Upstopfloor << ", DownStop: " << this->Downstopfloor << ", mov_dir: " << this->getMovingDirection()<< endl;
	cout << "----------" << this->type << " Elevator passengers List---------\n";
	printPassengers();
}


bool Elevator::floorIsOnMyList(int interest_floor) {
	PQNode<Request*>* temp = this->getWillVisit()->getFront();
	while (temp) {
		if (temp->getData()->getRequestFloor() == interest_floor)return true;
		temp = temp->getNext();
	}
	return false;
}

bool Elevator::ContainsFloor(int thefloorid) {
	if (this->getWillVisit()->isEmpty())return false;
	//no need to check that the floor is on our visitinglist
	int visitinglistsize = getWillVisit()->getCount();
	PQNode<Request*>* temp = this->getWillVisit()->getFront();
	while (temp) {
		if (temp->getData())return true;
		temp = temp->getNext();
	}
	return false;
}	
PQ<Request*>* Elevator::getWillVisit() {	return &VisitList;}
PQ<Request*>* Elevator::getUpRequests() {return 	&UpRequests;}
PQ<Request*>* Elevator::getDownRequests() {	return 	&DownRequests;
}
void Elevator::LoadPatients(Hospital* pHosp, PQ<Pickable*>* toload) {
	//at call we MUST check there that the elevator is in 'A' state.VVVVI
	//up patients
	Pickable* pick;
	if (this->movingdirection == "UP") {
		while (this->CurrentlyIn() < this->capacity) {
			if (!pHosp->getFloorwithID(this->currentfloor)->getUpPatients()->peek())return;
			//we check if the element at the top of the waiting list is null, if null we break without need to proceed 
			pick = pHosp->getFloorwithID(currentfloor)->getUpPatients()->dequeue();
			this->Passengers.enqueue(pick, computeInsertionPriority(pick));
		}
	}
	else {
		while (this->CurrentlyIn() < this->capacity) {
			if (!pHosp->getFloorwithID(this->currentfloor)->getDownPatients()->peek())return;
			pick = pHosp->getFloorwithID(currentfloor)->getDownPatients()->dequeue();
			this->Passengers.enqueue(pick, computeInsertionPriority(pick));
		}
	}
}


void Elevator::CascadeRequest(Pickable* p) {
	PQNode<Request*>* temp = this->getWillVisit()->getFront();
	while (temp) {
		if (temp->getData()->getRequestID() == p->getPickableId()) {
			this->getWillVisit()->Delete(temp);
			break;
		}
		temp = temp->getNext();
	}
}

void Elevator::setHospialMaxFloor(int n) {this->hospitalmaxfloor = n;}										 
int Elevator::computeInsertionPriority(Pickable* p) {
	int t = p->getTargetFloor();
	if (this->movingdirection == "UP") {
		return 	abs(this->hospitalmaxfloor - t);
	}
	else {
		return t;
	}
}

bool Elevator::handleMovingState() {
	this->state = 'M';
	currentfloor++;
	return 0;
}

PQ<Pickable*>* Elevator::getPassengers() {
	return &Passengers;
}


void Elevator::setType(char type) {
	this->type = type;
}
void  Elevator::setCapacity(int capacity) {this->capacity = capacity;}
void  Elevator::setSpeed(int speed) {this->speed = speed;}
int Elevator::getSpeed()const {return this->speed;}
char  Elevator::getType()const {return type;}
int  Elevator::getCapacity()const {return capacity;}
void Elevator::setState(char state) {
	if (state == 'I' || state == 'M' || state == 'A' || state == 'U' || state == 'O')
	this->state = state;
}
char Elevator::getElevatorState()const {return this->state;}
void Elevator::setCurrentfloor(int c) {this->currentfloor = c;}
int  Elevator::getCurElevatorfloor()const {return this->currentfloor;}

void Elevator::setMovingDirection(string direction) {
	if (direction == "UP" || direction == "DOWN") {
		this->movingdirection = direction;
	}
}
string Elevator::getMovingDirection()const {
	return this->movingdirection;
}
