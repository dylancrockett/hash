#pragma once

#include "HashEntry.hpp"
#include <string>
#include <iostream>

//function used to help space out the table properly
std::string repeat_char(char c, int num) {
	std::string str = "";

	for (int i = 0; i < num; i++) {
		str += c;
	}

	return str;
}

template <typename KEY, typename VALUE, typename FUNCTION>
class HashTable
{
private:
	//pointer to the front of the hash table
	HashEntry<KEY, VALUE> ** table;

	//table values
	unsigned long table_size;

	//function used to hash keys
	FUNCTION hash_function;


public:
	//constructor
	HashTable(FUNCTION hashing_function, unsigned long size = 128) {
		//set the hash function
		this->hash_function = hashing_function;

		//set the table size
		this->table_size = size;

		//create a table
		table = new HashEntry<KEY, VALUE> *[this->table_size]();
	}

	~HashTable() {
		//delete all HashEntries
		for (int i = 0; i < this->table_size; ++i) {
			HashEntry<KEY, VALUE> * current = this->table[i];

			delete current;
			this->table[i] = nullptr;
		}

		//delete the table
		delete[] this->table;
	}

	//insert a key value pair into the table
	void insert(const KEY key, VALUE value) {
		//hash the key
		unsigned long hashed_key = this->hash_function(key, this->table_size);

		//get the existing entry at that location at the table
		HashEntry<KEY, VALUE> * entry = this->table[hashed_key];
		
		//if no entry at that index exists yet
		if (entry == nullptr) {
			//create new entry
			entry = new HashEntry<KEY, VALUE>;

			//add the new key and value to the entry
			entry->push(key, value);

			//add the new entry to the table
			this->table[hashed_key] = entry;
		}
		else {
			//if the entry already exists then just add the key value pair
			entry->push(key, value);
		}
	}

	//get a pointer to a value by providing a key
	VALUE * get(KEY key) {
		//return value
		VALUE * val = nullptr;

		//hash the key
		unsigned long hashed_key = this->hash_function(key, this->table_size);

		//get the existing entry at that location at the table
		HashEntry<KEY, VALUE> * entry = this->table[hashed_key];
			
		//get the value from the entry of the entry exists
		if (entry != nullptr) {
			//get the value from the entry, returns nullptr if the value key pair doesnt exist in the entry
			val = entry->get(key);
		}

		//return the value
		return  val;
	}

	bool remove(KEY key) {
		//hash the key
		unsigned long hashed_key = this->hash_function(key, this->table_size);

		//get the existing entry at that location at the table
		HashEntry<KEY, VALUE> * entry = this->table[hashed_key];

		//if no entry exists then return false
		if (entry == nullptr) {
			return false;
		}
		else {
			//if no node in the entry contains the value then return false
			return entry->remove(key);
		}
	}

	//returns the number of values in the table
	unsigned long size() {
		//counts the number of values
		unsigned long count = 0;

		//go through each entry and get it's size
		for (int i = 0; i < this->table_size; i++) {
			HashEntry<KEY, VALUE> * current_entry = this->table[i];

			//only count if the entry isnt nullptr
			if (current_entry != nullptr) {
				count += current_entry->size();
			}
		}

		return count;
	}

	//print the table to a given stream
	void print(std::ostream & stream, int key_max = 15, int value_max = 15) {
		int max_index_length = 7, max_key_length = key_max, max_value_length = value_max;

		//header and column labels
		stream << "+" << repeat_char('-', max_index_length) << "-" << repeat_char('-', max_key_length) << "-" << repeat_char('-', max_value_length) << "+\n";
		stream << "| Hash Table Printout " << repeat_char(' ', (max_index_length + max_key_length + max_value_length - 21 + 2)) << "|\n";
		stream << "+" << repeat_char('-', max_index_length) << "+" << repeat_char('-', max_key_length) << "+" << repeat_char('-', max_value_length) << "+\n";
		stream << "| Index " << "| Key " << repeat_char(' ', max_key_length - 6) << " | Value " << repeat_char(' ', max_value_length - 7) << "|\n";

		//table data
		for (int i = 0; i < this->table_size; i++) {
			HashEntry<KEY, VALUE> * current_entry = this->table[i];

			bool first = true;
			if (current_entry != nullptr) {
				HashNode<KEY, VALUE> ** nodes = current_entry->get_nodes();
				for (int j = 0; j < current_entry->size(); j++) {
					HashNode<KEY, VALUE> * current_node = nodes[j];

					//try to convert the index into a string
					std::string ind = std::to_string(this->hash_function(current_node->key, this->table_size));

					//if this is the first line then add the index and table chars
					if (first) {
						stream << "+" << repeat_char('-', max_index_length) << "+" << repeat_char('-', max_key_length) << "+" << repeat_char('-', max_value_length) << "+\n";
						stream << "|" << repeat_char(' ', max_index_length - ind.length()) << ind << "|" << repeat_char(' ', max_key_length - current_node->key.length()) << current_node->key << "|" << repeat_char(' ', max_value_length - current_node->value.length()) << current_node->value << "|\n";
						first = false;
					}
					else {
						stream << "|" << repeat_char(' ', max_index_length) << "|" << repeat_char(' ', max_key_length - current_node->key.length()) << current_node->key << "|" << repeat_char(' ', max_value_length - current_node->value.length()) << current_node->value << "|\n";
					}
				}
			}
		}

		//close table
		stream << "+" << repeat_char('-', max_index_length) << "+" << repeat_char('-', max_key_length) << "+" << repeat_char('-', max_value_length) << "+\n";
	}
};