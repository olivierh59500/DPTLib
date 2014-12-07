
#include <stdio.h>
#include <stdlib.h>

#ifdef FORTIFY
#include "fortify/fortify.h"
#endif

#include "types.h"
#include "datastruct/bitarr.h"

#define NBITS 97

typedef bitarr_ARRAY(NBITS) TestBits;

static void dumpbits(const TestBits *arr, size_t nbits)
{
  int i;
  int c;

  c = 0;
  for (i = nbits - 1; i >= 0; i--)
  {
    int b;

    b = bitarr_get(arr, i);
    c += b;
    printf("%d", b);
  }

  printf("\n%d bits set\n", c);
}

int bitarr_test(void)
{
  TestBits arr;
  int      i;

  printf("test: create\n");

  bitarr_wipe(arr, sizeof(arr));


  printf("test: set\n");

  for (i = 0; i < NBITS; i++)
    bitarr_set(&arr, i);

  dumpbits(&arr, NBITS);


  printf("test: clear\n");

  for (i = 0; i < NBITS; i++)
    bitarr_clear(&arr, i);

  dumpbits(&arr, NBITS);


  printf("test: toggle\n");

  for (i = 0; i < NBITS; i++)
    bitarr_toggle(&arr, i);

  dumpbits(&arr, NBITS);


  printf("test: count\n");

  /* a cast is required :-| */
  printf("%d bits set\n", bitarr_count((bitarr_t *) &arr, sizeof(arr)));


  return 0;
}