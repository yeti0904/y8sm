mov r1, 0
mov r2, 1
mov r3, 5
count:
	add r1, r2
	rcp r1, ac
	cmp ac, r3
	not ac
	mvi pc, count

hlt