
; sumarray(), a Borland C++-callable function to sum the values
; in an array of floats.

; Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.

.MODEL  LARGE,C

.CODE

; double sumarray(float *arr, int n);
;   Returns sum of the n elements in arr.

    PUBLIC  sumarray

sumarray    PROC    ARR:DWORD,N:WORD
    les bx,ARR      ; ES:BX <- pointer to arr
    mov cx,N        ; Get number of elements
    fldz            ; Load zero
    jcxz    LeaveSum    ; Leave if count is 0.
DoSum:  fadd    dword ptr es:[bx] ; Add value in array to ST(0).
    add bx,4        ; Point to next value in array.
    loop    DoSum       ; Loop until done.
LeaveSum:           ; Return value in ST(0).
    ret
sumarray    ENDP

    END
