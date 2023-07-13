#pragma once
#include"Node.h"
template <typename T>
class LinkedList
{
	Node<T>*head;
	int count = 0;
public:
	Node<T>* getHead()const {
		return head;
	}
	bool isEmpty() {
		return(head == NULL);
	}
	
	void PrintList() const
	{
		Node<T>* p = head;
		while (p)
		{
			cout << p->getData() << "->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	
	bool InsertStack(T data, bool stacklike=true) {
		Node<T>* newnode = new Node<T>();
		newnode->data = data;
		if (isEmpty()) {
			head = newnode;
			newnode->next = nullptr;
		}
		else if(!isEmpty()&&stacklike){
			newnode->next = head;
			head = newnode;
		}
		else if (!isEmpty() && !stacklike) {
			Node<T>* temp = head;
			while (temp != NULL) {
				if (temp -> next == NULL) {
					newnode->next = NULL;
					temp->next = newnode;
					break;
				}
				temp = temp->next;
			}
		}
		count++;
		return true;
	}
	void InsertAt(int pos, T data) {
		int i = 0;
		Node<T>* temp = head;
		Node<T>* newnode= new Node<T>();
		newnode->data = data;
		while (i <= pos) {
			if (i == (pos-1)) {
				newnode->next = temp -> next;
				temp->next = newnode;
				break;
			}
			temp = temp->next;
			i++;
		}
		count++;
	}
	int getCount() {
		return count;
	}

	bool Remove(T&removeditem) {
		if (isEmpty())return false;
		Node<T>* temp = head;
		head = temp->getNext();
		removeditem = temp->getData();
		return true;
	}

	bool Pop(int n=1) {	 //this better be called Enqueue as it's FIFO
		//if you give it say 7 6 5 4 in this order, it puts them as 4 5 6 7 with head pointing to 4 and "Pop()"ing results in removing 7
//function mot5lfa, bt4eel a5r wa7d w hya btshawr 3l awl		
		if (isEmpty()) return false;
		for (int i = 0;i < n;i++) {
			Node<T>* temp = head;
			if (temp->next == NULL) {
				delete temp;
				head = NULL;
				count--;
				return true;
			}
			while (temp->next != NULL) {
				if (temp->next->next == NULL) {
					delete temp->next;
					temp->next = NULL;
					break;
				}
				temp = temp->next;
			}
			count--;
		}
	}
	void display() {
		Node<T>* temp;
		temp = head;
		if (!head) {
			cout << "List is empty" << endl;
			return;
		}
		while (temp != nullptr) {
			std::cout << temp->getData()<< std::endl;
			temp = temp->next;
		}
	}
	bool isSorted(Node<T>* head) {
		Node<T>* temp = head;
		if (temp->next == nullptr)return true;
		if (temp->next->getData() > temp->getData()&&temp->next!=nullptr)isSorted(temp->next);
		else return false;
	}
	// Traversal using recursion
	//normalTraverse specifies the direction of traversal, which is obviously defaulted to be "true" from head to tail
	void Traverse(Node<T>* head, bool normalTraverse = true) {
		Node<T>* temp = head;
		if (temp == nullptr)return;
		if (normalTraverse) {
			cout << temp->getData() << endl;
			Traverse(temp->getNext());
		}
		else{
			Traverse(temp->getNext(),false);
			cout << temp->getData() << endl;
		}
	}

	 bool notNULL(Node<T>* node) {
		 return(node != NULL);
	 }
	 void setAsHead(Node<T>*p) {
		 head = p;
	 }
	 
	 //Iterative Reversal
	void Reverse() {
		if (isEmpty())return;
		Node<T>* p1=head;
		Node<T>* p2;
		Node<T>* p3;
		if (head->next == nullptr)return;
		if (p1->next->next == nullptr) {
			p2 = p1->next;
			head = p2;
			head->next = p1;
			p1->next = nullptr;
			return;
		}
		else {
			p2 = p1->next;
			p3 = p2->next;
			p1->next = nullptr;
			while (p1&&p2&&p3) {
				p2->next = p1;
				if (notNULL(p3->next)) p1 = p3->next;
				else {
					head = p3;
					p3->next = p2;
					return;
				}
				p3->next = p2;
				if (notNULL(p1->next))p2 = p1->next;
				else {
					head = p1;
					p1->next = p3;
					return;
				}
				p1->next = p3;
				if(notNULL(p2->next))p3 = p2->next;
				else {
					head = p2;
					p2->next = p1;
					return;
				}
			}
		}

	}
	//Recursive Reversal
	void reverse(Node<T>* temp,Node<T>*itsprev=nullptr) {
		if (isEmpty())return;
		Node<T>* itsnext = temp->next;
		if (temp->next == nullptr) {
			temp->next = itsprev;
			this->head = temp;
			return;
		}
		temp->next = itsprev;
		itsprev = temp;
		reverse(itsnext, itsprev);
		//instead of the above two lines we can directly do reverse(itsnext, temp) without assigning itsprev=temp;
	}
	void REVERSE(Node<T>* node, Node<T>* itsPrev = nullptr) {
		if (isEmpty())return;
		if (!node)return;
		if (!node->next) {
			node->next = itsPrev;
			head = node;
			return;
		}
		Node<T>* nextnode = node->next;
		node->next = itsPrev;
		REVERSE(nextnode, node);
		
		
	}
	bool Delete(Node<T>*target) {
		Node<T>* prev=this->getHead();
		Node<T>* after=this->getHead()->getNext();
		if (!prev)return false;
		if (prev && !after) {
			delete prev;
			head = nullptr;
			return true;
		}

		while (prev) {
			if (prev->getNext() == target) {
				Node<T>* ptr = prev->getNext()->getNext();
				
			}
			prev = prev->next;
		}

		 }
};
