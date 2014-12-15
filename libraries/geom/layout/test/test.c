
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "geom/box.h"
#include "test/txtscr.h"
#include "utils/array.h"

#include "geom/layout.h"

int layout_test(void)
{
  static const box_t dims = { 0, 0, 8, 8 };
  static const layout_element elements[] =
  {
    { layout_BOX, { .box = { 2, 2, 2 } } },
    { layout_BOX, { .box = { 2, 2, 2 } } },
    { layout_BOX, { .box = { 1, INT_MAX, 2 } } },
  };

  result_t    err;
  packer_t   *packer;
  layout_spec spec;
  box_t       boxes[3];
  int         i;
  box_t       used;
  txtscr_t   *scr;

  packer = packer_create(&dims);
  if (packer == NULL)
    goto failure;

  spec.packer  = packer;
  spec.loc     = packer_LOC_TOP_LEFT;
  spec.clear   = packer_CLEAR_LEFT;
  spec.spacing = 2;
  spec.leading = 2;

  err = layout_place(&spec,
                      elements,
                      NELEMS(elements),
                      boxes,
                      NELEMS(boxes));
  if (err)
    goto failure;

  used = *packer_get_consumed_area(packer);

  printf("consumed: %d %d %d %d\n", used.x0, used.y0, used.x1, used.y1);

  packer_destroy(packer);

  scr = txtscr_create(8, 8);
  if (scr == NULL)
    goto failure;

  txtscr_clear(scr);

  for (i = 0; i < NELEMS(boxes); i++)
    txtscr_addbox(scr, &boxes[i]);

  txtscr_print(scr);

  txtscr_destroy(scr);

  return result_TEST_PASSED;


failure:

  return result_TEST_FAILED;
}