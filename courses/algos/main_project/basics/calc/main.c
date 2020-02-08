#include "leio.h"

#include <assert.h>

#define ASCII_UA (65)
#define ASCII_LA (97)
#define ASCII_NL (10)

#define TOK_EOF (0)
#define TOK_ADD (1)
#define TOK_SUB (2)
#define TOK_MUL (3)
#define TOK_DIV (4)
#define TOK_MOD (5)
#define TOK_LPAREN (6)
#define TOK_RPAREN (7)
#define TOK_NUM (8)
#define TOK_WS (TOK_NUM + 10)
// TOK_NUM to TOK_NUM+9: numbers 0 to 9

#include "stdio.h"

void err(int_t c) {
  std_putc(69); // E
  std_putc(58); //:
  std_putc(c);
  std_putc(ASCII_NL);
  std_exit(1);
}

int_t is_wspace(int_t x) {
  return x == 9|| x == 10 || x == 32;
}

int_t skip_ws() {
  int_t c = std_getc();
  while (is_wspace(c))
    c = std_getc();
  return c;
}

int_t encode_char(int_t c) {
  if (is_wspace(c))
    return TOK_WS;
  else if (c == -1)
    return TOK_EOF;
  else if (c == 37)
    return TOK_MOD;
  else if (c == 40)
    return TOK_LPAREN;
  else if (c == 41)
    return TOK_RPAREN;
  else if (c == 42)
    return TOK_MUL;
  else if (c == 43)
    return TOK_ADD;
  else if (c == 45)
    return TOK_SUB;
  else if (c == 47)
    return TOK_DIV;
  else if (c >= 48 && c <= 57)
    return (c - 48) + TOK_NUM;

  err(76); // L
  return 0;
}

// 16 upper bytes: number or state
// 7 lower bytes: next_state
// 8th byte: is_number

int_t tok_val_is_num(int_t tok) { return (tok / 128) % 2; }

int_t tok_val_next_state(int_t tok) { return tok % 128; }

int_t tok_val_state(int_t tok) { return tok / 65536; }

int_t peek_tok(int_t next_state) {
  // next_state represents the next_char to be read
  // 1) get a next_state which is not a whitespace
  if (next_state == TOK_WS) {
    next_state = encode_char(skip_ws());
  }

  return next_state;
}

int_t get_tok(int_t next_state) {
  // next_state represents the next_char to be read
  // 1) get a next_state which is not a whitespace
  if (next_state == TOK_WS) {
    next_state = encode_char(skip_ws());
  }

  // 2) current state is next state
  int_t state = next_state;
  int_t val = 0;

  // 3) parse number and fill next_state
  while (next_state >= TOK_NUM && next_state < TOK_WS) {
    int_t digit = next_state - TOK_NUM;
    val = 10 * val + digit;
    next_state = encode_char(std_getc());
  }

  // 4) if not a number, can leave next_state to a whitespace for future read
  if (state < TOK_NUM)
    next_state = TOK_WS;

  int_t is_num = state >= TOK_NUM;
  state = is_num ? val : state;
  return next_state + is_num * 128 + state * 65536;
}

// snum: bit sign in lower bit, rest is absolute value
int_t snum_encode(int_t val) {
  int_t sign = val < 0;
  int_t aval = sign ? -val : val;
  return aval * 2 + sign;
}

int_t snum_decode(int_t code) {
  int_t sign = code % 2;
  int_t aval = code / 2;
  return sign ? -aval : aval;
}

int_t snum_neg(int_t code) {
  int_t sign = code % 2;
  int_t new_sign = !sign;
  return code - sign + new_sign;
}

// encode / decode signed 15 bits number (undefined if 16th bit is one)
int_t pack16(int_t high, int_t low) { return high * 65536 + low; }
int_t unpack16_high(int_t val) { return val / 65536; }
int_t unpack16_low(int_t val) { return val % 65536; }

// file: EXPR @EOF
// EXPR : E3
// E3: E2 ('+' | '-' E2)*
// E2: E1 ('*' | '/' | '%' E1)*
// E1:  '+' | '-' E1
//    | E0
// E0:  '(' EXPR ')'
//    | @int

// struct parse_ret {
//  int_t next_state; // next_state of lexer
//  int_t val;        // resulting value
//};

int_t r_file(int_t next_state);
int_t r_expr(int_t next_state);
int_t r_e3(int_t next_state);
int_t r_e2(int_t next_state);
int_t r_e1(int_t next_state);
int_t r_e0(int_t next_state);

int_t r_file(int_t next_state) {
  int_t val_ret = r_expr(next_state);

  int_t tok = get_tok(unpack16_high(val_ret));
  if (tok_val_is_num(tok) || tok_val_state(tok) != TOK_EOF)
    err(80); // P

  return pack16(tok_val_next_state(tok), unpack16_low(val_ret));
}

int_t r_expr(int_t next_state) { return r_e3(next_state); }

int_t r_e3(int_t next_state) {
  int_t ret_val = r_e2(next_state);
  int_t val = snum_decode(unpack16_low(ret_val));
  next_state = peek_tok(unpack16_high(ret_val));

  while (next_state == TOK_ADD || next_state == TOK_SUB) {
    int_t tok = get_tok(next_state);
    int_t ret_val = r_e2(tok_val_next_state(tok));
    if (next_state == TOK_ADD)
      val += snum_decode(unpack16_low(ret_val));
    else if (next_state == TOK_SUB)
      val -= snum_decode(unpack16_low(ret_val));
    next_state = peek_tok(unpack16_high(ret_val));
  }

  return pack16(next_state, snum_encode(val));
}

int_t r_e2(int_t next_state) {
  int_t ret_val = r_e1(next_state);
  int_t val = snum_decode(unpack16_low(ret_val));
  next_state = peek_tok(unpack16_high(ret_val));

  while (next_state == TOK_MUL || next_state == TOK_DIV ||
         next_state == TOK_MOD) {
    int_t tok = get_tok(next_state);
    int_t ret_val = r_e1(tok_val_next_state(tok));
    if (next_state == TOK_MUL)
      val *= snum_decode(unpack16_low(ret_val));
    else if (next_state == TOK_DIV)
      val /= snum_decode(unpack16_low(ret_val));
    else if (next_state == TOK_MOD)
      val %= snum_decode(unpack16_low(ret_val));
    next_state = peek_tok(unpack16_high(ret_val));
  }

  return pack16(next_state, snum_encode(val));
}

int_t r_e1(int_t next_state) {
  next_state = peek_tok(next_state);

  if (next_state != TOK_ADD && next_state != TOK_SUB)
    return r_e0(next_state);

  // read + / -
  int_t tok = get_tok(next_state);

  // read e1
  int_t val_ret = r_e1(tok_val_next_state(tok));
  int_t val_ret_next_state = unpack16_high(val_ret);
  int_t res_val = next_state == TOK_ADD ? unpack16_low(val_ret)
                                        : snum_neg(unpack16_low(val_ret));
  return pack16(val_ret_next_state, res_val);
}

int_t r_e0(int_t next_state) {
  next_state = peek_tok(next_state);

  if (next_state == TOK_LPAREN) {
    // read (
    int_t tok = get_tok(next_state);

    // read expr
    int_t val_ret = r_expr(tok_val_next_state(tok));

    // read )
    tok = get_tok(unpack16_high(val_ret));
    if (tok_val_is_num(tok) || tok_val_state(tok) != TOK_RPAREN)
      err(80); // P

    return pack16(tok_val_next_state(tok), unpack16_low(val_ret));
  }

  else {
    if (next_state < TOK_NUM)
      err(80); // P

    int_t tok = get_tok(next_state);
    return pack16(tok_val_next_state(tok), snum_encode(tok_val_state(tok)));
  }
}

int_t parse() {
  int_t res = r_file(TOK_WS);
  return snum_decode(unpack16_low(res));
}

int main() {
  int_t res = parse();
  printnl_int(res);
}
