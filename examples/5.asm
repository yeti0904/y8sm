mov r1, 0 ; 3
mov r2, 1 ; 6
mov r3, 5 ; 9
count:
	add r1, r2 ; 12
	rcp r1, ac ; 15
	cmp ac, r3 ; 18
	not ac ; 20
	mvi pc, count ; 24

hlt ; 25