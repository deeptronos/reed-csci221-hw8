#include <memory>

#include "huffman.hh"


#include <iostream>

/*The algorithm for encoding a character (symbol) c is summarized as follows:
 * Build a Huffman tree from scratch, given the existing frequency table (see below).
 * Find the path to c in the Huffman tree from the root.
 * Convert the path of LEFTs and RIGHTs to zeros and ones, which will be returned as a sequence of bits (type bits_t).
 * Increment the frequency table by adding one to c's count.
 */

/*Algorithm to build a Huffman Tree from scratch:
 * Create one new HTree node for every possible character + EOF (257 total). Each node will have its symbol (character) as the key, and its count from the frequency table as a value.
 * Add all 257 HTrees to a new HForest.
 * Iterate until the forest has a single tree in it:
    * Find the top-two HTrees in the forest (those who have the lowest frequency count).
    * Merge them as the left and right nodes of a new HTree node. The key in the new tree is a new, fake "symbol" (you can use negative keys for these, or values > 257, but the former is simpler). The value of the new tree is the sum of the values of its two subtree.
 * At this point, your forest includes a single HTree, which is your Huffman tree.
*/
//Return a bits_t sequence of bits translated from the passed HTree::path_t
Huffman::bits_t pathToBits(const HTree::path_t& path){
	Huffman::bits_t pathBits;

	for(HTree::Direction dir:path){ // Ranged for-loop which navigates our path_t path
		if(dir == HTree::Direction::LEFT){
			pathBits.push_back(false); // LEFT = 0 = false
		}else if(dir == HTree::Direction::RIGHT){
			pathBits.push_back(true); // RIGHT = 1 = true
		}
	}
	return pathBits; //pathBits now contains booleans which correspond to the passed path_t path!
}


// Creates a forest full of "leaves" (unconnected tree nodes) that have key_'s which correspond to 0 through int size
HForest initSymbolForest(int size){
	HForest initForest;
	for(int currSymbol = 0; currSymbol <= size; ++currSymbol){
		initForest.add_tree(HTree::tree_ptr_t(new HTree(currSymbol, 0))); // add a new tree for each symbol to the forest, with "frequencies" (values) of zero
	}
	return initForest;
}

// UNTESTED
HForest breakTree(HTree::tree_ptr_t tree){ // Takes a hierarchical tree and turns it into a non-hierarchical forest
	HForest newForest;
	HTree::tree_ptr_t currLeaf = tree;

	while(currLeaf->left_ != nullptr && currLeaf->right_ != nullptr){ // Doesn't work, but only(?) bc buildTree() is written incorrectly
		if(currLeaf->left_->get_key() == -1){ // Organization leaf (leaves with key=-1) in left_ means a symbol leaf in right_
			newForest.add_tree(currLeaf->right_); // add the symbol leaf to the forest
			currLeaf->right_    = nullptr; // remove the symbol leaf from the tree
			currLeaf            = currLeaf->left_; // Move on to the next organization leaf

		}else if(currLeaf->right_->get_key() == -1){ // Organization leaf (leaves with key=-1) in right_ means a symbol leaf in left_
			newForest.add_tree(currLeaf->left_); // add the symbol leaf to the forest
			currLeaf->left_ = nullptr; // remove the symbol leaf from the tree
			currLeaf        = currLeaf->right_; // Move on to the next organization leaf

		}else if(currLeaf->left_->get_key() != -1 &&  currLeaf->right_->get_key() != -1){ // No organization leaves in children means that we're at the final organization leaf, and both of our children are (or, should be?) symbol leaves.
			// Add both symbol leaves to the forest
			newForest.add_tree(currLeaf->left_);
			newForest.add_tree(currLeaf->right_);
			// Remove them from the tree
			currLeaf->left_ = nullptr;
			currLeaf->right_= nullptr;

		}
	}
	tree = nullptr; // Set tree to not point to anything, since all that's left in the tree is organization leaves, and we've rendered it useless.

	return newForest;
}

// UNTESTED
HTree::tree_ptr_t buildTree(HForest forest){ // Takes a non-hierarchical forest and turns it into a hierarchical tree
	// PROBLEM: pop_top always pops the organization nodes (bc they have key = -1?)

	while(forest.size() != 1){ // Iterate until the forest has a single tree in it:

		// Find the top-two HTrees in the forest (those who have the lowest frequency count):
		HTree::tree_ptr_t treeOne = forest.pop_top();
		HTree::tree_ptr_t treeTwo = forest.pop_top();

		HTree::value_t newVal = treeOne->get_value() + treeTwo->get_value(); // The value of the new tree is the sum of the values of its two subtree.

		// Merge them as the left and right nodes of a new HTree node. The key in the new tree is a new, fake "symbol".
		forest.add_tree(HTree::tree_ptr_t(new HTree(-1, newVal, treeOne, treeTwo))); // "Organization leaves" have keys of -1
	}

	HTree::tree_ptr_t huffmanTree = forest.pop_top(); // Now, there is only one tree in the forest - our new Huffman tree!
	assert(forest.size() == 0); // Make sure that the forest is empty tho
	return huffmanTree;
}

// MOSTLY UNTESTED
Huffman::bits_t Huffman::encode(int symbol){
	if(myForest.size() == 0 && myTreePtr == nullptr){ // Base case: we have an empty forest, and we haven't assembled a tree
		//myForest.add_tree(HTree::tree_ptr_t(new HTree(symbol, 1))); // Add a new tree to the forest.
		myForest = initSymbolForest(ALPHABET_SIZE); // create a new forest full of symbol nodes with frequencies of 0

		myTreePtr = buildTree(myForest); // Build tree.

	}

	HTree::possible_path_t myPath = myTreePtr->path_to(symbol); // Get directions to the symbol's leaf.
	Huffman::bits_t returnable = pathToBits(*myPath); //TEMP

	HTree::tree_ptr_t symbolLeaf = myTreePtr; // Set symbolLeaf to the first leaf in the tree.

	for(HTree::Direction direction:*myPath){ // For each "direction" in path...
		symbolLeaf = symbolLeaf->get_child(direction); // Move symbolLeaf down the tree in that direction.
	} // Now, symbolLeaf should be at the symbol's leaf!

	 // Increment the leaf's value.
	HTree::value_t symbolValue = symbolLeaf->value_;
	++symbolValue; // Add 1 to symbolValue // NOTE: will this actually increment the value_t at symbolLeaf->value_? make sure it doesn't!!!
	symbolLeaf->setValue(symbolValue);

	//Break tree down into forest
	myForest = breakTree(myTreePtr);
	//Re-build tree
	myTreePtr = buildTree(myForest);

	return returnable;  // HOMIE LMAO
}

int Huffman::decode(bool bit) {
	return bit;
}

Huffman::Huffman() {

}

Huffman::~Huffman() {

}

// encode: finds a key _> update the value (++Value) _> break tree _> build tree
// break_tree - takes a hierarchical tree and turns it into a non-hierarchical forest
// build_tree - takes a non-hierarchical forest and turns it into a hierarchical tree
