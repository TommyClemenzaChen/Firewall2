#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ll.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ht.h"
#include "node.h"
#include "parser.h"
#include "strfuncs.h"
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define OPTIONS "h t: f: m s"
int main(int argc, char **argv) {
  int opt = 0;
  // default values
  uint64_t hashSize = 10000;
  uint64_t bfSize = 524288;
  bool mtf = false;
  bool statPrint = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

    switch (opt) {
    case ('t'):
      hashSize = strtoull(optarg, NULL, 10);
      break;
    case ('f'):
      bfSize = strtoull(optarg, NULL, 10);
      break;
    case ('m'):
      mtf = true;
      break;
    case ('s'):
      statPrint = true;
      break;
    case ('h'):
      fprintf(stderr, "Usage: ./banhammer [options]\n");
      fprintf(stderr, "  ./banhammer will read in words from stdin, identify "
                      "any badspeak or old speak and output an\n");
      fprintf(stderr, "  appropriate punishment message. The badspeak and "
                      "oldspeak (with the newspeak translation)\n");
      fprintf(stderr, "  that caused the punishment will be printed after the "
                      "message. If statistics are enabled\n");
      fprintf(stderr, "  punishment messages are suppressed and only "
                      "statistics will be printed.\n");
      fprintf(stderr, "    -t <ht_size>: Hash table size set to <ht_size>. "
                      "(default: 10000)\n");
      fprintf(stderr, "    -f <bf_size>: Bloom filter size set to <bf_size>. "
                      "(default 2^19)\n");
      fprintf(stderr,
              "    -s          : Enables the printing of statistics.\n");
      fprintf(stderr, "    -m          : Enables move-to-front rule.\n");
      fprintf(stderr,
              "    -h          : Display program synopsis and usage.\n");
      return 0;

    default:
      fprintf(stderr, "Usage: ./banhammer [options]\n");
      fprintf(stderr, "  ./banhammer will read in words from stdin, identify "
                      "any badspeak or old speak and output an\n");
      fprintf(stderr, "  appropriate punishment message. The badspeak and "
                      "oldspeak (with the newspeak translation)\n");
      fprintf(stderr, "  that caused the punishment will be printed after the "
                      "message. If statistics are enabled\n");
      fprintf(stderr, "  punishment messages are suppressed and only "
                      "statistics will be printed.\n");
      fprintf(stderr, "    -t <ht_size>: Hash table size set to <ht_size>. "
                      "(default: 10000)\n");
      fprintf(stderr, "    -f <bf_size>: Bloom filter size set to <bf_size>. "
                      "(default 2^19)\n");
      fprintf(stderr,
              "    -s          : Enables the printing of statistics.\n");
      fprintf(stderr, "    -m          : Enables move-to-front rule.\n");
      fprintf(stderr,
              "    -h          : Display program synopsis and usage.\n");
      return 1;
    }
  }

  char *word = (char *)malloc(MAX_PARSER_LINE_LENGTH + 1);

  BloomFilter *bf = bf_create(bfSize + 1);
  HashTable *ht = ht_create(hashSize + 1, mtf);

  // storing bad speak
  FILE *bad = fopen("badspeak.txt", "r");
  Parser *p = parser_create(bad);
  char *temp = (char *)malloc(MAX_PARSER_LINE_LENGTH + 1);

  while (next_word(p, word)) {

    //	printf("%s\n", word);
    bf_insert(bf, word);
    ht_insert(ht, word, NULL);
  }

  // Storing new and old speak
  FILE *new = fopen("newspeak.txt", "r");
  Parser *p2 = parser_create(new);
  char *word2 = (char *)malloc(MAX_PARSER_LINE_LENGTH + 1);

  while (next_word(p2, word)) {

    next_word(p2, word2);
    bf_insert(bf, word);
    ht_insert(ht, word, word2);
    //	printf("%s->%s\n", word, word2);
  }

  // bad and old speak words used
  LinkedList *badsp = ll_create(mtf);
  LinkedList *oldsp = ll_create(mtf);
  FILE *input = stdin;
  Parser *p3 = parser_create(input);
  char *word3 = (char *)malloc(MAX_PARSER_LINE_LENGTH + 1);

  while (next_word(p3, word3)) {

    // check if the word in bloom filter
    if (bf_probe(bf, word3)) {
      Node *b = ht_lookup(ht, word3);

      if (b == NULL) {
        continue;
      } else if (b->newspeak != NULL) {
        // word has a newspeak transalation so the citizen will require
        // conseuling
        ll_insert(oldsp, word3, b->newspeak);
      } else {
        // word doesn't have a newspeak transation so citizen will be guilty of
        // thoughtcrime as this word is a "badspeak" word
        ll_insert(badsp, word3, NULL);
      }
    }
  }

  // accsed of thought crime and REQUIREWS CONSEILING ON PROPER right speak
  if (ll_length(badsp) > 0 && ll_length(oldsp) > 0 && !statPrint) {
    printf("Dear beloved citizen of the GPRSC,\n\n");
    printf("We have some good news, and we have some bad news.\n");
    printf("The good news is that there is bad news. The bad news is that you "
           "will\n");
    printf("be sent to joycamp and subjected to a week-long destitute "
           "existence.\n");

    printf(
        "This is the penalty for using degenerate words, as well as using\n");
    printf("oldspeak in place of newspeak. We hope you can correct your "
           "behavior.\n\n");
    printf("Your transgressions, followed by the words you must think on:\n\n");
    ll_print(badsp);
    ll_print(oldsp);
  }
  // accsed only of thorughtcrime
  else if (ll_length(badsp) > 0 && !statPrint) {
    printf("Dear beloved citizen of the GPRSC,\n\n");
    printf("You have been caught using degenerate words that may cause\n");
    printf("distress among the moral and upstanding citizens of the GPSRC.\n");
    printf(
        "As such, you will be sent to joycamp. It is there where you will\n");
    printf(
        "sit and reflect on the consequences of your choice in language.\n\n");
    printf("Your transgressions:\n\n");
    ll_print(badsp);
  }
  // conseoling
  else if (ll_length(oldsp) > 0 && !statPrint) {
    printf("Dear beloved citizen of the GPRSC,\n\n");
    printf(
        "We recognize your efforts in conforming to the language standards\n");
    printf("of the GPSRC. Alas, you have been caught uttering questionable "
           "words\n");
    printf(
        "and thinking unpleasant thoughts. You must correct your wrongspeak\n");
    printf("and badthink at once. Failure to do so will result in your "
           "deliverance\n");
    printf("to joycamp.\n\n");
    printf("Words that you must think on:\n\n");
    ll_print(oldsp);
  }

  // stats
  if (statPrint) {
    uint32_t ht_keys, ht_misses, ht_probes, ht_hits, b_keys, b_hits, b_misses,
        b_bits_exam;
    double bit_exam_per_miss, falsePositive, bf_load, avg_seek_len;
    ht_stats(ht, &ht_keys, &ht_hits, &ht_misses, &ht_probes);
    bf_stats(bf, &b_keys, &b_hits, &b_misses, &b_bits_exam);
    if (b_misses == 0) {
      bit_exam_per_miss = 0;
    } else {
      bit_exam_per_miss =
          (double)(b_bits_exam - N_HASHES * b_hits) / (double)b_misses;
    }
    falsePositive = (double)ht_misses / (double)b_hits;

    bf_load = (double)bf_count(bf) / (double)bf_size(bf);
    avg_seek_len = ht_probes / (ht_hits + ht_misses);

    printf("ht keys: %u\n", ht_keys);
    printf("ht hits: %u\n", ht_hits);
    printf("ht misses: %u\n", ht_misses);
    printf("ht probes: %u\n", ht_probes);
    printf("bf keys: %u\n", b_keys);
    printf("bf hits: %u\n", b_hits);
    printf("bf misses: %u\n", b_misses);
    printf("bf bits examined: %u\n", b_bits_exam);
    printf("Bits examined per miss: %f\n", bit_exam_per_miss);
    printf("False positives: %f\n", falsePositive);
    printf("Average seek length: %f\n", avg_seek_len);
    printf("Bloom filter load: %f\n", bf_load);
  }
  ll_delete(&badsp);
  ll_delete(&oldsp);

  free(word);
  ht_delete(&ht);
  bf_delete(&bf);
  parser_delete(&p);
  parser_delete(&p2);
  parser_delete(&p3);
  free(word2);
  free(word3);

  return 0;
}
