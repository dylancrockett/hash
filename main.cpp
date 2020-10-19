#include "HashTable.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

vector<string> first_names = {"James", "John", "Robert", "Michael", "William", "David", "Richard", "Joseph", "Thomas", "Charles", "Christopher", "Daniel", "Matthew", "Anthony", "Donald", "Mark", "Paul", "Steven", "Andrew", "Kenneth", "Joshua", "George", "Kevin", "Brian", "Edward", "Mary", "Patricia", "Jenifer", "Linda", "Elizabeth", "Barbra", "Susan", "Jessica", "Sarah", "Karen", "Margaret", "Lisa", "Betty", "Dorothy", "Sandra", "Ashley", "Kimberly", "Donna", "Emily", "Michelle", "Carol", "Amanda", "Melissa", "Deborah"};
vector<string> last_names = {"Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Moore", "Taylor", "Hall", "Allen", "Young", "Hernandez", "Kind", "Wright", "Lopez", "Hill", "Scott", "Green", "Stewart", "Sanchez", "Morris", "Rogers", "Reed"};

//typedef for the hashing function
typedef unsigned long(*hash_function)(string, unsigned long);

//forward declares
string random_name();
string random_number();

//custom string hashing function
unsigned long hash_string(string key, unsigned long size) {
	unsigned long hash = 33;

	for (int i = 0; i < key.length(); i++) {
		hash = 33 * hash + (unsigned int)key[i];
	}

	return hash % size;
}

int main() {
	srand(time(nullptr));

	//create the hash table, given a hashing function and starting size of 100
	HashTable<string, string, hash_function> table(hash_string, 100);

	//add some values to the table
	for (int i = 0; i < 25; i++) {
		table.insert(random_name(), random_number());
	}

	//user response from cin
	string response = "";
	
	//interface while loop
	while (response != "6") {
		//clear screen
		system("cls");

		//print the hash table to the screen
		table.print(cout, 20, 25);

		//main menu
		cout << endl << "<> Hash Table Editor <>" << endl;
		cout << " [1] Insert a new value with a key." << endl;
		cout << " [2] Remove a value by key." << endl;
		cout << " [3] Search a value by key." << endl;
		cout << " [4] Add bulk random values." << endl;
		cout << " [5] Resize table." << endl;
		cout << " [6] Exit program." << endl;
		cout  << endl << "Please choose from one of the above options:" << endl;
		cout << "> "; getline(cin, response);

		//insert a new value
		if (response == "1") {
			system("cls");

			string key, value;

			//get the key
			cout << "Please enter the key for the new entry:" << endl << "> ";
			getline(cin, response); cout << endl << endl; key = response;

			//get the value
			cout << "Please enter the value to be inserted:" << endl;
			getline(cin, response); cout << endl << endl; value = response;

			//insert the value
			table.insert(key, value);

			cout << "Value inserted." << endl;
			system("pause");
		}
		else if (response == "2") {
			system("cls");

			//print table for refference
			table.print(cout, 20, 25);

			string key;

			//get the key
			cout << "Please enter the key of the entry to be removed:" << endl << "> ";
			getline(cin, response); cout << endl << endl; key = response;

			//remove value
			if (table.remove(key)) {
				cout << "Entry with key '" << key << "' was removed." << endl;
			}
			else {
				cout << "No entry with key '" << key << "' exists in the table." << endl;
			}

			system("pause");
		}
		else if (response == "3") {
			system("cls");

			//print table for refference
			table.print(cout, 20, 25);

			string key;

			//get the key
			cout << "Please enter the key of the entry to be searched:" << endl << "> ";
			getline(cin, response); cout << endl << endl; key = response;

			string * val = table.get(key);
			//check if search was sucessful
			if (val != nullptr) {
				cout << "Entry with key '" << key << "' has a value of '" << *val << "'." << endl;
			}
			else {
				cout << "No entry with key '" << key << "' exists in the table." << endl;
			}

			system("pause");
		}
		else if (response == "4") {
			system("cls");

			string number;

			//get the key
			cout << "Please enter the number of random entrys you would like to add (int only):" << endl << "> ";
			getline(cin, response); cout << endl << endl; number = response;

			//try to add random values, output error message if input is invalid
			try {
				int x = stoi(number);

				//add x number of random values
				for (int i = 0; i < x; i++) {
					table.insert(random_name(), random_number());
				}

				cout << "Added " << x << " new entries successfuly!" << endl;
			}
			catch (...) {
				cout << "Value given could not be converted to an integer." << endl;
			}

			system("pause");
		}
	}
}

//generates a random name from the first and last name lists
string random_name() {
	string name = "";

	//add the first name
	name += first_names[rand() % first_names.size()] + " ";

	//add the last name
	name += last_names[rand() % last_names.size()];

	return name;
}

//returns a random int from 0-9 as a string
string rand_int() {
	return to_string(rand() % 10);
}

//generates a random phone number
string random_number() {
	return (rand_int() + rand_int() + rand_int() + "-" + rand_int() + rand_int() + rand_int() + "-" + rand_int() + rand_int() + rand_int() + rand_int());
}