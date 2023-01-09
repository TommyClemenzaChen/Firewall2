#include "node.h"
#include "strfuncs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *oldspeak, char *newspeak) {

  Node *n = (Node *)malloc(sizeof(Node));
  if (oldspeak == NULL) {
    n->oldspeak = NULL;
  } else {
    n->oldspeak = sdup(oldspeak);
  }
  if (newspeak == NULL) {
    n->newspeak = NULL;

  } else {
    n->newspeak = sdup(newspeak);
  }
  n->prev = NULL;
  n->next = NULL;

  return n;
}

void node_delete(Node **n) {
  free((*n)->oldspeak);
  free((*n)->newspeak);
  free(*n);
  n = NULL;
}

void node_print(Node *n) {
  if (!n) {
    printf("NULL\n");
    return;
  }
  if (n->newspeak) {
    printf("%s -> %s\n", n->oldspeak, n->newspeak);
  } else {
    printf("%s\n", n->oldspeak);
  }
}
