#pragma once
#include<iostream>
using namespace std;
template<typename T>
class Node
{
public:

	T data;
	Node* next=nullptr;
	
	Node* getNext() {		
		return next;
	}
	T getData() {
		return data;
	}
	void setData(T data) {
		this->data = data;
	}

};
