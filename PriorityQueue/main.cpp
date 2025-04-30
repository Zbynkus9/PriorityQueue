#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

template<typename T>
class SNode {
public:
	SNode(T val, int prio) : value(val), priority(prio), next(nullptr) {}
private:
	T value;
	int priority;
	SNode* next;
	template<typename> friend class SLinkedListPriorityQueue
		;
};

template<typename T>
class SLinkedListPriorityQueue {
public:
	SLinkedListPriorityQueue() : head(nullptr), size(0) {}
	~SLinkedListPriorityQueue() {
		while (head) {
			SNode<T>* temp = head;
			head = head->next;
			delete temp;
		}
	}
	T Peek() const {
		if (head) {
			return head->value;
		}
	}
	unsigned int getSize() const {
		return size;
	}
	void Enqueue(T val, int prio) {
		SNode<T>* newNode = new SNode<T>(val, prio);
		if (!head || head->priority < prio) {
			newNode->next = head;
			head = newNode;
			size++;
		}
		else {
			SNode<T>* current = head->next;
			while (currnet->next != nullptr) {
				if (current->next->priority <= prio) {
					newNode->next = current->next;
					current->next = newNode;
					size++;
					return;
				}
				current = current->next;
			}
			newNode->next = current->next;
			current->next = newNode;
			size++;
			return;
		}
	}
	void Dequeue() {
		SNode<T>* temp = head;
		head = head->next;
		delete temp;
	}
	void modifyNodePriorityOfGivenValue(T val, int new_prio) {
		SNode<T>* current = head;
		SNode<T>* preReInsertPlace = nullptr;
		SNode<T>* nodeForChange = nullptr;
		if (head->priority < new_prio) preReInsertPlace = head;
		while (current && !(preReInsertPlace && nodeForChange)) {
			if (current->next->priority < new_prio && preReInsertPlace == nullptr) {
				preReInsertPlace = current;
			}
			if (current->next->value == val && nodeForChange == nullptr) {
				nodeForChange = current->next;
				if (current->priority < new_prio) {
					current->next = nodeForChange->next;
				}
				else if (nodeForChange->next != nullptr) {
					if (nodeForChange->next->priority > new_prio) {
						current->next = nodeForChange->next;
					}
				}
			}
			current = current->next;
		}
		nodeForChange->next = preReInsertPlace->next;
		preReInsertPlace->next = nodeForChange;
	}

	void clearList() {
		while (head) {
			SNode<T>* temp = head;
			head = head->next;
			delete temp;
		}
		size = 0;
	}	
private:
	SNode<T>* head
	unsigned int size;
};

template<typename T>
class DArrElement {
public:
	DArrElement() : value(T()) {}
	DArrElement(T val, int prio) : value(val), priority(prio) {}
	T getValue() const {
		return value;
	}
private:
	T value;
	int priority;
};


template<typename T>
class DArrayPriorityQueue {
public:
	DArrayPriorityQueue(unsigned int cap = 10) : size(0), capacity(cap) {
		arr = new DArrElement<T>[capacity];
	}
	~DArrayPriorityQueue() {
		delete[] arr;
	}
	void resizeAndCopy() {
		capacity *= 2;
		DArrElement<T>* newArr = new DArrElement<T>[capacity];
		for (unsigned int i = 0; i < size; i++) {
			newArr[i] = arr[i];
		}
		delete[] arr;
		arr = newArr;
	}
	void clearArray() {
		size = 0;
	}
private:
	DArrElement<T>* arr;
	unsigned int size;
	unsigned int capacity;
};

int main() {
	return 0;
}