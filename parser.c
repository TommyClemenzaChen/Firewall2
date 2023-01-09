#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "strfuncs.h"
#include <ctype.h>

struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
  Parser *p = (Parser *)malloc(sizeof(Parser));
  p->current_line[0] = '\n';
  p->line_offset = 0;
  p->f = f;
  return p;
}

void parser_delete(Parser **p) {
  free((*p));
  p = NULL;
}
// ONLY call fgets when line_offset is 0
bool next_word(Parser *p, char *word) {
  uint64_t index = 0;
  char *currline;
  char *temp = (char *)malloc(MAX_PARSER_LINE_LENGTH + 1);
  // only call fgets when there is a new line variable
  if (p->current_line[p->line_offset] == '\n') {

    p->line_offset = 0;
    currline = fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f);

    if (currline == NULL) {

      free(temp);
      fclose(p->f);
      return false;
    }
  }

  // increment 1 when it is not a letter/num or ' or /

  while (isalnum(p->current_line[p->line_offset]) == 0 &&
         p->current_line[p->line_offset] != '\'' &&
         p->current_line[p->line_offset] != '-') {
    // return true when it is new line and \e[A makes sure will subtract a line
    // to counteract the line being added in our main func
    if (p->current_line[p->line_offset] == '\n') {
      strcpy(word, "");
      return true;
    }
    if (p->current_line[p->line_offset] == '\0') {
      return false;
    }
    p->line_offset++;
  }
  // when the char is letter/num or one of the "ok" symbols.
  while (isalnum(p->current_line[p->line_offset]) != 0 ||
         p->current_line[p->line_offset] == '\'' ||
         p->current_line[p->line_offset] == '-') {
    temp[index] = tolower(p->current_line[p->line_offset]);
    index++;
    p->line_offset++;
    temp[index] = '\0';
  }
  if (p->current_line[p->line_offset] != '\n') {

    p->line_offset++;
  }

  scpy(word, temp);
  free(temp);

  return true;
}
