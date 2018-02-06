#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

#ifndef TREE_AVL_TREE_H
#define TREE_AVL_TREE_H

namespace {

    template<typename K, typename V> struct avl_node {
      avl_node(const K& k, const V& v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
      // Maximum height of avl tree is 1.44 * log2(n + 2) - 0.308
      // so we can use 1 byte char to save memory.
      // With number of nodes of 10^9 it's only 44
      unsigned char height;
      K key;
      V value;
      std::unique_ptr<avl_node<K, V>> left;
      std::unique_ptr<avl_node<K, V>> right;
    };

    template<typename K, typename V> char height(const std::unique_ptr<avl_node<K, V>>& node) {
      return node ? node->height : 0;
    }

    template<typename K, typename V> char balance_factor(const std::unique_ptr<avl_node<K, V>>& node) {
      return height(node->right) - height(node->left);
    }

    template<typename K, typename V> void recalculate_height(std::unique_ptr<avl_node<K, V>>& node) {
      char l_height = height(node->left);
      char r_height = height(node->right);
      node->height = std::max(l_height, r_height) + 1;
    }

    template<typename K, typename V> std::unique_ptr<avl_node<K, V>>& find_min_node(std::unique_ptr<avl_node<K, V>>& node) {
      return node->left ? find_min_node(node->left) : node;
    }

    template<typename K, typename V> std::unique_ptr<avl_node<K, V>> small_right_rotattion(std::unique_ptr<avl_node<K, V>>& node) {
      /*
      Used when balance_factor == 2 and height(q->right) <= height(q->left)
                        p
                      /  \
                     /    \
                    q      c
                   / \
                  /   \
                 a     b
                  \
                   \
                    d

                    ||
                    ||
                    \/
                    q
                  /   \
                 /     \
                a       p
                 \     / \
                  \   /   \
                  d  b     c
      */
      std::unique_ptr<avl_node<K, V>> new_root = std::move(node->left);
      node->left = std::move(new_root->right);
      new_root->right = std::move(node);
      recalculate_height(new_root->right);
      recalculate_height(new_root);
      return new_root;
    }

    template<typename K, typename V> std::unique_ptr<avl_node<K, V>> small_left_rotation(std::unique_ptr<avl_node<K, V>>& node) {
      // Is a mirror of small_right_rotattion
      std::unique_ptr<avl_node<K, V>> new_root = std::move(node->right);
      node->right = std::move(new_root->left);
      new_root->left = std::move(node);
      recalculate_height(new_root->left);
      recalculate_height(new_root);
      return new_root;
    }

    template<typename K, typename V> std::unique_ptr<avl_node<K, V>> balance(std::unique_ptr<avl_node<K, V>>& node) {
      recalculate_height(node);
      // Left rotation needed
      if (balance_factor(node) == 2) {
        // R < C. Need big rotation
        if (balance_factor(node->right) < 0)
          node->right = std::move(small_right_rotattion(node->right));
        return small_left_rotation(node);
      }

      // Right rotation needed
      if (balance_factor(node) == -2) {
        // C > L. Big rotation needed
        if (balance_factor(node->left) > 0)
          node->left = std::move(small_left_rotation(node->left));
        return small_right_rotattion(node);
      }

      return std::move(node);
    }
}

namespace avl_utils {
  template<typename K, typename V, typename Compare = std::less<K>, typename Equality = std::equal_to<K>> class avl_tree {
  public:

    avl_tree() : root(nullptr) {}

    void insert(const K& key, const V& value) {
      root = insert(root, key, value);
    }

    void remove(const K& key) {
      root = remove(root, key);
    }

    V lookup(const K& key) const {
      return lookup(root, key);
    }

  private:

    V lookup(const std::unique_ptr<avl_node<K, V>>& node, const K& key) const {
      if (!node)
        throw std::out_of_range("Can't find such an element");
      if (equal_to(node->key, key))
        return node->value;
      if (compare(key, node->key)) {
        return lookup(node->left, key);
      } else {
        return lookup(node->right, key);
      }
    }

    std::unique_ptr<avl_node<K, V>> remove_min_node(std::unique_ptr<avl_node<K, V>> &node) {
      if (!node->left)
        return remove_node(node);
      node->left = remove_min_node(node->left);
      return std::move(balance(node));
    }

    std::unique_ptr<avl_node<K, V>> remove_node(std::unique_ptr<avl_node<K, V>> &node) {
      // 1st case: don't have right neithbourgh
      if (!node->right)
        return std::move(node->left);
      // 2nd case: have right neighbourgh
      auto& min_node = find_min_node(node->right);
      node->key = min_node->key;
      node->value = min_node->value;
      node->right = std::move(remove_min_node(node->right));
      return std::move(balance(node));
    }

    std::unique_ptr<avl_node<K, V>> remove(std::unique_ptr<avl_node<K, V>>& node, const K& key) {
      if (!node)
        throw std::out_of_range("Can't find node to delete");

      // Found node to delete
      if (equal_to(key, node->key)) {
        return std::move(remove_node(node));
      } else if (compare(key, node->key)) {
        node->left = std::move(remove(node->left, key));
      } else {
        node->right = std::move(remove(node->right, key));
      }

      return std::move(balance(node));
    }

    std::unique_ptr<avl_node<K, V>> insert(std::unique_ptr<avl_node<K, V>>& node, const K& key, const V& value) const {
      if (node == nullptr)
        return std::make_unique<avl_node<K, V>>(key, value);
      if (equal_to(node->key, key)) {
        node->value = value;
        return std::move(node);
      }
      if (compare(key, node->key)) {
        node->left = insert(node->left, key, value);
      } else {
        node->right = insert(node->right, key, value);
      }

      return balance(node);
    }

    std::unique_ptr<avl_node<K, V>> root;
    Compare compare;
    Equality equal_to;
  };

}

#endif //TREE_AVL_TREE_H
