#include "ht.h"
#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ll.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdint.h>

struct HashTable {
  uint64_t salt;
  uint32_t size;
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_examined;
  bool mtf;
  LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {

  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht != NULL) {
    ht->mtf = mtf;
    ht->salt = 0x9846e4f157fe8840;
    ht->n_hits = ht->n_misses = ht->n_examined = 0;
    ht->n_keys = 0;
    ht->size = size;
    ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {
  for (int x = 0; x < (*ht)->size; x++) {
    if ((*ht)->lists[x]) {
      ll_delete(&(*ht)->lists[x]);
    }
  }
  free(*ht);
  ht = NULL;
}

uint32_t ht_size(HashTable *ht) { return ht->size; }

Node *ht_lookup(HashTable *ht, char *oldspeak) {
  // gets the index to see which linked list we will use
  // mod by size in order to keep it in bounds
  // divide by 64 b/c it will determine which set of 64 bits it will be in
  uint32_t seek, link, link2;
  ll_stats(&seek, &link);
  uint64_t index = (hash(ht->salt, oldspeak) % ht->size);
  if (ht->lists[index] == NULL) {
    ht->n_misses++;
    return NULL;
  }
  ht->n_hits++;

  Node *temp = ll_lookup(ht->lists[index], oldspeak);
  ll_stats(&seek, &link2);
  ht->n_examined += link2 - link;
  return temp;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
  uint64_t index = (hash(ht->salt, oldspeak) % ht->size);

  if (ht->lists[index] == NULL) {
    ht->lists[index] = ll_create(ht->mtf);
  }
  uint64_t isDup = ll_length(ht->lists[index]);
  ll_insert(ht->lists[index], oldspeak, newspeak);

  uint64_t isDup2 = ll_length(ht->lists[index]);
  if (isDup != isDup2) {
    ht->n_keys++;
  }
}

uint32_t ht_count(HashTable *ht) { return ht->n_keys; }

void ht_print(HashTable *ht) {
  for (int x = 0; x < ht->size; x++) {
    ll_print(ht->lists[x]);
  }
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {

  *nk = ht->n_keys;
  *nh = ht->n_hits;
  *nm = ht->n_misses;
  *ne = ht->n_examined;
}
