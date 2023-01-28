
; pow.asm: integer power function callable from Borland C++.
; Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.

.MODEL  LARGE,C

.CODE

; double intpow(double x, unsigned int y);
; Returns x^y.

    PUBLIC  intpow

intpow  PROC    X:QWORD,Y:WORD
    fld1            ; Load 1 into the 80x87
    mov cx,Y        ; Get y
    fld X       ; Load x into the 80x87

    jcxz    Return      ; If y already zero, return

TestY:  test    cx,1        ; Is the LSB of y set?
    jz  NextIteration   ; Jump if no
    fmul    st(1),st    ; ret *= x
NextIteration:
    fmul    st,st(0)    ; Square x
    shr cx,1        ; y >>= 1
    jnz TestY       ; Continue if nonzero
Return: fstp    st      ; Pop stack.  Return value is
                ; now in ST(0).
    ret
intpow  ENDP

    END
