/*
 * test_tree: A simple unit test for the Tree data structure.
 */

#include "../inc/htree.hh"
#include <cassert>

using namespace std;

// Create an elaborate example test tree:
HTree::tree_ptr_t create_test_tree()
{
  return
    make_shared<HTree>(126, 1,
      make_shared<HTree>(-5, 2,
        make_shared<HTree>(12, 3),
        make_shared<HTree>(6, 4,
          make_shared<HTree>(9, 5))),
      make_shared<HTree>(12, 6,
        make_shared<HTree>(3, 7)));
}

void test_htree(const HTree::tree_ptr_t root)
{
  using path_t = HTree::path_t;
  const auto L = HTree::Direction::LEFT;
  const auto R = HTree::Direction::RIGHT;

  assert(*(root->path_to(126)) == path_t({ }));
  assert(*(root->path_to(-5)) == path_t({ L }));
  assert(*(root->path_to(9)) == path_t({ L, R, L }));
  assert(*(root->path_to(3)) == path_t({ R, L }));
  assert(*(root->path_to(12)) == path_t({ L, L }));
  assert(root->path_to(54) == nullptr);

  assert(root->get_child(L)->get_key() == -5);
  assert(root->get_child(L)->get_child(L)->get_key() == 12);
  assert(root->get_child(L)->get_child(L)->get_child(L) == nullptr);
  assert(root->get_child(L)->get_child(L)->get_child(R) == nullptr);
  assert(root->get_child(L)->get_child(R)->get_key() == 6);
  assert(root->get_child(L)->get_child(R)->get_child(L)->get_key() == 9);
  assert(root->get_child(R)->get_child(R) == nullptr);
}

void testSetValue(HTree::tree_ptr_t root){
	HTree::value_t newVal = 72;
	root->setValue(newVal);
	assert(root->get_value() == newVal);
}

int main()
{
  auto root = create_test_tree();
  test_htree(root);
  testSetValue(root);
  return 0;
}

