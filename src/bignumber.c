#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BIN128_MAX_LEN 128
#define OCT128_MAX_LEN 44
#define HEX128_MAX_LEN 32
#define UDEC128_MAX_STR "340282366920938463463374607431768211455"
#define UDEC128_MAX_LEN 39

struct uint128 {
  char *n;
  unsigned char nlen;
  unsigned char base;
  unsigned char ptrlen;
};

#define ZEROES_REMOVE(num)                                                     \
  do {                                                                         \
    while (num->n[0] == '0' && num->nlen > 1) {                                \
      for (int i = 1; i < num->nlen; ++i)                                      \
        num->n[i - 1] = num->n[i];                                             \
      num->nlen--;                                                             \
    }                                                                          \
  } while (0)

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

int numcmp(uint128_t n1, uint128_t n2) {
  if (n1->nlen != n2->nlen)
    return n1->nlen < n2->nlen ? 1 : -1;

  for (int i = 0; i < n1->nlen; ++i)
    if (n1->n[i] != n2->n[i])
      return n1->n[i] < n2->n[i] ? 1 : -1;

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

uint128_t initu128(char *num, char base) {
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

void destroyu128(uint128_t num) {
  if (num) {
    if (num->n) {
      free(num->n);
    }
    free(num);
  }
}

void addu128(uint128_t dest, const uint128_t src) {
  int i, offset = abs(dest->nlen - src->nlen), res = 0, dec = 0;
  char buff[BIN128_MAX_LEN];

  // TODO check numbers == base
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

  // TODO check that the sum do not overflow
  if (dec) {
    for (i = dest->nlen - 1; i >= 0; --i)
      dest->n[i + 1] = dest->n[i];
    dest->n[0] = dec + '0';
    dest->nlen++;
  }
}

void subtractu128(uint128_t dest, uint128_t src) {
  int offset = abs(dest->nlen - src->nlen), res = 0, dec = 0;
  char buff[BIN128_MAX_LEN];

  // check if src is greater than dest, we cannot store a negative number
  // TODO check numbers when they have equal length
  if (numcmp(dest, src) == 1) {
    dest->n[0] = '0';
    dest->nlen = 1;
    // TODO set error
    return;
  }

  // TODO check numbers == base
  ZEROES_FILL(buff, src->n, src->nlen - 1, offset);

  for (int i = dest->nlen - 1; i >= 0; --i) {
    res = (dest->n[i] - '0') - (buff[i] - '0') - dec;
    dec = res < 0 ? 1 : 0;
    dest->n[i] = dec ? (res + 10) + '0' : res + '0';
  }

  ZEROES_REMOVE(dest);
}

void printu128(uint128_t num) { write(STDOUT_FILENO, num->n, num->nlen); }
