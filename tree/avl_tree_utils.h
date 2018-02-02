#ifndef TREE_AVL_TREE_UTILS_H
#define TREE_AVL_TREE_UTILS_H

namespace avl_utils {

  template<typename K, typename V> struct avl_node {
    avl_node(const K& k, const V& v) : key(k), value(v), height(1) {}
    // Maximum height of avl tree is 1.44 * log2(n + 2) - 0.308
    // so we can use 1 byte char to save memory.
    // With number of nodes of 10^9 it's only 44
    unsigned char height;
    K key;
    V value;
    avl_node* left;
    avl_node* right;
  };

  template<typename K, typename V> char height(avl_node<K, V>* node) {
    return node ? node->height : 0;
  }

  template<typename K, typename V> char balance_factor(avl_node<K, V>* node) {
    return height(node->right) - height(node->left);
  }

  template<typename K, typename V> void recalculate_height(avl_node<K, V>* node) {
    char l_height = height(node->left);
    char r_height = height(node->right);
    node->height = std::max(l_height, r_height) + 1;
  }

  template<typename K, typename V> avl_node<K, V>* small_right_rotattion(avl_node<K, V>* node) {
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
    avl_node<K, V>* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    recalculate_height(node);
    recalculate_height(new_root);
    return new_root;
  }

  template<typename K, typename V> avl_node<K, V>* small_left_rotation(avl_node<K, V>* node) {
    // Is a mirror of small_right_rotattion
    avl_node<K, V>* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    recalculate_height(node);
    recalculate_height(new_root);
    return new_root;
  }

  template<typename K, typename V> avl_node<K, V>* balance(avl_node<K, V>* node) {
    recalculate_height(node);
    // Left rotation needed
    if (balance_factor(node) == 2) {
      // R < C. Need big rotation
      if (balance_factor(node->right) < 0)
        node->right = small_right_rotattion(node->right);
      return small_left_rotation(node);
    }

    // Right rotation needed
    if (balance_factor(node) == -2) {
      // C > L. Big rotation needed
      if (balance_factor(node->left) > 0)
        node->left = small_left_rotation(node->left);
      return small_right_rotattion(node);
    }

    return node;
  }
}

#endif //  TREE_AVL_TREE_UTILS_H
