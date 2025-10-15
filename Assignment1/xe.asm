QUIZ	START	0x000000
INIT	LDT	#0	. STR2 문자열의 길이
        LDX	#0
        

LOOP	LDCH	STR1, X
        COMP	#90	.C'Z'보다 큰지 비교 (대문자인지 비교함)
        JGT	STEP
        RMO	X, S	.S에 X 값 임시 저장
        RMO	T, X	.X에 T 값 이동
        STCH	STR2, X	.STR2에 A에 저장된 문자 저장
        RMO	X, A	.A에 X 값 임시 이동
        ADD	#1	.A의 값 1증가
        RMO	A, T	.T에 A 값 저장 (STR2의 문자열 길이 T가 1증가한다.)
        RMO	S, X	.S에 임시 저장한 값(STR1 인덱스) 다시 X에 저장

STEP	TIX	MAX	. MAX와 루프 조건 비교
        JLT	LOOP

HALT	J	HALT

STR1 	BYTE	C'syStemPRogrammING'
STR2	RESB	17	. 최대 17글자 저장가능한 공간 예약
MAX	WORD	17	. 17번줄 compare시 사용할 값

        END	INIT