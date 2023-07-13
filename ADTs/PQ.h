#pragma once
#define MAX_SIZE 100
#include<iostream>

using namespace std; //Priority Queue implemented as a linked list

template<class T>
class PQNode {
public:
	PQNode<T>* next;
	PQNode() {
		next = nullptr;
	}
	T data;
	int priority;
	int getPriority()const {
		return priority;
}
	void setData(T data) {
		this->data = data;
	}
	T getData() const {
		if(this)return this->data;
	}
	PQNode* getNext()const {
		return this->next;
	}
};
template <typename T>
class PQ
{	
	PQNode<T>* front;
	PQNode<T>* rear;
	int isMaxPQ = true;
	//T arr[MAX_SIZE];
	int count = 0;
public:
	PQ() {
		//root = nullptr;
		front = nullptr;
		rear = nullptr;
	  }
	PQ(int dummy,bool maxormin=true) {//max true, min false
		if (maxormin ==false)isMaxPQ = false;
	}
	int getSize()const{
		return count;
	}
	bool isEmpty() {
		return (front==nullptr);
	}
	PQNode<T>*getFront() {
		return front;
	}	//this code is rather risky
	T dequeue() {
		if (isEmpty())return NULL;
		PQNode<T>* temp = front;
		if (!front->next)rear = nullptr;//added 23/11
		front = front->getNext();
		count--;
		return temp->data;
		
	}
	bool enqueue(T item, int priority = 0) {
		PQNode<T>* newnode = new PQNode<T>();
		newnode->setData(item);
		newnode->priority = priority;
		PQNode<T>* temp = front;
		if (isEmpty()) {
			front = newnode;
			rear = newnode;
			newnode->next = nullptr;
			count++;
			return true;
		}
		if (isMaxPQ)  {
			if (temp) {
				if (priority > temp->getPriority()) {
					newnode->next = temp;
					front = newnode;
					count++;
					return true;
				}
				if (!temp->next) {
					newnode->next = nullptr;
					temp->next = newnode;
					count++;
					return true;
				}
				if (priority <= temp->getPriority()) {
					while (temp->next) {
						if (priority <= temp->priority && priority > temp->getNext()->getPriority()) {
							newnode->next = temp->next;
							temp->next = newnode;
							count++;
							return true;
						}
						temp = temp->next;
						if (!temp->next) {
							newnode->next = nullptr;
							temp->next = newnode;
							count++;
							return true;
						}
					}
				}
			}
		}
		else {
			if (temp) {
				if (priority <temp->getPriority()) {
					newnode->next = temp;
					front = newnode;
					count++;
					return true;
				}
				if (!temp->next) {
					newnode->next = nullptr;
					temp->next = newnode;
					count++;
					return true;
				}
				if (priority >= temp->getPriority()) {
					while (temp->next) {
						if (priority >= temp->priority && priority < temp->getNext()->getPriority()) {
							newnode->next = temp->next;
							temp->next = newnode;
							count++;
							return true;
						}
						temp = temp->next;
						if (!temp->next) {
							newnode->next = nullptr;
							temp->next = newnode;
							count++;
							return true;
						}
					}
				}
			}
		}
	}
	void Traverse() {
		PQNode<T>* temp = front;
		while (temp) {
			cout << temp->getData() << endl;
			temp = temp->getNext();
		}
	}

	//reverse a priorityQueue
	void reverseQueue()
	{
		if (this->isEmpty())
			return;
		PQNode<T>*data = this->getFront();
		this->dequeue();
		this->reverseQueue();
		this->enqueue(data->getData());
	}


	void TraversePointer() {
		PQNode<T>* temp = front;
		while (temp) {
			cout << *(temp->getData()) << endl;
			temp = temp->next;
		}
	}

	int getCount()const {
		return count;
	}
	T peek() {
		return this->getFront()->getData();
			 }
	bool Delete(PQNode<T>* target) {
		if (!this->getFront())return false;
		if (target == this->getFront()) {
			this->dequeue();
			return true;
		}
		PQNode<T>* prev = this->getFront();
		PQNode<T>* after;
		if (!prev->getNext()){
			this->dequeue();
			return true;
		}
			while (prev) {
				if (prev->getNext() == target) {
					after = target->getNext();
					PQNode<T>* temp = target;
					temp->next = nullptr;
					prev->next = after;
					delete temp;
					break;
				}
					prev = prev->next;
			}
			return 1;
	}
	void DepopulatePQ() {
		if (this->isEmpty())return;
		PQNode<T>* temp = front;
		while (temp) {
			this->dequeue();
			temp = temp->getNext();
		}
		count = 0;
	}

	/*RemoveNode(PQNode<T>* node) {
		PQNode<T>* temp = this->getFront();
		while (temp) {


		 }
	}*/
};


/*bool enqueue(T item,int priority) {
if (!count) arr[count++] = item;
	if (count == 1) {

		arr[count++] = item;
		return true;
	}
	if (count == 2){
		arr[count++] = item;
		return true;
}

}
*/
