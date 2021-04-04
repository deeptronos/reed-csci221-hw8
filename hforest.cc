/*
 * HForest: a class to represent a collection of HTrees, each with an
 * associated value (inverse priority).
 * Implemented using the STL's make_heap.
 */
#include <iostream>
#include <algorithm>
#include "hforest.hh"

//////////////////////////////////////////////////////////////////////////////
// Comparator function for std::*_heap
static bool
compare_trees(HForest::tree_t t1, HForest::tree_t t2)
{
  return t1->get_value() < t2->get_value();
}

//////////////////////////////////////////////////////////////////////////////
// Add a single tree to the forest:
void
HForest::add_tree(tree_t tree)
{
  trees_.push_back(tree);
  std::push_heap(trees_.begin(), trees_.end(), compare_trees);
}
// For Debug:
void tree_print(std::vector<HTree::tree_ptr_t> myTrees, std::string descriptor){

	std::cout << descriptor;
	for(HTree::tree_ptr_t currTree:myTrees){
		std::cout<< "(" << currTree->get_key() << ", " << currTree->get_value() << "), ";
	}
	std::cout << std::endl;

}

//////////////////////////////////////////////////////////////////////////////
// Return the tree with the highest frequency count (and remove it from forest)
HForest::tree_t
HForest::pop_top()
{
  if (trees_.empty()) {
    return nullptr;
  }

  tree_print(trees_, "Before weird code: ");

  //std::pop_heap(trees_.begin(), trees_.end(), compare_trees);
  std::reverse(trees_.begin(), trees_.end()); // Reverse trees_ so that the tree with the lowest frequency count is first
  std::pop_heap(trees_.begin(), trees_.end(), compare_trees); // Pop that tree from trees_
  auto ret = trees_.back();

  trees_.pop_back();

  std::reverse(trees_.begin() + 1, trees_.end()); // Put trees_ back in heap order - The first element will be the largest at this point due to pop_heap, but the rest of the vector will still be reversed. Thus, we must reverse everything past begin().

  tree_print(trees_, "After weird code: ");

  return ret;
}

