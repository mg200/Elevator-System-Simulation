#pragma once
#include"Node.h"
template <class T>
class Queue
{
	T Data;
	Node<T>* top;
	Node<T>* rear;
    int count;
public:
	Queue() {
		top = nullptr;
		rear = nullptr;
        count = 0;
	}
    bool isEmpty() {
        return (top == nullptr);
    }
    Node<T>* getTop()const {
        return top;
    }
   
    void reverseQueue()
    {
        if (this->isEmpty())
            return;
        Node<T>* data = this->getTop();
        this->dequeue();
        this->reverseQueue();
        this->enqueue(data->getData());
    }

     void enqueue(const T&newelement) {
         Node<T>* temp = top;
         Node<T>* newnode = new Node<T>();
         newnode->setData(newelement);
         if (isEmpty()) {
             top = newnode;
             rear = newnode;
             count++;
             return;
         }
         while (temp != nullptr) {       //t5lof 3kly, emal e7na 3amleen el rear dah leh
             if (temp == rear) {
                 newnode->next = nullptr;
                 temp->next = newnode;
                 rear = newnode;
                 count++;
                 break;
             }

             temp = temp->getNext();
         }
     }
    T dequeue() {
        if (isEmpty())return NULL;
        Node<T>* temp = top;
        if (!top->next)rear = nullptr;//added 23/11
        top = top->getNext();
        //count--;
        this->count--;
        return temp->data;
    }

    void Traverse() {
        Node<T>* temp = top;
        while (temp != nullptr) {
            //cout << "Hello" << endl;
            cout << temp->getData() << endl;
            temp = temp->getNext();
        }
    }
    void TraversePointer() {
        Node<T>* temp = top;
        while (temp) {
            cout << *(temp->getData()) << endl;
            temp = temp->next;
        }
    }
    int countTheList() {
        Node<T>* temp = this->getTop();
        int sum = 0;
        while (temp) {
            sum++;
            temp = temp->getNext();
        }
        return sum;
    }
    
    bool DepopulateQueue() {
        if (this->isEmpty())return false;
        Node<T>* temp = this->getTop();
        while (temp) {
            this->dequeue();
            temp = temp->getNext();
        }
        return true;
    }

    int getCount() {
        return count;
    }
    
    
};

// void enqueue(T newelement) {
//    Node<T>* temp = top;
//    Node<T>* newnode = new Node<T>();
//    newnode->setData(newelement);
//    if (isEmpty()) {
//        top = newnode;
//        rear = newnode;
//        return;
//    }
//    while (temp != nullptr) {
//        if (temp==rear) {
//            newnode->next = nullptr;
//            temp->next = newnode;
//            rear = newnode;
//            break;
//        }

//        temp = temp->getNext();
//    }
//}