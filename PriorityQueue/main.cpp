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
		if (!head) return;

		SNode<T>* prev = nullptr;
		SNode<T>* current = head;

		while (current && current->value != val) {
			prev = current;
			current = current->next;
		}

		if (!current) return; // Nie znaleziono węzła o danej wartości

		if (prev) {
			prev->next = current->next;
		}
		else {
			head = current->next;
		}
		current->next = nullptr;

		current->priority = new_prio;

		// Wstawiamy zmieniony węzeł w odpowiedniej pozycji
		if (!head || head->priority < new_prio) {
			current->next = head;
			head = current;
		}
		else {
			SNode<T>* insertPos = head;
			while (insertPos->next && insertPos->next->priority >= new_prio) {
				insertPos = insertPos->next;
			}
			current->next = insertPos->next;
			insertPos->next = current;
		}
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
	SNode<T>* head;
	unsigned int size;
};

template<typename T>
class DArrElement {
public:
	DArrElement() : value(T()), priority(0) {}
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

	// Tablica do przechowywania czasow sortowania  
    long long** sortTimes = new long long*[5];  
    for (int i = 0; i < 5; i++) {  
        sortTimes[i] = new long long[iteracje];  
    }
	
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
			for (int j = 0; j < iteracje; j++) {
				auto start = chrono::high_resolution_clock::now();
				listQueue.Enqueue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[0][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				listQueue.getSize();
				end = chrono::high_resolution_clock::now();
				sortTimes[2][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				listQueue.Peek();
				end = chrono::high_resolution_clock::now();
				sortTimes[3][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (j == iteracje / 4) {
					cout << "Dodano 25% elementow do kolejki" << endl;
				}
				else if (j+1 == iteracje / 2) {
					cout << "Dodano 50% elementow do kolejki" << endl;
				}
				else if (j+1 == (3 * iteracje) / 4) {
					cout << "Dodano 75% elementow do kolejki" << endl;
				}
				else if (j+1 == iteracje) {
					cout << "Dodano 100% elementow do kolejki" << endl;
				}
			}
			for (int k = 0; k < iteracje; k++) {
				auto start = chrono::high_resolution_clock::now();
				listQueue.modifyNodePriorityOfGivenValue(k, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[4][k] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (k+1 == iteracje / 4) {
					cout << "Zmieniono priorytet 25% elementow w kolejce" << endl;
				}
				else if (k+1 == iteracje / 2) {
					cout << "Zmieniono priorytet 50% elementow w kolejce" << endl;
				}
				else if (k+1 == (3 * iteracje) / 4) {
					cout << "Zmieniono priorytet 75% elementow w kolejce" << endl;
				}
				else if (k+1 == iteracje) {
					cout << "Zmieniono priorytet 100% elementow w kolejce" << endl;
				}
			}
			for (int l = 0; l < iteracje; l++) {
				auto start = chrono::high_resolution_clock::now();
				listQueue.Dequeue();
				auto end = chrono::high_resolution_clock::now();
				sortTimes[1][l] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (l+1 == iteracje / 4) {
					cout << "Usunieto 25% elementow z kolejki" << endl;
				}
				else if (l+1 == iteracje / 2) {
					cout << "Usunieto 50% elementow z kolejki" << endl;
				}
				else if (l+1 == (3 * iteracje) / 4) {
					cout << "Usunieto 75% elementow z kolejki" << endl;
				}
				else if (l+1 == iteracje) {
					cout << "Usunieto 100% elementow z kolejki" << endl;
				}
			}
			break;

		case 1:
			for (int j = 0; j < iteracje; j++) {
				auto start = chrono::high_resolution_clock::now();
				arrayQueue.Enqueue(j, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[0][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				arrayQueue.getSize();
				end = chrono::high_resolution_clock::now();
				sortTimes[2][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				start = chrono::high_resolution_clock::now();
				arrayQueue.Peek();
				end = chrono::high_resolution_clock::now();
				sortTimes[3][j] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (j+1 == iteracje / 4) {
					cout << "Dodano 25% elementow do kolejki" << endl;
				}
				else if (j+1 == iteracje / 2) {
					cout << "Dodano 50% elementow do kolejki" << endl;
				}
				else if (j+1 == (3 * iteracje) / 4) {
					cout << "Dodano 75% elementow do kolejki" << endl;
				}
				else if (j+1 == iteracje) {
					cout << "Dodano 100% elementow do kolejki" << endl;
				}
			}
			for (int k = 0; k < iteracje; k++) {
				auto start = chrono::high_resolution_clock::now();
				arrayQueue.modifyNodePriorityOfGivenValue(k, rand() % 100);
				auto end = chrono::high_resolution_clock::now();
				sortTimes[4][k] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (k+1 == iteracje / 4) {
					cout << "Zmieniono priorytet 25% elementow w kolejce" << endl;
				}
				else if (k+1 == iteracje / 2) {
					cout << "Zmieniono priorytet 50% elementow w kolejce" << endl;
				}
				else if (k+1 == (3 * iteracje) / 4) {
					cout << "Zmieniono priorytet 75% elementow w kolejce" << endl;
				}
				else if (k+1 == iteracje) {
					cout << "Zmieniono priorytet 100% elementow w kolejce" << endl;
				}
			}
			for (int l = 0; l < iteracje; l++) {
				auto start = chrono::high_resolution_clock::now();
				arrayQueue.Dequeue();
				auto end = chrono::high_resolution_clock::now();
				sortTimes[1][l] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (l+1 == iteracje / 4) {
					cout << "Usunieto 25% elementow z kolejki" << endl;
				}
				else if (l+1 == iteracje / 2) {
					cout << "Usunieto 50% elementow z kolejki" << endl;
				}
				else if (l+1 == (3 * iteracje) / 4) {
					cout << "Usunieto 75% elementow z kolejki" << endl;
				}
				else if (l+1 == iteracje) {
					cout << "Usunieto 100% elementow z kolejki" << endl;
				}
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