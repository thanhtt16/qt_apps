#include "LayoutSet.h"

LayoutSet::LayoutSet(QObject *parent) : QObject(parent) {
  layoutList.append({0, "1", 1, 1, 1, 1});
  layoutList.append({1, "4", 4, 1, 2, 2});
  layoutList.append({2, "9", 9, 1, 3, 3});
  layoutList.append({3, "16", 16, 1, 4, 4});
  layoutList.append({4, "25", 25, 1, 5, 5});
  layoutList.append({5, "36", 36, 1, 6, 6});
}

LayoutStruct LayoutSet::getLayout(int layoutCode) {
  for (int index = 0; index < layoutList.size(); ++index) {
    if (layoutList.at(index).code == layoutCode) return layoutList.at(index);
  }
  return layoutList.first();
}
