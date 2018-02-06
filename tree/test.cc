#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <unordered_set>

#include "avl_tree.h"

namespace {
  constexpr int loops = 20000000;
}

int main() {
  std::srand(std::time(nullptr));

  avl_utils::avl_tree<int, float> avl;
  std::map<int, float> rb;
  std::vector<int> vals_to_insert;
  std::unordered_set<int> dups;
  for (int i = 0; i < loops; i++) {
    int r = std::rand();
    while (dups.find(r) != dups.end())
      r = std::rand();
    vals_to_insert.push_back(r);
    dups.insert(r);
  }
  auto start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    avl.insert(vals_to_insert[i], i);
  }
  auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL insert: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    rb.insert(std::make_pair(vals_to_insert[i], i));
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  std::cout << "RB insert: " << (end - start).count() << std::endl;


  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    avl.lookup(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL lookup: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    rb.at(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "RB lookup: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    avl.remove(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL remove: " << (end - start).count() << std::endl;


  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < loops; i++) {
    rb.erase(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "RB remove: " << (end - start).count() << std::endl;

  return 0;
}
