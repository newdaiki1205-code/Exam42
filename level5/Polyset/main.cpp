#include <iostream>
#include <stdlib.h>

#include "searchable_array_bag.hpp"
#include "searchable_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "set.hpp"

int main(int argc, char **argv) {
  if (argc == 1)
    return 1;
  searchable_bag *t = new searchable_tree_bag;
  searchable_bag *a = new searchable_array_bag;

  for (int i = 1; i < argc; i++) {
    t->insert(atoi(argv[i]));
    a->insert(atoi(argv[i]));
  }
  t->print();
  a->print();

  for (int i = 1; i < argc; i++) {
    std::cout << "tree searches " << argv[i] << " result: " << t->has(atoi(argv[i])) << std::endl;
    std::cout << "array searches " << argv[i] << " result: "<<  a->has(atoi(argv[i])) << std::endl;
    std::cout << "tree searches " << atoi(argv[i]-1) << " result: " << t->has(atoi(argv[i]) - 1) << std::endl;
    std::cout << "array searches " << atoi(argv[i]-1) << " result: " << a->has(atoi(argv[i]) - 1) << std::endl;
  }

  t->clear();
  a->clear();

  const searchable_array_bag tmp(static_cast<searchable_array_bag &>(*a));
  tmp.print();
  tmp.has(1);

  set sa(*a);
  set st(*t);
  int intArr[] = {1, 2, 3, 4};
  for (int i = 1; i < argc; i++) {
    st.insert(atoi(argv[i]));
    sa.insert(atoi(argv[i]));

    sa.has(atoi(argv[i]));
    sa.print();
    sa.get_bag().print();
    st.print();
    sa.clear();
    sa.insert(intArr, sizeof(intArr) / sizeof(int));
    std::cout << std::endl;
  }

  delete a;
  delete t;
  return (0);
}
