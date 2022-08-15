.entry LOOP
.entry LENGTH
.extern L3 
TOM: mov S1.1
 add r2,STR 
LOOP: jmp L3
macro m3 
 inc K
 mov S1.2 ,r3
endmacro 
 prn #-5 
 sub r1, r4 
 bne LOOP
 bne L3 
 m3
STR: .string "project" 
LENGTH: .data 10,-9,15 
K: .data 22
S1: .struct 8,"tal"
rts