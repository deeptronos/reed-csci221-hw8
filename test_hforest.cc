#include <stdio.h>
#include <assert.h>
#include <cassert>
#include <iostream>

#include "hforest.hh"
#include "htree.hh"

HTree::tree_ptr_t create_test_tree_small(){
// Creates a tree with only a root node
  return std::make_shared<HTree>(3,5);
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
 assert(forest->pop_tree()==large_tree); // tests popping on single tree, checks pointer returned corrrect
 assert(forest->size() == 2); // checks that size has decreased

 auto other_large_tree = create_test_tree_large();
 auto small_tree = create_test_tree_small();
 forest->add_tree(large_tree); //re add old tree
 forest->add_tree(small_tree); //smaller tree sandwhiched in between
 forest->add_tree(other_large_tree); //another large tree, identical to large_tree
 //checking if there are two nodes of the same size which it deletes
 assert(forest->pop_tree()==other_large_tree); //check that it deletes the most recently added tree
 assert(forest->pop_tree()==large_tree); //check that it deletes the large tree, and not the small tree in between

}


int main(){
    auto park = create_test_forest();

	test_size(park);
	test_add_tree(park);
	test_pop(park);
	return 0;
}
