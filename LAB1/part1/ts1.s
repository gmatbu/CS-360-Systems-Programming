.global getebp

# Points to the current top of the stack.
get_esp:		
	movl	%esp, %eax
	ret

# Points to the stack Frame of the current active function.
getebp:		
	movl	%ebp, %eax
	ret
	

