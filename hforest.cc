#include <iostream>
#include "hforest.hh"


bool compare_trees(HTree::tree_ptr_t t1, HTree::tree_ptr_t t2) {  // Eitan's compare_trees() function
    return t1->get_value() < t2->get_value();
}

HForest::HForest() {    //  Initialization when HForest is created without a tree in the arguments

}


HForest::HForest(HTree::tree_ptr_t firstForestTree) {   // Initialization when HForest is created with a tree in the arguments
    add_tree(firstForestTree);
}


HForest::size_t HForest::size() const{  // Return how many trees are in the forest
    return size_;
}


HTree::tree_ptr_t HForest::pop_tree(){ //  Return a pointer to the HTree with the LOWEST value in the root node, and remove it from the forest.


    if(size_ == 0){ //  Don't execute any more of pop_tree() if size is 0
        return nullptr;
    }

    // The forest will, presumably, already have its entries in heap order, since add_tree() runs make_heap() after adding a new tree to entries. Thus, the tree with the lowest-value root node will be the last entry.
    HTree::tree_ptr_t minRootNode       = entries.back();   // dereference the desired root node
    entries.erase(entries.end() - 1);

    size_ = size_ -1;
    return minRootNode;
}

void HForest::add_tree(HTree::tree_ptr_t tree) {    //  add_tree: pushes 'tree' onto the back of entries, sorts entries into a heap, increments size
    entries.push_back(tree);
    std::cout << "Before make_heap(): ";
    for(auto entry:entries){
    	std::cout << "( "<< entry->get_key() << ", " << entry->get_value() << "), ";
    }
	std::cout << std::endl;

    std::make_heap(entries.begin(), entries.end(), compare_trees);

	std::cout << "After make_heap(): ";
	for(auto entry:entries){
		std::cout << "( "<< entry->get_key() << ", " << entry->get_value() << "), ";
	}
	std::cout << std::endl;

    size_ = size_ + 1;
}





