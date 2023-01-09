#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "bv.h"

struct BitVector {
  uint32_t length;
  uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
  // collac bellow malloc
  BitVector *bv = (BitVector *)malloc(sizeof(length));
  // Create an array of 0s
  uint32_t temp = 0;
  if (length % 64 != 0) {
    temp = 1;
  }
  bv->vector = (uint64_t *)calloc(length / 64 + temp, sizeof(uint64_t));
  bv->length = length;

  return bv;
}

void bv_delete(BitVector **bv) {
  free((*bv)->vector);
  free(*bv);
}

uint32_t bv_length(BitVector *bv) { return bv->length; }

void bv_set_bit(BitVector *bv, uint32_t i) {
  // set ith least significant bit to 1
  // 1 in bit we want change, 0 everywhere else
  // bit shifting
  //  most sig-> 00000000 <- least significant
  //  place 1 on last val
  //  shift by i % 64 to left
  //  Check pizza
  //  do nothing when outbounds
  //
  uint64_t a = i / 64, b = i % 64;
  bv->vector[a] |= (1UL << b);
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
  // oposite above
  // Create a 64 bit that will remove the certain bit while keeping rest the
  // same use and

  bv->vector[i / 64] &= ~(1UL << (i % 64));
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
  // copy the bits to another thingy and shift, then remove the rest of the bits
  // in the beginning
  uint64_t temp = bv->vector[i / 64];

  // converts 1 to uint64
  return (temp >> i % 64) & 1UL;
}

void bv_print(BitVector *bv) {
  // use bv get bits
  // iterate through each bit
  // print it
  for (uint64_t i = 0; i < bv->length; i++) {
    printf("%u", bv_get_bit(bv, i));
    if (i != 0 && i % 63 == 0) {
      printf("\n");
    }
  }

  //|64|64|64
  printf("\n");
}
