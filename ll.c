#include "ll.h"
#include "node.h"
#include "strfuncs.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t seeks;
uint32_t links;
struct LinkedList {
  uint32_t length;
  Node *head;
  Node *tail;
  bool mtf;
};

LinkedList *ll_create(bool mtf) {
  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
  ll->length = 0;

  // sentinel nodes
  ll->head = node_create(NULL, NULL);
  ll->tail = node_create(NULL, NULL);

  ll->head->next = ll->tail;
  ll->tail->prev = ll->head;

  ll->mtf = mtf;

  return ll;
}

void ll_delete(LinkedList **ll) {
  Node *curr = (*ll)->head;
  Node *temp = curr;
  while (curr) {
    temp = curr;
    curr = curr->next;

    node_delete(&temp);
  }

  free(*ll);
  ll = NULL;
}

uint32_t ll_length(LinkedList *ll) { return ll->length; }

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
  seeks += 1;
  Node *curr = ll->head->next;

  //[head,1,2,3,tail]
  while (curr->next && curr) {
    links += 1;
    // check if statement
    if (scmp(curr->oldspeak, oldspeak)) {
      if (ll->mtf) {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;

        curr->next = ll->head->next;
        curr->prev = ll->head;

        ll->head->next->prev = curr;
        ll->head->next = curr;
      }
      return curr;
    }
    // curr's next NULL
    //  [head, 1,2,3,4,tail]
    curr = curr->next;
  }
  return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
  if (ll_lookup(ll, oldspeak)) {
    return;
  }
  Node *n = node_create(oldspeak, newspeak);
  // Does the inserting
  n->next = ll->head->next;
  n->prev = ll->head;

  ll->head->next->prev = n;
  ll->head->next = n;

  ll->length++;
}

void ll_print(LinkedList *ll) {
  Node *curr = ll->head->next;
  while (curr->next) {
    node_print(curr);
    curr = curr->next;
  }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
  *n_seeks = seeks;
  *n_links = links;
}
