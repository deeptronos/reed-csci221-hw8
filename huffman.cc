#include <memory>

#include "huffman.hh"


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

//NOTE: while the logic that bitToDirection() executes is fairly trivial and could have easily been implemented within the methods where it is used, it is declared and implemented as a discrete function here, following pathToBits(), so that I can ensure consistency between the logic of the two functions.
		// I.E., it ensures that I don't mix-up which bool corresponds to which HTree::Direction.

// Returns the HTree::Direction corresponding to the passed bool bit
HTree::Direction bitToDirection(bool bit){
	if(!bit){ // false = 0 = LEFT
		return HTree::Direction::LEFT;
	}else{ // true = 1 = RIGHT
		return HTree::Direction::RIGHT;
	}
}

// Creates a forest full of "leaves" (unconnected tree nodes) that have key_'s which correspond to 0 through int size
HForest initSymbolForest(int size){
	HForest initForest;
	for(int currSymbol = 0; currSymbol <= size; ++currSymbol){
		initForest.add_tree(HTree::tree_ptr_t(new HTree(currSymbol, 0))); // add a new tree for each symbol to the forest, with "frequencies" (values) of zero
	}
	return initForest;
}

// Takes a hierarchical tree and turns it into a non-hierarchical forest
HForest breakTree(HTree::tree_ptr_t tree){
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

// Takes a non-hierarchical forest and turns it into a hierarchical tree
HTree::tree_ptr_t buildTree(HForest& forest){

	std::shared_ptr<HTree> treeNav(nullptr); // a pointer we can use for traversing myTree, since we wanna keep track of the root to return it at the end of the function.

	while(forest.size() > 0){
		if(treeNav == nullptr){ // base case
			// Find the top-two HTrees in the forest (those who have the lowest frequency count):
			HTree::tree_ptr_t treeOne = forest.pop_top();
			HTree::tree_ptr_t treeTwo = forest.pop_top();
			HTree::value_t newVal = treeOne->get_value() + treeTwo->get_value(); // The value of the new tree is the sum of the values of its two subtree.

			treeNav = HTree::tree_ptr_t(new HTree(-1, newVal, treeOne, treeTwo)); // Move treeNav to point to a new organizational HTree, with treeOne and treeTwo as its left_ and right_, respectively.
		}else{
			HTree::tree_ptr_t topTree = forest.pop_top(); // Pop the HTree with the lowest frequency count
			treeNav = HTree::tree_ptr_t(new HTree(-1, (treeNav->value_ + topTree->value_), treeNav, topTree)); // Move treeNav to point to a new organizational HTree, which has a value equal to the sum of a its subtrees, and whose subtrees are topTree and the HTree pointed to by the previous treeNav
		}

	}

	assert(forest.size() == 0);
	return treeNav;

}

Huffman::bits_t Huffman::encode(int symbol){
	if(myForest.size() == 0 && myTreePtr == nullptr){ // Base case: we have an empty forest, and we haven't assembled a tree

		myForest = initSymbolForest(ALPHABET_SIZE); // create a new forest full of symbol nodes with frequencies of 0

		myTreePtr = buildTree(myForest); // Build tree.

	}

	HTree::possible_path_t myPath = myTreePtr->path_to(symbol); // Get directions to the symbol's leaf.
	Huffman::bits_t returnable = pathToBits(*myPath); //TEMP

	HTree::tree_ptr_t symbolLeaf = myTreePtr; // Set symbolLeaf to the first leaf in the tree.

	for(HTree::Direction direction:*myPath){ // For each "direction" in path...
		symbolLeaf = symbolLeaf->get_child(direction); // Move symbolLeaf down the tree in that direction.
	} // Now, symbolLeaf should be at the symbol's leaf!

	 // Increment the leaf's value.     //  THIS CAN BE SIMPLIFIED
	HTree::value_t symbolValue = symbolLeaf->value_;
	++symbolValue;
	symbolLeaf->setValue(symbolValue);

	//Break tree down into forest.
	myForest = breakTree(myTreePtr);
	//Re-build tree.
	myTreePtr = buildTree(myForest);

	return returnable;
}

/*
 * the algorithm for decoding a single bit b at a time into a symbol works as follows:

    * If this is the first bit in a sequence (Huffman tree not yet computed), build a Huffman tree from scratch, given the existing frequency table (see below), and start from the root node.
    * Otherwise, start from the previous node you've computed.
    * Compute the next node as either the left or right child of the current node, depending on the bit value.
        * If this is an intermediate node (not a leaf), you will return a negative number for a symbol. Otherwise, you're a leaf, pointing to an actual symbol, which you will return.
    * Eventually, the current node will be a leaf, at which point a real character c will be returned, and you can reset the Huffman tree to nullptr to remember that next bit starts a new sequence. Additionally, increment the frequency table for c.
    * NOTE: shouldn't we actually *decrement* the frequency table for c? (maybe I'm misunderstanding the function of decode(), but someone explained why decrementing was correct instead of incrementing in a lab, and it made sense!)
*/

// NOTE: Rather than using the Huffman's tree (myTreePtr) to indicate whether we need to start a new sequence, I'm using an additional navigation pointer (navNode).
	//  This is because the tree at myTreePtr functions both as a Huffman tree and as our frequency table.
int Huffman::decode(bool bit) {
	if(myForest.size() == 0 && myTreePtr == nullptr){ // If we haven't established a frequency table...
		myForest    = initSymbolForest(ALPHABET_SIZE); // create a new forest full of symbol nodes with frequencies of 0
		myTreePtr   = buildTree(myForest); // Build tree.

	}
	if(navNode == nullptr) { //Base case: this is the first bit in a sequence...
		navNode     = myTreePtr; // Set navNode to the root of the tree

	}
	// If navNode != nullptr, it should be at the previous node we've computed.

	navNode = navNode->get_child(bitToDirection(bit));

	if(navNode->key_ == -1){ // If we're at an organization node...
		return -1;

	}else{ // Otherwise, we're at an actual character (c) leaf!
		++navNode->value_; // Increment frequency for c.
		HTree::key_t symbol = navNode->key_; // get and store the character symbol, so that we can return it after we've reset the tree.
		navNode             = nullptr; // The next bit will start a new sequence.

		//Do these to update our "frequency table" now that we've changed c's frequency:
		myForest    = breakTree(myTreePtr); //Break tree down into forest.
		myTreePtr   = buildTree(myForest); //Re-build tree.

		return symbol;

	}
}

// these are just here rather than making the constructor/destructor = default, because we aren't supposed to alter Huffman's public interface :P
Huffman::Huffman() {

}

Huffman::~Huffman() {

}

// encode: finds a key _> update the value (++Value) _> break tree _> build tree
// break_tree - takes a hierarchical tree and turns it into a non-hierarchical forest
// build_tree - takes a non-hierarchical forest and turns it into a hierarchical tree


