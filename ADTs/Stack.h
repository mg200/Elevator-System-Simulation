#pragma once
#include"Node.h"
template <class T>
class Stack
{
	Node<T>* top=nullptr;
public:
	Node<T>* getTop() {
		return top;
	}
	void setNext(Node<T>*itsnext) {
	}

	bool isEmpty() {
		return (top == nullptr);
	}
	bool push(T* obj) {
		Node<T>* newnode = new Node<T>();
		newnode->setData(obj);
		if (isEmpty()) {
			newnode->next = nullptr;
			top = newnode;
		}
		else {
			newnode->next = top;
			top = newnode;
		}
		return true;
	}

	bool push(T data){
		Node<T>* newnode = new Node<T>();
		newnode->setData(data);
		if(isEmpty()){
			newnode->next = nullptr;
			top = newnode;
		}
		else {
			newnode->next = top;
			top = newnode;
		}
		return true;
	}	
	bool push(Node<T>* dataptr) {

		if (isEmpty()) {
			dataptr->next = nullptr;
			top = dataptr;
		}
		else {
			dataptr->next = top;
			top = dataptr;
		}
		return true;
	}
	bool pop(T&val) {
		if (isEmpty())return false;
		Node<T>* temp = top;
		top = temp->next;
		val= temp->getData();
		delete temp;
		return true;
	}
	void pop() {
		Node<T>* temp = top;
		top = temp->next;
		delete temp;
	}
	void display() {
		Node<T>* temp = top;
		while (temp != nullptr) {
			std::cout << temp->getData() << std::endl;
			temp = temp->getNext();
		}

	}

};

