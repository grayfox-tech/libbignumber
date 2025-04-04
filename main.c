#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BIN128_MAX_LEN 128
#define OCT128_MAX_LEN 44
#define HEX128_MAX_LEN 32
#define UDEC128_MAX_STR "340282366920938463463374607431768211455"
#define UDEC128_MAX_LEN 39

#define BIN_BASE 2
#define OCT_BASE 8
#define DEC_BASE 10
#define HEX_BASE 16

typedef struct uint128 {
  char *n;
  unsigned char nlen;
  unsigned char base;
  unsigned char ptrlen;
} *uint128_t;

#define ZEROES_FILL(dest, src, len, offset)                                    \
  do {                                                                         \
    for (int i = len; i >= 0; --i)                                             \
      dest[i + offset] = src[i];                                               \
    for (int i = 0; i < offset; ++i)                                           \
      dest[i] = '0';                                                           \
  } while (0)

static int is_valid_num(char *num, size_t len, char base) {
  switch (base) {
  case 2:
    return !(len > BIN128_MAX_LEN);
  case 8:
    return !((len == OCT128_MAX_LEN && num[0] - '0' > 1) ||
             len > OCT128_MAX_LEN);
  case 10:
    return !((len == UDEC128_MAX_LEN && num[0] - '0' > 3) ||
             len > UDEC128_MAX_LEN);
  case 16:
    return !(len > HEX128_MAX_LEN);
  default:;
  }

  return 0;
}

static char *alloc_num(char base, unsigned char *nlen) {
  unsigned char len = 0;
  if (nlen)
    nlen = &len;

  switch (base) {
  case 2:
    len = BIN128_MAX_LEN;
    break;
  case 8:
    len = OCT128_MAX_LEN;
    break;
  case 10:
    len = UDEC128_MAX_LEN;
    break;
  case 16:
    len = HEX128_MAX_LEN;
    break;
  default:;
  }

  return (char *)malloc(len * sizeof(char));
}

uint128_t init128(char *num, char base) {
  size_t __nlen = strlen(num);
  uint128_t __n = NULL;

  if (!is_valid_num(num, __nlen, base))
    goto err;

  __n = (uint128_t)malloc(sizeof(struct uint128));
  if (!__n)
    goto err;

  __n->n = alloc_num(base, &__n->ptrlen);
  if (!__n->n)
    goto err;

  memcpy(__n->n, num, __nlen);
  __n->nlen = __nlen;
  __n->base = base;

  return __n;

err:
  if (__n)
    free(__n);

  return NULL;
}

void destroy128(uint128_t num) { free(num); }

void addu128(uint128_t dest, const uint128_t src) {
  int i, offset = abs(dest->nlen - src->nlen), res = 0, dec = 0;
  char buff[BIN128_MAX_LEN];

  if (dest->nlen < src->nlen) {
    ZEROES_FILL(dest->n, dest->n, dest->nlen - 1, offset);
    dest->nlen += offset;
    memcpy(buff, src->n, src->nlen);
  } else {
    ZEROES_FILL(buff, src->n, src->nlen - 1, offset);
  }

  for (i = dest->nlen - 1; i >= 0; --i) {
    res = (dest->n[i] - '0') + (buff[i] - '0') + dec;
    dec = res >= 10 ? 1 : 0;
    dest->n[i] = dec ? (res - 10) + '0' : res + '0';
  }

  if (dec) {
    for (i = dest->nlen - 1; i >= 0; --i)
      dest->n[i + 1] = dest->n[i];
    dest->n[0] = dec + '0';
    dest->nlen++;
  }
}

void subtractu128(uint128_t dest, uint128_t src) {}

void print128(uint128_t num) { write(STDOUT_FILENO, num->n, num->nlen); }

int main() {

  // uint128_t n1 = init128("340282366920938463463374607431768211455",
  // DEC_BASE); uint128_t n2 = init128("21234354343344", DEC_BASE);

  // uint128_t n1 = init128("1204759", DEC_BASE);
  // uint128_t n2 = init128("344", DEC_BASE);

  uint128_t n1 = init128("659", DEC_BASE);
  uint128_t n2 = init128("344", DEC_BASE);

  print128(n1);
  putc('\n', stdout);
  print128(n2);
  putc('\n', stdout);

  addu128(n2, n1);
  print128(n1);
  putc('\n', stdout);
  print128(n2);
  putc('\n', stdout);

  destroy128(n1);
  destroy128(n2);

  return 0;
}
