; StringConversion.s
; Student names: Yunjae Jo, Hyukjoo Chung
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; ECE319K lab 7 number to string conversions
;
; Used in ECE319K Labs 7,8,9,10. You write these two functions

 
    EXPORT   Dec2String
    EXPORT   Fix2String
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8

  
;-----------------------Dec2String-----------------------
; Convert a 32-bit number into unsigned decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit unsigned number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
Dec2String
	PUSH {R4-R11}
	MOV R2, #10
	MOV R5, #10
	MOV R9,#0
HTD MOV R6, #1
	SUBS R2, R2, #1
	MOV R3, R2
DBP CMP R3, #0
	BEQ CHK
	MUL R6, R6, R5
	ADDS R3, R3, #-1
	B DBP
CHK	CMP R9,#0
	BNE ODL
	CMP R0,#0
	BEQ SPL
	CMP R0,R6
	BLO HTD
	ADDS R9,R9,#1
	
ODL UDIV R7, R0, R6
CNT	MUL R8, R7, R6
	ADD R7, R7, #48
	STRB R7, [R1]
	ADDS R1, R1, #1
	SUBS R0, R0, R8
	CMP R2, #0
	BNE HTD
	B TR
SPL	MOV R7,#48
	STRB R7,[R1]
	ADDS R1,R1,#1
TR	MOV R7, #0
	STRB R7, [R1]
	POP{R4-R11}
	
    BX LR
;* * * * * * * * End of Dec2String * * * * * * * *


; -----------------------Fix2String----------------------
; Create characters for LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
;          R1 pointer to empty array
; Outputs: none
; E.g., R0=0,    then create "0.000 "
;       R0=3,    then create "0.003"
;       R0=89,   then create "0.089"
;       R0=123,  then create "0.123"
;       R0=9999, then create "9.999"
;       R0>9999, then create "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Fix2String
	PUSH {R4-R11}
	LDR R2,=9999
	CMP R0,R2
	BHI ERR
	MOV R2,#1
	
	MOV R2,#4 ;UP TO 4 digit (9999)
	MOV R5,#10 ;base 10
HT2 MOV R6,#1
	SUBS R2,R2,#1
	MOV R3,R2

	CMP R3,#2
	BNE DB2
	MOV R8,#46
	STRB R8,[R1]
	ADDS R1,R1,#1
DB2 CMP R3,#0
	BEQ OD2
TD  MUL R6,R6,R5
	ADDS R3,R3,#-1
	B DB2
OD2	UDIV R7,R0,R6
	MUL R8,R7,R6
	ADD R7,R7,#48
	STRB R7,[R1]
	ADDS R1,R1,#1
	SUBS R0,R0,R8
	CMP R2,#0
	BNE HT2
	MOV R7,#32
	STRB R7,[R1]
	ADDS R1,R1,#1
	MOV R7,#0
	STRB R7,[R1]
	POP {R4-R11}
	BX LR

ERR	MOV R2,#42
	STRB R2,[R1]
	ADDS R1,R1,#1
	MOV R2,#46
	STRB R2,[R1]
	ADDS R1,R1,#1
	MOV R2,#42
	STRB R2,[R1]
	ADDS R1,R1,#1
	MOV R2,#42
	STRB R2,[R1]
	ADDS R1,R1,#1
	MOV R2,#42
	STRB R2,[R1]
	ADDS R1,R1,#1
	MOV R2,#0
	STRB R2,[R1]
	ADDS R1,R1,#1
	POP {R4-R11}
	BX LR
	


 


     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
