#ifndef __LIBBIGNUMBER_INCLUDED_H__
#define __LIBBIGNUMBER_INCLUDED_H__

#define BIN_BASE 2
#define OCT_BASE 8
#define DEC_BASE 10
#define HEX_BASE 16

typedef struct uint128 *uint128_t;

extern uint128_t initu128(char *num, char base);
extern void destroyu128(uint128_t num);
extern void addu128(uint128_t dest, const uint128_t src);
extern void subtractu128(uint128_t dest, uint128_t src);
extern int numcmp(uint128_t n1, uint128_t n2);
extern void printu128(uint128_t num);

#endif // __LIBBIGNUMBER_INCLUDED_H__
