#include <algorithm>
#include <functional>

#include "avl_tree_utils.h"

#ifndef TREE_AVL_TREE_H
#define TREE_AVL_TREE_H

namespace avl_utils {
  template<typename K, typename V, typename Compare = std::less<K>, typename Equality = std::equal_to<K>> class avl_tree {
  public:
    void insert(const K& key, const V& value) {
      root = insert(root, key, value);
    }

    void remove(const K& key);
    V lookup(const K& key) const throw (std::out_of_range) {
      return lookup(root, key);
    }

  private:

    V lookup(avl_node<K, V>* node, const K& key) const {
      if (node == nullptr)
        throw std::out_of_range("Can't find such an element");
      if (equal_to(node->key, key))
        return node->value;
      if (compare(key, node->key)) {
        return lookup(node->left, key);
      } else {
        return lookup(node->right, key);
      }
    }

    avl_node<K, V>* insert(avl_node<K, V>* node, const K& key, const V& value) const throw (std::out_of_range) {
      if (node == nullptr)
        return new avl_node<K, V>(key, value);
      if (equal_to(node->key, key)) {
        node->value = value;
        return node;
      }
      if (compare(key, node->key)) {
        node->left = insert(node->left, key, value);
      } else {
        node->right = insert(node->right, key, value);
      }

      return balance(node);
    }

    avl_node<K, V>* root = nullptr;
    Compare compare;
    Equality equal_to;
  };

}

#endif //TREE_AVL_TREE_H
