#include <stdio.h>
#include <assert.h>
#include <cassert>
#include <iostream>

#include "hforest.hh"
#include "htree.hh"

HTree::tree_ptr_t create_test_tree_smallest(){
// Creates a tree with only a root node
	return std::make_shared<HTree>(0,1);
}

HTree::tree_ptr_t create_test_tree_small(){
// Creates a tree with only a root node and a child
  return std::make_shared<HTree>(3,5,
			 std::make_shared<HTree>(4,6));
}

HTree::tree_ptr_t create_test_tree_medium(){
// Creates a tree with only two children
  return std::make_shared<HTree>(1,2,
		  std::make_shared<HTree>(3,4),
		  std::make_shared<HTree>(5,6));
}

HTree::tree_ptr_t create_test_tree_large()
// Creates a large sample tree
// creates a tree with depth 3 to 4
{
  return std::make_shared<HTree>(126, 999,
		 std::make_shared<HTree>(1,5, 
			  std::make_shared<HTree>(9,8), 
			  std::make_shared<HTree>(30,2)),
		  std::make_shared<HTree>(12,40,
			  std::make_shared<HTree>(9,8,
				  std::make_shared<HTree>(85, 2)),
			  std::make_shared<HTree>(13,12)));
}

HForest::forest_ptr_t create_test_forest(){ //TYPE NEEDS TO BE CHANGED
  return std::make_shared<HForest>(create_test_tree_medium()); //not sure of exact syntax
}



void test_size(HForest::forest_ptr_t forest){
  assert(HForest().size()==0); //test on empty forest
  assert(forest->size()==1) ;
}

void test_add_tree( HForest::forest_ptr_t forest){
  auto small_tree = create_test_tree_small(); //creates a tree with only one node
  forest->add_tree(small_tree); //adds tree pointer to the forest
  assert(forest->size()==2); //check size of changed forest
}


void test_pop(HForest::forest_ptr_t forest){

	assert(HForest().pop_tree()== nullptr); //check that popping empty forest does nothing

 auto large_tree = create_test_tree_large(); //tree with a known large root
 forest->add_tree(large_tree);

 auto smallest_tree = create_test_tree_smallest();
 forest->add_tree(smallest_tree);

 assert(forest->pop_tree()==smallest_tree); // tests popping on single tree, checks pointer returned correctly
 assert(forest->size() == 3); // checks that size has decreased

 auto other_smallest_tree = create_test_tree_smallest();
 auto small_tree = create_test_tree_small();
 forest->add_tree(smallest_tree); //re add old tree
 forest->add_tree(small_tree); //bigger tree sandwiched in between
 forest->add_tree(other_smallest_tree); //another smallest tree, identical to smallest_tree
 //checking if there are two nodes of the same size which it deletes
 assert(forest->pop_tree()==other_smallest_tree); //check that it deletes the most recently added tree
 assert(forest->pop_tree()==smallest_tree); //check that it deletes the large tree, and not the small tree in between

}


int main(){
    auto park = create_test_forest();

	test_size(park);
	test_add_tree(park);
	test_pop(park);
	return 0;
}
