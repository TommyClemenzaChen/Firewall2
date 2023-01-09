#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PARSER_LINE_LENGTH 1000

typedef struct Parser Parser;

Parser *parser_create(FILE *f);

void parser_delete(Parser **p);

bool next_word(Parser *p, char *word);

#endif
