#include "weight.h"


u32 readCount() {
  u32 count;
  u8 i;
  ADSK = 0;
  while(ADDO);
  for(i = 0; i < 24; ++i) {
    ADSK = 1;
    count = count << 1;
    ADSK = 0;
    if(ADDO) count++;
  }
  ADSK = 1;
  count = count & (~0xff000000);
  ADSK = 0;
  return count;
}

#define WEIGHT_BASE (163350)
#define WEIGHT_FACTOR (0.002118)
float readGram(u32 v) {
  return (v -WEIGHT_BASE) * WEIGHT_FACTOR;
}
