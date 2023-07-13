
#include "Stair.h"
//#include "Event.h"
#include"Arrival.h"
//#include"Hospital.h"


Stair::Stair(Hospital* h, int ET, int ID):Event(h,ET, ID) {
	this->setType('S');
	this->setET(ET);

}


void Stair::Print(std::ostream&out) const {
	cout << this->EventType << " " <<this->getET()<< " "<< this->getID() << endl;
}

void Stair::setType(char type) {
	this->EventType = type;
 }




void Stair::Execute() {
	//note for phase 1 it is asked to remove a cargo from the waiting list
	//for phase2, we change this to a visitor
	int IDofStair = this->getID();
	int srcfloornumber=0, trgtfloornumber=0;
//we need the srcfloor to remove from its Pickables list the cargo we want to move to the Stairs list
 //and we need the target floor to know which list (Up or Down) we will be looking in
	int x = HospitalManager->getEvents()->getCount();
		PQNode<Event*>* temp = HospitalManager->getEvents()->getFront();
		Node<Event*>* nodetemp = new Node<Event*>();
		nodetemp->setData(temp->getData());
		while (temp) {
			if (temp->getData()->getID() == IDofStair && temp->getData()->getType() == 'A') {
				Event* p2 = temp->getData();
				auto* arrEvent = dynamic_cast<Arrival*>(p2);
				//dynamic casting to access the child's attributes using the parent class
				srcfloornumber = arrEvent->getSrcFloor();
				trgtfloornumber = arrEvent->getTrgtFloor();
				break;
			}
			temp = temp->getNext();
		}
	//}


	Floor* targetfloor = HospitalManager->getFloorwithID(srcfloornumber);
	Queue<Pickable*>* targetPickableslist;

	if (trgtfloornumber > srcfloornumber) 
		targetPickableslist = targetfloor->getUpVisitors();
	else targetPickableslist = targetfloor->getDownVisitors();
	{
		Node<Pickable*>* node = targetPickableslist->getTop();
		Stack<Pickable*>hold;
		Pickable* targetPickable;
		while (node) {
			hold.push(targetPickableslist->dequeue());
			if (node->getData()->getPickableId() == IDofStair) {
				hold.pop(targetPickable);//or the line below
				//targetPickable = node->getData();
//we should potentially add a function that removes an element from a PQ or Queue by using a stack as we use this a lot	
				HospitalManager->getStairs()->enqueue(targetPickable);
				targetPickable->UsedStairs = true;
				//cout << "----------------------------COUNT of StairsList at this POINT IS  " << HospitalManager->getStairs()->getCount() << endl;
				while (!targetPickableslist->isEmpty())
					hold.push(targetPickableslist->dequeue());
				while (!hold.isEmpty()) {
					hold.pop(targetPickable);
					targetPickableslist->enqueue(targetPickable);
				}
				return;
			}
			node = node->getNext();
		}
	}
}


//}