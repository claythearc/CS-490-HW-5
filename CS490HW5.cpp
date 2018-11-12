/*
Clayton Turner
CS 490, Homework 5
Dr. Weisskoph
*/
#include "stdafx.h"
#include <queue>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <fstream>
#pragma warning(disable:4996)

using namespace std;


int FIFO(int resident_size, int* dataset, int size) {
	int faults = 0; //Number of faults
	deque<int> Pages = deque<int>(); //FIFO queue to hold our data. a deque is nice here because it allows specific indexing
	if (resident_size == 3) { //if resident set size is 3, display the table headings.
		cout << "New Page | Page Replaced | Current Page List \n";
	}
	//loop over the dataset array.
	for (int i = 0; i < size; i++) {
		bool inQueue = false; //flag for if the element is in the queue or not.
		//Loop over the current Pages queue to see if the element is in the queue
		//If it is, set the flag.
		for (int page_idx = 0; page_idx < Pages.size(); page_idx++) {
			if (Pages[page_idx] == dataset[i]) {
				inQueue = true;
			}
		}
		if (!inQueue) {
			/*
			If the element is not in the queue, set our temp variable to * for pretty printing,
			check to see if Pages is over the RSS,
				if it is, pop the front off and store it in temp.
			Push the new page to the end
			If we're at #25 or less reference, print output for the table.
			Lastly, print the summary.
			*/
			string temp = "*";
			faults = faults++;
			if (Pages.size() == resident_size) {
				temp = to_string(Pages[0]);
				Pages.pop_front();
			}
			Pages.push_back(dataset[i]);
			if ((i <= 25) && (resident_size == 3)) {
				cout << dataset[i] << "\t\t" << temp << "\t\t";
				if (!Pages.empty()) {
					for (int pgs = 0; pgs < Pages.size(); pgs++) {
						cout << Pages[pgs] << " ";
					}
					cout << "\n";
				}
			}
		}

	}
	cout << "FIFO, RSS = " << resident_size << "\n";
	cout << "Page Faults: " << faults << " Page Fault Freq: " << faults / (double)size << "\n";

	return faults;
}

int LRU(int resident_size, int dataset[], int size) {
	int faults = 0; //number of faults
	//Print tbale heading if resident set size is 3.
	if (resident_size == 3) {
		cout << "New Page | Page Replaced | Current Page List \n";
	}
	//Queue to hold our information
	deque<int> Pages = deque<int>();
	for (int i = 0; i < size; i++) {
		//Flags for where the object is located in the queue
		//and if it's located period.
		bool inQueue = false;
		int PgIdx = -1;
		string temp = "*";
		//loop over queue, to try to find the element
		for (int page_idx = 0; page_idx < Pages.size(); page_idx++) {
			if (Pages[page_idx] == dataset[i]) {
				inQueue = true;
				PgIdx = page_idx;
			}
			
		}
		/*
		If it's not in the queue, check queue size
		pop off the front if necessary first,
		then push the latest entry to the back.
		also, print the table entry for the first 25 references.
		*/
		if (!inQueue) {
			faults++;
			if (Pages.size() == resident_size) {
				temp = to_string( Pages[0] );
				Pages.pop_front();
			}
			Pages.push_back(dataset[i]);
			if ((i <= 25) && (resident_size == 3)) {
				cout << dataset[i] << "\t\t" << temp << "\t\t";
				if (!Pages.empty()) {
					for (int pgs = 0; pgs < Pages.size(); pgs++) {
						cout << Pages[pgs] << " ";
					}
					cout << "\n";
				}
			}
		}
		//If it is in the queue, remove it from the list, and put it back in the back.
		if (inQueue) {
			Pages.erase(Pages.begin() + PgIdx);
			Pages.push_back(dataset[i]);
		}
	}
	//Print summary.
	cout << "LRU, RSS = " << resident_size << "\n";
	cout << "Page Faults: " << faults << " Page Fault Freq: " << faults / (double)size << "\n";
	return faults;
}

int main()
{
	/*
	Driver Function to run the comparisons.
	*/
	
	//Build our dataset for dataset 1, and the resident set size array.
	
	int dataset[33] = { 1, 1, 1, 1, 0, 3, 1, 1, 3, 5, 1, 8, 1, 3, 5, 13, 15, 6, 1, 1, 3, 6, 7, 8, 9, 3, 1, 1, 4, 4, 4, 1, 2 };
	int RSS[3] = { 3,5,7 };
	
	vector<int> fifo_results;
	vector<int> lru_results;
	//Foreach to call the functions for each element in RSS.
	cout << "Dataset 1: \n";
	for (int iter : RSS) {
		fifo_results.push_back( FIFO(iter, dataset, sizeof(dataset) / sizeof(int)));
		lru_results.push_back( LRU(iter, dataset, sizeof(dataset) / sizeof(int)));
		system("pause");
	}
	
	//Read the data file
	vector<int> datasettwo = vector<int>();
	ifstream DataFile("FA18PageData.txt");
	string line;
	if (DataFile.is_open()) {
		while (getline(DataFile, line, ' ')) {
			try {
				int current = stoi(line);
				datasettwo.push_back(current);
			}
			catch (invalid_argument& e) {
				//Not an int.
			}

		}
	}
	DataFile.close();
	cout << "Dataset 2: \n";
	vector<int> fifo2_results;
	vector<int> lru2_results;
	for (auto iter : RSS) {
		fifo2_results.push_back( FIFO(iter, &datasettwo[0], datasettwo.size()));
		lru2_results.push_back( LRU(iter, &datasettwo[0], datasettwo.size()));
		system("pause");
	}
	cout << "Dataset 1: \n";
	cout << "RSS | FIFO Faults | FIFO PFF | LRU Faults | LRU PFF \n";
	for (int i = 0; i < sizeof(RSS) / sizeof(int); i++) {
		cout << RSS[i] << "\t" << fifo_results[i] << "\t" << fifo_results[i] / (double)(sizeof(dataset) / sizeof(int)) << "\t" << lru_results[i] << "\t" << lru_results[i] / (double)(sizeof(dataset) / sizeof(int)) << "\n";
	}

	cout << "Dataset 2: \n";
	cout << "RSS | FIFO Faults | FIFO PFF | LRU Faults | LRU PFF \n";
	for (int i = 0; i < sizeof(RSS) / sizeof(int); i++) {
		cout << RSS[i] << "\t" << fifo2_results[i] << "\t" << fifo2_results[i] / (double)datasettwo.size() << "\t" << lru2_results[i] << "\t" << lru2_results[i] / (double)datasettwo.size() << "\n";
	}
	system("pause");

    return 0;
}







