#include "../src/bignumber.h"
#include <stdio.h>

int main() {

  // uint128_t n1 = init128("340282366920938463463374607431768211455",
  // DEC_BASE); uint128_t n2 = init128("21234354343344", DEC_BASE);

  // uint128_t n1 = init128("1204759", DEC_BASE);
  // uint128_t n2 = init128("344", DEC_BASE);

  // uint128_t n1 = initu128("1024", DEC_BASE);
  // uint128_t n2 = initu128("345", DEC_BASE);

  uint128_t n1 = initu128("1024", DEC_BASE);
  uint128_t n2 = initu128("1000", DEC_BASE);

  printu128(n1);
  putc('\n', stdout);
  printu128(n2);
  putc('\n', stdout);
  /*
    addu128(n1, n2);
    printu128(n1);
    putc('\n', stdout);
    printu128(n2);
    putc('\n', stdout);
  */
  subtractu128(n1, n2);
  printu128(n1);
  putc('\n', stdout);
  printu128(n2);
  putc('\n', stdout);

  destroyu128(n1);
  destroyu128(n2);

  return 0;
}
