#include "utils.h"

void swap(int &i1, int &i2) {
  i1 ^= i2;
  i2 ^= i1;
  i1 ^= i2;
}
