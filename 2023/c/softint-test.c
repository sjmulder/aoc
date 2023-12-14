#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "softint.h"

int
main()
{
	/* -- u64_add -- */

	/* zeroes */
	assert(u64_eq(
	    u64_add(u64_mk(0, 0), u64_mk(0, 0)),
	    u64_mk(0, 0)));

	/* simple case */
	assert(u64_eq(
	    u64_add(u64_mk(1, 2), u64_mk(3, 4)),
	    u64_mk(4, 6)));

	/* overflow lo in a */
	assert(u64_eq(
	    u64_add(u64_mk(0, 0xFFFFFFFFU), u64_mk(0, 1)),
	    u64_mk(1, 0)));

	/* overflow lo in b */
	assert(u64_eq(
	    u64_add(u64_mk(0, 1), u64_mk(0, 0xFFFFFFFFU)),
	    u64_mk(1, 0)));

	/* overflow max */
	assert(u64_eq(
	    u64_add(u64_mk(0xFFFFFFFFU, 0xFFFFFFFFU), u64_mk(0, 1)),
	    u64_mk(0, 0)));

	/* -- u64_sub -- */

	/* zeroes */
	assert(u64_eq(
	    u64_sub(u64_mk(0, 0), u64_mk(0, 0)),
	    u64_mk(0, 0)));

	/* simple case */
	assert(u64_eq(
	    u64_sub(u64_mk(4, 3), u64_mk(2, 1)),
	    u64_mk(2, 2)));

	/* overflow */
	assert(u64_eq(
	    u64_sub(u64_mk(1, 0), u64_mk(0, 1)),
	    u64_mk(0, 0xFFFFFFFFU)));

	/* overflow max */
	assert(u64_eq(
	    u64_sub(u64_mk(0, 0), u64_mk(0, 1)),
	    u64_mk(0xFFFFFFFFU, 0xFFFFFFFFU)));
	
	/* -- u64_add32 -- */

	/* zeroes */
	assert(u64_eq(u64_add32(u64_mk(0, 0), 0), u64_mk(0, 0)));

	/* simple case */
	assert(u64_eq(u64_add32(u64_mk(1, 2), 3), u64_mk(1, 5)));

	/* overflow lo in a */
	assert(u64_eq(
	    u64_add32(u64_mk(0, 0xFFFFFFFFU), 1),
	    u64_mk(1, 0)));

	/* overflow max */
	assert(u64_eq(
	    u64_add32(u64_mk(0xFFFFFFFFU, 0xFFFFFFFFU), 1),
	    u64_mk(0, 0)));
	
	/* -- u64_sub32 -- */

	/* zeroes */
	assert(u64_eq(u64_sub32(u64_mk(0, 0), 0), u64_mk(0, 0)));

	/* simple case */
	assert(u64_eq(u64_sub32(u64_mk(3, 2), 1), u64_mk(3, 1)));

	/* overflow */
	assert(u64_eq(
	    u64_sub32(u64_mk(1, 0), 1),
	    u64_mk(0, 0xFFFFFFFFU)));

	/* overflow max */
	assert(u64_eq(
	    u64_sub32(u64_mk(0, 0), 1),
	    u64_mk(0xFFFFFFFFU, 0xFFFFFFFFU)));

	return 0;
}
