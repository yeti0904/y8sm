mov r1, 0
mov r2, 1
mov r3, 5

loop:
	add r1, r2
	rcp r1, ac
	cmp r1, r3
	mvi pc, loop
	mov pc, done

done:
	hlt