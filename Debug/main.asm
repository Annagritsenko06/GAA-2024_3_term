.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../Debug/StaticLib1.lib
ExitProcess PROTO:DWORD 
.stack 4096


 outnum PROTO : DWORD

 outstr PROTO : DWORD

 copystr PROTO : DWORD, : DWORD

 mystrlen PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 sdword 9
		LTRL2 sdword 19
		LTRL3 sdword 0
		LTRL4 sdword 5
		LTRL5 byte 'Lenght + 5:          ', 0
		LTRL6 byte 'String copy:         ', 0
		LTRL7 db 'o', 0; Символьный литерал
		LTRL8 db 0; Булевый литерал
		LTRL9 sdword 77
		LTRL10 byte '345', 0
		LTRL11 byte '1234567', 0
		LTRL12 sdword 125
		LTRL13 sdword 1
		LTRL14 byte ';', 0
		LTRL15 sdword 3
.data
		temp sdword ?
		buffer byte 256 dup(0)
		countz sdword 0
		woor sdword 0
		wooo sdword 0
		wooe sdword 0
		book sdword 0
		boostr dword ?
		mains db ?; Символьная переменная
		mainl db ?; Булевый тип
		mainu sdword 0
		mainx sdword 0
		mainy sdword 0
		mainz sdword 0
		mainsa dword ?
		mainsb dword ?
		mainsc dword ?
		mainnum sdword 0
		mainstr dword ?
		mainresult sdword 0
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

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, countz
ret
count ENDP
;---------------


;------------- woo --------------
woo PROC 
	
; -------- save registers -------
push ebx
push edx
; -------------------------------
push LTRL1

pop ebx
mov woor, ebx

push LTRL2

pop ebx
mov wooo, ebx

push LTRL3

pop ebx
mov wooe, ebx

push woor
push wooo
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov wooe, ebx

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, wooe
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
push LTRL4
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov book, ebx


push offset LTRL5
call outstr


push book
call outnum


push boob
push offset buffer
call copystr
mov boostr, eax

push offset LTRL6
call outstr


push boostr
call outstr

; ------ restore registers ------
pop edx
pop ebx
; -------------------------------
mov eax, book
ret
boo ENDP
;---------------


;------------- MAIN --------------
main PROC
push LTRL9

pop ebx
mov mainu, ebx

push LTRL4

pop ebx
mov mainx, ebx

push LTRL1

pop ebx
mov mainy, ebx

mov mainsa, offset LTRL10
mov mainsb, offset LTRL11

push mystrlen
call outnum


push mainsb
push offset buffer
call mystrlen


push mainy
push mainx
call count
push eax

pop ebx
mov mainz, ebx


push mainsb
push mainsa
call boo


call woo


push mainz
call outnum


push mainsc
call outstr


push mainsb
push offset buffer
call copystr
mov mainstr, eax
push LTRL12

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL13

jg cycle1
jmp cyclenext1
cycle1:

push mainab
call outnum


push offset LTRL14
call outstr

push mainab
push LTRL15
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL13

jg cycle1
cyclenext1:

push mainab
call outnum

push 0
call ExitProcess
main ENDP
end main
