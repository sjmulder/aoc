	.data

scan_fmt:	.asciz " %15s %d"
cmd:		.skip 16
scalar:		.int 0
print_fmt:	.asciz "02: %d %d\n"

	.text
	.global main
	.extern scanf
	.extern printf

main:
	push	{lr}
	mov	r4, #0	// horizontal
	mov	r5, #0	// depth (pt2)
	mov	r6, #0	// depth (pt1), aim (pt2)
_loop:
	ldr	r0, =scan_fmt
	ldr	r1, =cmd
	ldr	r2, =scalar
	bl	scanf
	cmp	r0, #2
	bne	_end
	ldr	r1, =cmd
	ldrb	r1, [r1]
	ldr	r2, =scalar
	ldr	r2, [r2]
	cmp	r1, #'d'
	addeq	r6, r2
	cmp	r1, #'u'
	subeq	r6, r2
	cmp	r1, #'f'
	addeq	r4, r2
	muleq	r2, r6
	addeq	r5, r2
	b	_loop
_end:
	ldr	r0, =print_fmt
	mov	r1, r4
	mul	r1, r6
	mov	r2, r4
	mul	r2, r5
	bl	printf
	mov	r0, #0
	pop	{lr}
