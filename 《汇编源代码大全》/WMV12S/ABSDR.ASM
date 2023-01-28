; bios call to do absolute disk read/write
; using int86 from C to call int 25h and 26h will freeze the computer
; (because si and di were modified?), so I had to write this in assembly.

_text	segment public byte 'code'
	assume cs:_text

	public	_absdr		; absolute disk read
_absdr	proc	near
	push	si
	push	di
	push	bp
	mov	bp,sp

	mov	al,[bp+8]	; drive number
	mov	cx,[bp+10]	; number of sectors to read
	mov	dx,[bp+12]	; beginning sector number
	mov	bx,[bp+14]	; buffer address
	int	25h		; absolute disk read
; int 25h supposed to destroy all registers except segment registers
	jc	error		; if error, leave error no in AX
	mov	ax,0		; clear error number if no error
error:
	inc	sp		; pop the flags
	inc	sp		; pop the flags
	pop	bp
	pop	di
	pop	si
	ret
_absdr	endp

	public	_absdw		; absolute disk write
_absdw	proc	near
	push	si
	push	di
	push	bp
	mov	bp,sp

	mov	al,[bp+8]	; drive number
	mov	cx,[bp+10]	; number of sectors to read
	mov	dx,[bp+12]	; beginning sector number
	mov	bx,[bp+14]	; buffer address
	int	26h		; absolute disk read
; int 26h supposed to destroy all registers except segment registers
	jc	error1		; if error, leave error no in AX
	mov	ax,0		; clear error number if no error
error1:
	inc	sp		; pop the flags
	inc	sp		; pop the flags
	pop	bp
	pop	di
	pop	si
	ret
_absdw	endp

_text	ends
	end
