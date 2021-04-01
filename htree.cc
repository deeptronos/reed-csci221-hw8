/*
 * HTree: a class to represent a tree node with two values: a key
 * and a value.
 */

#include "htree.hh"
using namespace std;



//Constructor
HTree::HTree(	HTree::key_t key,
		HTree::value_t value,
		HTree::tree_ptr_t left,
		HTree::tree_ptr_t right){
	key_ = key;
	value_ = value;
	left_ = left;
	right_ = right;
}



HTree::key_t HTree::get_key() const {
// Method to return key of a node
	return key_;
}
HTree::value_t HTree::get_value() const {
// Method to return value of a node
	return value_;
}


HTree::tree_ptr_t HTree::get_child(HTree::Direction dir) const {
// Returns the child of a given node
 
	if (dir == HTree::Direction::LEFT){ //checks input
		return left_;
	}
        if (dir == HTree::Direction::RIGHT){
		return right_;

	}
    return nullptr; //if bad input, returns nullptr
}



HTree::possible_path_t HTree::path_to(key_t key) const 
// Returns the path to a node with a given key.

{
 if (get_key()==key){ //base case, found key
	 return possible_path_t(new path_t()); //creates a pointer to a new list
 }
 if (get_child(Direction::LEFT)){ //Checks if left exists
 	auto left_path = get_child(Direction::LEFT)->path_to(key); //recursive search left child
 	if (left_path!=nullptr){ //Checks if there is a path
		left_path->push_front(Direction::LEFT); //prepends to the list
 		return left_path; 
 	}
 }
 if (get_child(Direction::RIGHT)){ //checks if right child exists
	auto right_path = get_child(Direction::RIGHT)->path_to(key);
 	if (right_path!=nullptr){ 
		right_path->push_front(Direction::RIGHT);
	 	return right_path;
 	}
 }

return nullptr; // if nothing found, returns nullptr
}

