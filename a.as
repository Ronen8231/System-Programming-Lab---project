MAIN: mov @r3, 12
LOOP: jmp 1
 prn -5
 no_such_op 1, 3                    
2absd: prn 3
 bne LOOP
 sub @r1 ,      @r4
 bne END
L1: inc K, 7
 bne LOOP
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
