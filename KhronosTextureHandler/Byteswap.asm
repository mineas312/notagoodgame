.code
;										RCX
;extern "C" uint32_t _SwapBytes32bit(uint32_t a)
_SwapBytes32bit proc
	bswap ecx
	mov eax, ecx
	ret
_SwapBytes32bit endp	
										;RCX
; extern "C" uint64_t _SwapBytes64bit(uint64_t a);
_SwapBytes64bit proc
	bswap rcx
	mov rax, rcx
	ret
_SwapBytes64bit endp
end