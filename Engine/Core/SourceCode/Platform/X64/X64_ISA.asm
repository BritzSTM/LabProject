COMMENT !
 CPU에서 명령어를 지원하는지 확인하기 위한 어셈블리 함수들을 정의한 파일.
 X64는 inline asm이 허용 되지 않기 때문에 따로 오브젝트 파일을 생성하여 링킹을 진행한다.
 
 다음 Reference를 참고 : https://software.intel.com/sites/default/files/4f/5b/36945
 LAST UPDATE : 2018/ 07/ 19/ PM 21:03
!

;.686P
;.modle flat, stdcall
;option casemap:none

.DATA

 COMMENT !
  Boolean을 반환하기 위해 설정하는 매크로 함수이다.
  현재 Windows x64는 al에 설정을 한다.
 !

 ; Boolean Type
 FALSE equ 0
 TRUE  equ 1

 SET_BOOL MACRO Flag
 mov al, Flag
 ENDM


 PUSHAQ_64 MACRO
    db   048H       ; emit a REX prefix, to enable hot-patching  
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
 ENDM

 POPAQ_64 MACRO
    pop rdi    
    pop rsi    
    pop rbp    
    pop rbx    
    pop rdx    
    pop rcx
    pop rax
 ENDM


 FUNC_RETURN_BOOL_START MACRO

    PUSHAQ_64
    pushfq

 ENDM


 FUNC_RETURN_BOOL_END MACRO         ; 종료에 대한 매크로 집합 함수 마지막 부분에 위치 해야 합니다.

 RETURN_FALSE:
	popfq
    POPAQ_64
    SET_BOOL FALSE
	ret

 RETURN_TRUE:
	popfq
    POPAQ_64
	SET_BOOL TRUE
	ret

 ENDM


.CODE

__IsSupportSSE_ASM PROC     ; 프로세서가 SSE명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

	; rax 1
	; rdx.fxsr[bit 24] | rdx.sse[bit 25]      => 0300,0000H
    mov rax, 1
	cpuid

	and rdx, 03000000H
	cmp rdx, 03000000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportSSE_ASM ENDP
	

__IsSupportSSE2_ASM PROC    ; 프로세서가 SSE2명령어를 지원하는지 확인합니다.
    
	FUNC_RETURN_BOOL_START

	; rax 1
	; rdx.clfsh[bit 19] | rdx.sse2[bit 26]    => 0408,0000H
	mov rax, 1
	cpuid

	and rdx, 04080000H
	cmp rdx, 04080000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportSSE2_ASM ENDP


__IsSupportSSE3_ASM PROC    ; 프로세서가 SSE3명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

    ; rax 1
    ; rcx.sse3[bit 0] | rcx.monitor[bit 3]    => 0000,0009H
	mov rax, 1
	cpuid

	and rcx, 00000009H
	cmp rcx, 00000009H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportSSE3_ASM ENDP


__IsSupportSSSE3_ASM PROC   ; 프로세서가 SSSE3명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

    ; rax 1
    ; rcx.ssse3[bit 9]                        => 0000,0200H
	mov rax, 1
	cpuid

	and rcx, 00000100H
	cmp rcx, 00000100H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportSSSE3_ASM ENDP


__IsSupportSSE4_1_ASM PROC  ; 프로세서가 SSE4_1명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

    ; rax 1
    ; rcx.sse4_1[bit 19]                      => 0008,0000H
    mov rax, 1
	cpuid

	and rcx, 00080000H
	cmp rcx, 00080000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportSSE4_1_ASM ENDP


__IsSupportSSE4_2_ASM PROC  ; 프로세서가 SSE4_2명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START
    ; rax 1
    ; rcx.sse4_2[bit 20] | rcx.popcnt[bit 23] => 0090,0000H
	mov rax, 1
	cpuid

	and rcx, 00900000H
	cmp rcx, 00900000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

    FUNC_RETURN_BOOL_END

__IsSupportSSE4_2_ASM ENDP


__IsSupportFP16_ASM PROC    ; 프로세서가 FP16명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

    ; rax 1
    ; rcx.fp16[bit 29]                        => 2000,0000H
	mov rax, 1
	cpuid

	and rcx, 20000000H
	cmp rcx, 20000000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportFP16_ASM ENDP


__IsSupportMOVBE_ASM PROC   ; 프로세서가 movbe명령어를 지원하는지 확인합니다.

    FUNC_RETURN_BOOL_START

    ; rax 1
    ; rcx.movbe[bit 22]                       => 0040,0000H
	mov rax, 1
	cpuid

	and rcx, 00400000H
	cmp rcx, 00400000H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportMOVBE_ASM ENDP


__IsSupportAVX_ASM PROC     ; 프로세서가 AVX명령어를 지원하는지 확인하는 함수
    
	FUNC_RETURN_BOOL_START

	mov rax, 1
	cpuid

	;ecx.OSXSAVE[bit 27] | ecx.AVX[bit 28] = 018000000H
	and rcx, 018000000H
	cmp rcx, 018000000H

	jne RETURN_FALSE

	;OS에서 XMM, YMM을 지원하는지 확인
	;ecx = 0 => XEFATURE_ENABLED_MASK
	mov rcx, 0
	XGETBV                   ; Result in EDX:rax

	and rax, 06H
	cmp rax, 06H

	jne RETURN_FALSE

	; 명령어를 지원함
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportAVX_ASM ENDP


__IsSupportAVX2_ASM PROC   ; 프로세서가 AVX2명령어를 지원하는지 확인하는 함수

    FUNC_RETURN_BOOL_START

    ; 기본적으로 AVX명령어 셋을 지원해야 한다
	call __IsSupportAVX_ASM
	cmp rax, TRUE

	jne RETURN_FALSE

	; AVX2지원 확인
	mov rax, 07H
	mov rcx, 00H
	cpuid

	; Check Flag
	and ebx, 20H
	cmp ebx, 20H

	jne RETURN_FALSE

	; 모든 테스트 통과
	jmp RETURN_TRUE

	FUNC_RETURN_BOOL_END

__IsSupportAVX2_ASM ENDP

END