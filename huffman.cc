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


void breakTree(HTree::tree_ptr_t tree){ // Takes a hierarchical tree and turns it into a non-hierarchical forest

}

HTree:tree_ptr_t buildTree(HForest forest){ // Takes a non-hierarchical forest and turns it into a hierarchical tree
	while(forest.size() != 1){ // Iterate until the forest has a single tree in it:

		// Find the top-two HTrees in the forest (those who have the lowest frequency count):
		HTree::tree_ptr_t treeOne = forest.pop_top();
		HTree::tree_ptr_t treeTwo = forest.pop_top();

		HTree::value_t newVal = treeOne->get_value() + treeTwo->get_value(); // The value of the new tree is the sum of the values of its two subtree.

		// Merge them as the left and right nodes of a new HTree node. The key in the new tree is a new, fake "symbol".
		forest.add_tree(HTree::tree_ptr_t(new HTree(-1, 1))) // "Organization leaves" have keys of -1
	}

	HTree::tree_ptr_t huffmanTree = forest.pop_top(); // Now, there is only one tree in the forest - our new Huffman tree!
	return huffmanTree;
}


Huffman::bits_t Huffman::encode(int symbol){
	if(myForest.size() == 0 && myTreePtr == nullptr){ // Base case: we have a forest with nothing in it, and we haven't assembled a tree
		myForest.add_tree(HTree::tree_ptr_t(new HTree(symbol, 1))); // Add a new tree to the forest.
		myTreePtr = buildTree(myForest); // Build tree.
	}else{
		HTree::possible_path_t path = myTreePtr->path_to(symbol); // Get directions to the symbol's leaf.

		HTree::tree_ptr_t symbolLeaf = myTreePtr; // Set symbolLeaf to the first leaf in the tree.
		for(HTree::Direction direction:*path){ // For each "direction" in path...
			symbolLeaf = symbolLeaf->get_child(direction); // Move symbolLeaf down the tree in that direction.
		} // Now, symbolLeaf should be at the symbol's leaf!

		 // Increment the leaf's value.
		HTree::value_t symbolValue = symbolLeaf->value_;
		symbolLeaf->updateValue();
		++symbolLeaf->value_;
	}

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
