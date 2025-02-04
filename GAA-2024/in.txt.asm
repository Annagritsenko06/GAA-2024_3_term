.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../Debug/StaticLib1.lib
ExitProcess PROTO:DWORD 
.stack 4096


 outnum PROTO : DWORD

 outstr PROTO : DWORD

 strcopy PROTO : DWORD, : DWORD

 mystrlen PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte 'Result 1:   ', 0
		LTRL2 byte 'Result 2:   ', 0
		LTRL3 sdword 5
		LTRL4 byte 'Lenght + 5:          ', 0
		LTRL5 byte 'pooooop', 0
		LTRL6 byte 'String copy:', 0
		LTRL7 byte 'Subtraction Result:  ', 0
		LTRL8 byte 'GCD: ', 0
		LTRL9 sdword 9
		LTRL10 db 'o', 0; Символьный литерал
		LTRL11 db 1; Булевый литерал
		LTRL12 sdword 2
		LTRL13 sdword 11
		LTRL14 sdword 11
		LTRL15 sdword 255
		LTRL16 sdword 9
		LTRL17 byte '345', 0
		LTRL18 byte 'pppppppp', 0
		LTRL19 byte 'Lenght sb:', 0
		LTRL20 sdword 125
		LTRL21 sdword 1
		LTRL22 byte ';', 0
		LTRL23 sdword 3
.data
		temp sdword ?
		buffer byte 256 dup(0)
		countz sdword 0
		wooz sdword 0
		book sdword 0
		boostr dword ?
		boost dword ?
		subtresult sdword 0
		gcdtemp sdword 0
		mains db ?; Символьная переменная
		mainl db ?; Булевый тип
		mainx sdword 0
		mainy sdword 0
		maint sdword 0
		mainw sdword 0
		mainh sdword 0
		maing sdword 0
		mainz sdword 0
		mainsa dword ?
		mainsb dword ?
		mainnum sdword 0
		mainab sdword 0
.code

;------------- count --------------
count PROC,
	countx : sdword, county : sdword  
; -------- save registers -------
push ebx
push edx
; -------------------------------
push countx
push countx
push county
pop ebx
pop eax
add eax, ebx
push eax
pop ebx
pop eax
imul eax, ebx
push eax

pop ebx
mov countz, ebx


push offset LTRL1
call outstr

push offset newline

call outstr


push countz
call outnum

push offset newline

call outstr

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, countz
ret
count ENDP
;---------------


;------------- woo --------------
woo PROC,
	woox : sdword, wooy : sdword  
; -------- save registers -------
push ebx
push edx
; -------------------------------
push woox
push woox
push wooy
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov wooz, ebx


push offset LTRL2
call outstr

push offset newline

call outstr


push wooz
call outnum

push offset newline

call outstr

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, wooz
ret
woo ENDP
;---------------


;------------- boo --------------
boo PROC,
	booa : dword, boob : dword  
; -------- save registers -------
push ebx
push edx
; -------------------------------

push booa
push offset buffer
call mystrlen
push eax
push LTRL3
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov book, ebx


push offset LTRL4
call outstr

push offset newline

call outstr


push book
call outnum

push offset newline

call outstr

mov boostr, offset LTRL5

push boostr
push offset buffer
call strcopy
mov boost, eax

push offset LTRL6
call outstr

push offset newline

call outstr


push boostr
call outstr

push offset newline

call outstr

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, book
ret
boo ENDP
;---------------


;------------- subt --------------
subt PROC,
	subtx : sdword, subty : sdword  
; -------- save registers -------
push ebx
push edx
; -------------------------------
push subtx
push subty
pop ebx
pop eax
sub eax, ebx
push eax

pop ebx
mov subtresult, ebx


push offset LTRL7
call outstr

push offset newline

call outstr


push subtresult
call outnum

push offset newline

call outstr

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, subtresult
ret
subt ENDP
;---------------


;------------- gcd --------------
gcd PROC,
	gcda : sdword, gcdb : sdword  
; -------- save registers -------
push ebx
push edx
; -------------------------------

push offset LTRL8
call outstr

push offset newline

call outstr

mov edx, gcdb
cmp edx, LTRL9

jle cycle1
jmp cyclenext1
cycle1:
push gcdb

pop ebx
mov gcdtemp, ebx

push gcda
push gcdb
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov gcdb, ebx

push gcdtemp

pop ebx
mov gcda, ebx


push gcda
call outnum

push offset newline

call outstr

mov edx, gcdb
cmp edx, LTRL9

jle cycle1
cyclenext1:
; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, gcda
ret
gcd ENDP
;---------------


;------------- MAIN --------------
main PROC
push LTRL12

pop ebx
mov mainx, ebx

push LTRL13

pop ebx
mov mainy, ebx

push LTRL14

pop ebx
mov maint, ebx

push maint
push mainy
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov mainw, ebx


push mainw
call outnum

push offset newline

call outstr

push LTRL15

pop ebx
mov mainh, ebx

push LTRL16

pop ebx
mov maing, ebx

mov mainsa, offset LTRL17
mov mainsb, offset LTRL18

push mainsb
push offset buffer
call mystrlen
push eax

pop ebx
mov mainnum, ebx


push offset LTRL19
call outstr

push offset newline

call outstr


push mainnum
call outnum

push offset newline

call outstr


push mainy
push mainx
call count
push eax

pop ebx
mov mainz, ebx


push mainsb
push mainsa
call boo


push maing
push mainx
call woo


push mainy
push mainx
call subt


push maing
push mainh
call gcd

push LTRL20

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL21

jg cycle2
jmp cyclenext2
cycle2:

push mainab
call outnum

push offset newline

call outstr


push offset LTRL22
call outstr

push offset newline

call outstr

push mainab
push LTRL23
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL21

jg cycle2
cyclenext2:

push mainab
call outnum

push offset newline

call outstr

push 0
call ExitProcess
main ENDP
end main
