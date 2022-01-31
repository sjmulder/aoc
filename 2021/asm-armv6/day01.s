	.data

scan_fmt:	.asciz " %d"
print_fmt:	.asciz "01: %d %d\n"
num:		.int 0 

	.text
	.global main
	.extern scanf

main:
	push  {lr}
	mov   r4,  #0		// count
	mov   r5,  #0		// ninc1
	mov   r6,  #0		// ninc3
	mov   r7,  #0x7FFFFFFF	// nums
	mov   r8,  #0x7FFFFFFF
	mov   r9,  #0x7FFFFFFF
	mov   r10, #0x7FFFFFFF
_loop:
	ldr   r0, =scan_fmt
	ldr   r1, =num
	bl    scanf
	cmp   r0, #1
	bne   _end
	mov   r7, r8
	mov   r8, r9
	mov   r9, r10
	ldr   r10, =num
	ldr   r10, [r10]
	add   r4, #1
	cmp   r10, r9
	addgt r5, #1
	cmp   r10, r7
	addgt r6, #1
	b     _loop
_end:
	ldr   r0, =print_fmt
	mov   r1, r5
	mov   r2, r6
	bl    printf
	mov   r0, #0
	pop   {lr}
