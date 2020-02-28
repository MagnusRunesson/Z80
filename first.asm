; ================================================================
;   Example source with target 'bin'
;   Copyright  (c)  Günter Woigk 1994 - 2017
;                   mailto:kio@little-bat.de
; ================================================================


; same as 'rom', except that the default fill byte for 'defs' et. al. is 0x00
; this example assumes that the code is loaded at address 0x8000 and that
; variables are stored at 0x5B00 upward, which may be used for the ZX Spectrum


#target bin


data_start  equ 0x5B00
code_start  equ 0x0000


#data   VARIABLES, data_start, 0x100

; define some variables here



#code   CODE, code_start
	nop
	nop
	nop
	nop

	ld bc,$80ff
	nop


;	ld (bc),a
;	; All things should start with 3 sweet nops
;	nop
;	nop
;	nop
;	nop

	; then some nice incs
;	inc a
;	inc b
;	inc c
;	inc a

	; some nops
;	nop
;	nop
;	nop
;	nop

;	; then load from rom
;	ld hl,$0004
;	nop
;	nop
;	nop
	ld a,0
	ld (bc),a
	nop

;loopie:
	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

	ld a,(bc)
	inc a
	ld (bc),a
	nop

;	jp loopie

	;ld b,(hl)
	nop

	; more sweet nops
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a
	inc a


; define some code here
;	jp $e0e0

	; Write to ROM
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a

	; Write to ROM
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a

	; Write to ROM
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a

	; Write to ROM
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a

	; Write to ROM
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a

	;ld hl,$0200
	;ld (hl),a

	;nop
	;ld hl,janne
	;ld a,(hl)

	;ld hl,$4000
	;ld (hl),a
	;ld hl,$2000
	;ld (hl),a
	;ld hl,$1000
	;ld (hl),a
	;ld hl,$0800
	;ld (hl),a
	;ld hl,$0400
	;ld (hl),a
	;ld hl,$0200
	;ld (hl),a

	; Write to RAM
	;ld hl,$8000
	;ld (hl),a


	;ld hl,$8000
	;ld (hl),a
	;ld hl,$8000
	;ld (hl),a
	;ld hl,$8000
	;ld (hl),a
	;ld hl,$8000
	;ld (hl),a


	;ld hl,$0100
	;ld (hl),a
	;ld hl,$0080
	;ld (hl),a
	;ld hl,$0040
	;ld (hl),a
	;ld hl,$0020
	;ld (hl),a
	;ld hl,$0010
	;ld (hl),a
	;ld hl,$0008
	;ld (hl),a
	;ld hl,$0004
	;ld (hl),a
	;ld hl,$0002
	;ld (hl),a
	;ld hl,$0001
	;ld (hl),a
	;ld a,(hl)
	;inc hl
	;ld (hl),a

;	jr  $

;janne:	.db	$7f
;output: .db	$30

;ramtest: .db 0
