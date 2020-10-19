#pragma once

//node storing an pair of data in the entry
template <typename KEY, typename VALUE>
struct HashNode {
	HashNode * front = nullptr;
	HashNode * back = nullptr;

	//key and value of the node
	KEY key;
	VALUE value;

	//Node constructor
	HashNode(KEY key, VALUE value) {
		this->key = key;
		this->value = value;
	}

	//Node deconstructor - allows lists constructed with nodes to automatically delete when the list is deconstructed
	~HashNode() {
		if (this->back != nullptr) {
			delete this->back;
		}
	}
};

//entrys are similar to rows of the table, each is a doubly linked list so it can store many values
template<typename KEY, typename VALUE>
class HashEntry
{
private:
	//return a pointer to the node which has a matching key
	HashNode<KEY, VALUE> * find_node(KEY key) {
		HashNode<KEY, VALUE> * current_node = this->begin_node;

		//traverse the linked list until we reach the end of the list
		while (current_node != nullptr) {
			//check if the current node's key is equal to the key we are looking for
			if (current_node->key == key) {
				return current_node;
			}
			else {
				current_node = current_node->back;
			}
		}

		//if no matching node was found return a nullptr
		return nullptr;
	}

public:
	//pointers to the node at the front and the back of the linked list
	HashNode<KEY, VALUE> * begin_node = nullptr;
	HashNode<KEY, VALUE> * end_node = nullptr;

	//constructor and destructor
	HashEntry() {}

	~HashEntry() {
		//start cascading deletion of the list
		delete begin_node;
	}

	//add a value to the front of the LinkedList
	void push(KEY key, VALUE value) {
		HashNode<KEY, VALUE> * new_node = new HashNode<KEY, VALUE>(key, value);

		//case if there are no existing nodes
		if (this->begin_node == nullptr) {
			this->begin_node = new_node;
			this->end_node = new_node;
		}
		else {
			//point this new node's back node to the previous front node
			new_node->back = this->begin_node;
			this->begin_node->front = new_node;

			//set the new node as the new begining node
			this->begin_node = new_node;
		}
	}

	//get the pointer to a value given a key
	VALUE * get(KEY key) {
		VALUE * val = nullptr;
		HashNode<KEY, VALUE> * node = this->find_node(key);
		
		if (node != nullptr) {
			val = &node->value;
		}

		return val;
	}

	//delete a node based on the key of the node
	bool remove(KEY key) {
		HashNode<KEY, VALUE> * node = find_node(key);

		//if the pointer passed is a nulltpr then return false since no node with the given key exists
		if (node == nullptr) {
			return false;
		}

		HashNode<KEY, VALUE> * node_front = node->front;
		HashNode<KEY, VALUE> * node_back = node->back;

		//if the node we found is the only node in the list then set both the front and end node to null and delete the node
		if (node == this->begin_node && node == this->end_node) {
			this->begin_node = nullptr;
			this->end_node = nullptr;

			//set the front and back to nullptr to prevent cascading node deletion
			node->front = nullptr;
			node->back = nullptr;

			//delete the node
			delete node;
			return true;
		}
		//if the node to be deleted is the begining node of the LinkedList
		else if (node == this->begin_node) {
			//set the node behind the node to be deleted to the new begining node and set that node's new front node to null
			this->begin_node = node->back;
			this->begin_node->front = nullptr;

			//set the front and back to nullptr to prevent cascading node deletion
			node->front = nullptr;
			node->back = nullptr;

			//delete the node
			delete node;
			return true;
		}
		//if the node to be deleted is the end node of the LinkedList
		else if (node == this->end_node) {
			//set the node in front the node to be deleted to the new bend node and set that node's new end node to null
			this->end_node = node->front;
			this->end_node->back = nullptr;

			//set the front and back to nullptr to prevent cascading node deletion
			node->front = nullptr;
			node->back = nullptr;

			//delete the node
			delete node;
			return true;
		}
		//no special conditons just remove the node
		else {
			node_front->back = node_back;
			node_back->front = node_front;

			//set the front and back to nullptr to prevent cascading node deletion
			node->front = nullptr;
			node->back = nullptr;

			//delete the node
			delete node;
			return true;
		}

		return false;
	}

	//get the size of the linked list
	int size() {
		int count = 0;
		HashNode<KEY, VALUE> * current_node = this->begin_node;

		//traverse the linked list until we reach the end of the LinkedList
		while (current_node != nullptr) {
			count++;
			current_node = current_node->back;
		}

		return count;
	}

	//return the nodes of the hash table in an array
	HashNode<KEY, VALUE> ** get_nodes() {
		//allocate space in memeory for the array
		HashNode<KEY, VALUE> ** nodes = new HashNode<KEY, VALUE>*[this->size()];

		HashNode<KEY, VALUE> * current_node = this->begin_node;

		//populate the array
		for (int i = 0; i < this->size(); i++) {
			nodes[i] = current_node;
			current_node = current_node->back;
		}

		//return the array
		return nodes;
	}
};