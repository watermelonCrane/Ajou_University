QUIZ	START	0x000000
INIT	LDA	ZERO
        LDX	ZERO
        STA	LEN
        STA	TMP
QLOOP	LDCH	STR1, X	. A레지스터에 STR1[X]를 로드
        COMP	DET	. C'Z'보다 큰지 비교
        JGT	STEP	. C'Z' 보다 크다면(대문자가 아니라면) QLOOP 이후 STEP으로 점프
        STX	TMP	. 임시공간에 X값 저장
        LDX	LEN	. X레지스터에 현재 STR2 길이 로드
        STCH	STR2, X . STR2에 A의 값 저장 (6번째 줄에서 로드한 값)
        LDA	LEN
        ADD	ONE
        STA	LEN	. LEN++
        LDX	TMP
STEP	TIX	MAX	. 반복 조건 체크 X가 11일 때까지 반복
        JLT	QLOOP


HALT 	J	HALT

ZERO	WORD	0
ONE	WORD	1
DET	WORD	90	. C'Z'로 설정
MAX	WORD	17	. MAX 상수를 17로 설정, 반복문 횟수 설정 용도. QLOOP 반복문 17회 반복
TMP	RESW	1	. 임시저장 공간 1 Word
LEN	RESW	1	. STR2의 길이 저장 공간
STR1	BYTE	C'syStemPRogrammING'	.총 17글자
STR2	RESB	17	. 최대 17글자를 저장할 수 있는 예약공간