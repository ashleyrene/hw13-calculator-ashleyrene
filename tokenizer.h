#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <stddef.h>

enum token_type { TOK_LPAREN, TOK_RPAREN, TOK_ADD, TOK_MUL, TOK_NEG, TOK_LITERAL, TOK_EOF };

struct stream {
  const char *text;
  int pos;
  size_t length;
};

struct token {
  enum token_type type;
  const char *start;
  size_t length;
};

struct token peek(struct stream *s);
struct token next(struct stream *s);

#endif // TOKENIZER_H_
