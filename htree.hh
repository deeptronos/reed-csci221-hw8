/*
 * HTree: a class to represent a tree node with two values: a key
 * and a value.
 */

#pragma once

#include <list>
#include <memory>

class HTree {
 public:
  using tree_ptr_t = std::shared_ptr<const HTree>;
  enum class Direction { LEFT, RIGHT };
  using path_t = std::list<Direction>;
  using possible_path_t = std::unique_ptr<path_t>;
  using key_t = int;
  using value_t = uint64_t;
  key_t key_;
  value_t value_;
  tree_ptr_t left_;
  tree_ptr_t right_;

  // Initialize with a key and a value:
  HTree(key_t key,
        value_t value,
        tree_ptr_t left = nullptr,
        tree_ptr_t right = nullptr);
  ~HTree() = default;

  key_t get_key() const;  // Return key in current node
  value_t get_value() const;      // Return value in current node

  // Return the child of this node indicated by dir.
  // If the child is nullptr (current node is a leaf), returns nullptr.
  tree_ptr_t get_child(Direction dir) const;

  // Return an optional list of directions from root to a node of a given key.
  // If key not contained in this tree, returns nullptr
  possible_path_t path_to(key_t key) const;


  
};
