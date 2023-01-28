comment |

  rand.asm

public domain, linear congruential pseudo-random
number generator routine by Jerry Coffin.

These do have a bit of a defect - there's one bit that actually
is predictable - the two rotates at the end of rand bury it in
the middle of the number instead of the end where it causes
numbers to alternate between odd and even.  As is, in binary you
can still see the alternation, but when converted to decimal, it
generally isn't too obvious.  An alternative is to use a 32 bit
generator and only use 16 bits of what it produces.  With 16 bit
registers, this is quite a bit more work, and for light duty use,
this one seems to be adequate. For serious statistical analysis,
you should probably investigate other algorithms.

srand should be called before rand is used, but needs only to be
called once at the beginning of the program.  It uses the seconds
and hundredths of a second read from DOS to seed the generator
which is relatively random due to the imprecision of the clock,
but as noted above, it's likely not particularly good for serious
statistical analysis but seems to work fine for games and such.

Assemble with a definition to tell it what memory model to use, as in:
                                                                      
tasm /Dmemmodel=small /mx rand                                        
                                                                      
As is, these will assemble to produce names that a c compiler         
will recognize (lower case with an underscore before the              
function name.) If you wish to call from BASIC, Pascal, etc.          
change the language on the model line to suit...                      

|

%.model memmodel,c

.data
k1      dw      9821
k2      dw      1
;
; These two values are relatively critical to the quality of
; number generated.  They were picked in accordance with Knuth
; Vol. 2, which you should likely read youself if you want to
; modify this or produce another of your own...
;
seed    dw      ?

.code

        public rand
rand proc
;
; returns a 16 bit semi-poor quality pseudo-random number in AX.
;
; destroys AX and DX.
;
        mov     ax,seed
        imul    k1
        add     ax,k2
        mov     seed,ax
        ror     al,1
        rol     ah,1
        ret
rand endp

        public srand
srand proc
;
; Seeds rand from the DOS clock.
;
        mov     ah,2ch
        int     21h
        mov     seed,dx
        ret
srand   endp
        end
