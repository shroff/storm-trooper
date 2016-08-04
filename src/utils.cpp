#include "utils.h"

void swap(int &i1, int &i2) {
  i1 ^= i2;
  i2 ^= i1;
  i1 ^= i2;
}


bool validateArguments(String fn, int required, int given) {
  if (given < required) {
    Serial.println(fn);
    Serial.printf("Insufficient arguments. Need %d, but given %d\n", required, given);
    return false;
  }
  return true;
}
