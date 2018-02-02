#include <iostream>
#include <map>
#include <chrono>

#include "avl_tree.h"

int main() {
  avl_utils::avl_tree<int, float> avl;
  std::map<int, float> rb;

  auto start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (int i = 0; i < 2000000; i++) {
    avl.insert(i, i);
  }
  auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL insert: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (int i = 0; i < 2000000; i++) {
    rb.insert(std::make_pair(i, i));
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  std::cout << "RB insert: " << (end - start).count() << std::endl;


  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (int i = 0; i < 2000000; i++) {
    avl.lookup(i);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "AVL lookup: " << (end - start).count() << std::endl;

  start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  for (int i = 0; i < 2000000; i++) {
    rb.at(i);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

  std::cout << "RB lookup: " << (end - start).count() << std::endl;

  return 0;
}
