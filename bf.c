#include <stdio.h>

#include "bf.h"
#include "bv.h"
#include "city.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define N_HASHES 5

struct BloomFilter {
  uint64_t salts[N_HASHES];
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;
};

static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21

};

BloomFilter *bf_create(uint32_t size) {
  // allocate based on user input
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));

  // if curr node isn't node,
  if (bf) {
    // sets all the values to zero
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;

    // copies the default_salts to salts so bloomfilter can access values
    for (int i = 0; i < N_HASHES; i++) {
      bf->salts[i] = default_salts[i];
    }
    //
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf;
}

void bf_delete(BloomFilter **bf) {
  bv_delete(&(*bf)->filter);
  free(*bf);
}

uint32_t bf_size(BloomFilter *bf) { return bv_length(bf->filter); }

void bf_insert(BloomFilter *bf, char *oldspeak) {
  // setting the bits at the indicies that result from hash funciton

  uint64_t temp;

  for (int x = 0; x < 5; x++) {
    // mod in order to keep the hash value in range of the bit vector
    temp = hash(bf->salts[x], oldspeak) % bv_length(bf->filter);

    bv_set_bit(bf->filter, temp);
  }
  bf->n_keys += 1;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
  uint64_t temp;

  for (int x = 0; x < 5; x++) {

    bf->n_bits_examined += 1;
    temp = hash(bf->salts[x], oldspeak) % bv_length(bf->filter);
    if (bv_get_bit(bf->filter, temp) == 0) {
      bf->n_misses += 1;
      return false;
    }
  }
  bf->n_hits += 1;
  return true;
}

uint32_t bf_count(BloomFilter *bf) { return (bf->n_keys * 5); }

void bf_print(BloomFilter *bf) {
  printf("nkeys: %u nhits: %u nmisses: %u examined: %u\n", bf->n_keys,
         bf->n_hits, bf->n_misses, bf->n_bits_examined);
  // bv_print(bf->filter);
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = bf->n_keys;
  *nh = bf->n_hits;
  *nm = bf->n_misses;
  *ne = bf->n_bits_examined;
}
