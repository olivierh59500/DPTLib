
#include <stdio.h>
#include <string.h>

#ifdef FORTIFY
#include "fortify/fortify.h"
#endif

#include "base/result.h"
#include "base/suppress.h"
#include "utils/array.h"

#include "datastruct/hash.h"

static char *my_strdup(const char *s)
{
  size_t l;
  char  *d;

  l = strlen(s);

  d = malloc(l + 1);
  if (d == NULL)
    return NULL;

  memcpy(d, s, l + 1);

  return d;
}

static int my_walk_fn(const void *key, const void *value, void *opaque)
{
  const char *sk = key;
  const char *sv = value;

  NOT_USED(opaque);

  printf("walk '%s':'%s'...\n", sk, sv);

  return 0;
}

result_t hash_test(void); /* suppress "No previous prototype" warning */

result_t hash_test(void)
{
  static const struct
  {
    const char *name;
    const char *value;
  }
  data[] =
  {
    { "deckard",   "rick" },
    { "batty",     "roy" },
    { "tyrell",    "rachael" },
    { "gaff",      "n/a" },
    { "bryant",    "n/a" },
    { "pris",      "n/a" },
    { "sebastian", "jf" },
  };

  result_t err;
  hash_t  *d;
  int      i;

  printf("test: create\n");

  /* use default string handling */
  err = hash_create(20, NULL, NULL, NULL, NULL, &d);
  if (err)
    goto Failure;

  printf("test: insert\n");

  for (i = 0; i < NELEMS(data); i++)
  {
    char *s;
    char *v;

    printf("adding '%s':'%s'...\n", data[i].name, data[i].value);

    s = my_strdup(data[i].name);
    v = my_strdup(data[i].value);

    if (!s || !v)
    {
      free(s);
      free(v);
      goto Failure;
    }

    err = hash_insert(d, s, v);
    if (err)
    {
      free(s);
      free(v);
      goto Failure;
    }
  }

  printf("test: iterate\n");

  hash_walk(d, my_walk_fn, NULL);

  printf("test: iterate by continuation\n");

  {
    int cont = 0;

    for (;;)
    {
      const void *key;
      const void *value;

      printf("cont = %x\n", cont);

      err = hash_walk_continuation(d, cont, &cont, &key, &value);
      if (err == result_HASH_END)
        break;

      printf("walk '%s':'%s'...\n", key, value);
    }
  }

  printf("test: remove\n");

  for (i = 0; i < NELEMS(data); i++)
    hash_remove(d, data[i].name);

  printf("test: destroy\n");

  hash_destroy(d);

  return result_TEST_PASSED;


Failure:

  return result_TEST_FAILED;
}
