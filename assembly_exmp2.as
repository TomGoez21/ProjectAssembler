.entry LOOP
.entry LENGTH
.extern L3 
.extern W 
mov: mov S1.1 ,W 
 add r2,STR 
LOOP: jmp W
macro m1 
 inc K
 mov S1.2 ,r3
endmacro 
 prn #-5 
 sub r1, r4
m1 
 bne LOOP
 bne L3 
STR: hlt 
STR: .string "abcdef" 
LENGTH: .data a,-9,15 
K: .data 22 
S1: .struct 8, "ab" 
rts