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
		return T();
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
			SNode<T>* current = head;
			while (current->next != nullptr && current->next->priority >= prio) {
				current = current->next;
			}
			newNode->next = current->next;
			current->next = newNode;
			size++;
		}
	}
	void Dequeue() {
		SNode<T>* temp = head;
		head = head->next;
		delete temp;
		size--;
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
	int getPriority() const {
		return priority;
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
	T Peek() {
		if (size == 0) {
			return T();
		}
		int maxPrio = arr[0].getPriority();
		int maxIndex = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (arr[i].getPriority() > maxPrio) {
				maxPrio = arr[i].getPriority();
				maxIndex = i;
			}
		}
		return arr[maxIndex].getValue();
	}
	unsigned int getSize() const {
		return size;
	}
	void Enqueue(T val, int prio) {
		if (size == capacity) {
			resizeAndCopy();
		}
		arr[size] = DArrElement<T>(val, prio);
		size++;
	}
	void Dequeue() {
		if (size == 0) {
			return;
		}
		int maxPrio = arr[0].getPriority();
		int maxIndex = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (arr[i].getPriority() > maxPrio) {
				maxPrio = arr[i].getPriority();
				maxIndex = i;
			}
		}
		for (unsigned int i = maxIndex; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
	}
	void modifyNodePriorityOfGivenValue(T val, int new_prio) {
		for (unsigned int i = 0; i < size; i++) {
			if (arr[i].getValue() == val) {
				arr[i] = DArrElement<T>(val, new_prio);
				break;
			}
		}
	}
private:
	DArrElement<T>* arr;
	unsigned int size;
	unsigned int capacity;
};

int main() {
	srand(time(NULL)); // Inicjalizacja generatora liczb losowych
	const unsigned int iteracje = 10000;

	const string sortingResultsFiles[] = {
		"SLinkedListPriorityQueue_results.csv",
		"DArrayPriorityQueue_results.csv",
	};
	const int sortingResultsFilesAmount = sizeof(sortingResultsFiles) / sizeof(sortingResultsFiles[0]); // Obliczenie liczby plikow


	long long sortTimes[5][iteracje];
	
	SLinkedListPriorityQueue<int> listQueue;
	DArrayPriorityQueue<int> arrayQueue;

	ofstream file;

	for (int i = 0; i < sortingResultsFilesAmount; i++) {

		file.open(sortingResultsFiles[i]);

		if (!file.is_open()) {
			cerr << "Nie mozna otworzyc pliku: " << sortingResultsFiles[i] << endl;
			return 1;
		}

		file << "Iteracja, Dodawanie elementu, Usuwanie elementu, Zwracanie rozmiaru kolejki, Peek, Modyfikacja priorytetu określonego elementu";
		file << endl;
		cout << "\nNaglowek napisany dla pliku: " << sortingResultsFiles[i] << endl;

		switch (i)
		{
		case 0:
			for (int j = 1; j <= iteracje; j++) {
				auto start = chrono::high_resolution_clock::now();
				listQueue.Enqueue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[0][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				listQueue.Dequeue();
				end = chrono::high_resolution_clock::now();
				sortTimes[1][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				listQueue.getSize();
				end = chrono::high_resolution_clock::now();
				sortTimes[2][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				listQueue.Peek();
				end = chrono::high_resolution_clock::now();
				sortTimes[3][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
			}
			for (int k = 1; k <= iteracje; k++) {
				auto start = chrono::high_resolution_clock::now();
				listQueue.modifyNodePriorityOfGivenValue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[4][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
			}
			break;

		case 1:
			for (int j = 1; j <= iteracje; j++) {
				auto start = chrono::high_resolution_clock::now();
				arrayQueue.Enqueue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[0][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				arrayQueue.Dequeue();
				end = chrono::high_resolution_clock::now();
				sortTimes[1][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				arrayQueue.getSize();
				end = chrono::high_resolution_clock::now();
				sortTimes[2][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				arrayQueue.Peek();
				end = chrono::high_resolution_clock::now();
				sortTimes[3][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
			}
			for (int k = 1; k <= iteracje; k++) {
				auto start = chrono::high_resolution_clock::now();
				arrayQueue.modifyNodePriorityOfGivenValue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[4][j] = chrono::duration_cast<chrono::microseconds>(end - start).count();
			}
			break;

		default:
			break;
		}


		cout << "Zapisuje wyniki do pliku: " << sortingResultsFiles[i] << endl;

		for (int j = 0; j < iteracje; j++) {
			file << j + 1 << ","; // Iteracja
			file << sortTimes[0][j] << ","; // Dodawanie elementu
			file << sortTimes[1][j] << ","; // Usuwanie elementu
			file << sortTimes[2][j] << ","; // Zwracanie rozmiaru kolejki
			file << sortTimes[3][j] << ","; // Peek
			file << sortTimes[4][j] << ","; // Modyfikacja priorytetu
			file << endl;
		}

		file.close();

	}
	return 0;
}