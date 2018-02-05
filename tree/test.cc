#include <iostream>
#include <map>
#include <chrono>
#include <ctime>
#include <vector>

#include "avl_tree.h"

int main() {
  std::srand(std::time(nullptr));

  avl_utils::avl_tree<int, float> avl;
  std::map<int, float> rb;
  std::vector<int> vals_to_insert;
  for (int i = 0; i < 20000000; i++) {
    vals_to_insert.push_back(std::rand());
  }
  auto start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < 20000000; i++) {
    avl.insert(vals_to_insert[i], i);
  }
  auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL insert: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < 20000000; i++) {
    rb.insert(std::make_pair(vals_to_insert[i], i));
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  std::cout << "RB insert: " << (end - start).count() << std::endl;


  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < 20000000; i++) {
    avl.lookup(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL lookup: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (size_t i = 0; i < 20000000; i++) {
    rb.at(vals_to_insert[i]);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "RB lookup: " << (end - start).count() << std::endl;

  return 0;
}
