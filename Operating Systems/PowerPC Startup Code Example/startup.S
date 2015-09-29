.global initStack
.global stackPointer
.global initRAM
.global atomic

.section .startup
initStack: 		
	addis 1, 0, stackPointer@h
	ori   1, 1, stackPointer@l
	addis 14, 0, 0x00000000
	stw 14, 0(1)			
	b initRAM
	
.section .text
atomic:
	lwarx  5,0,3  # first stack parameter (r3) into r5
	add    0,4,5  # add second stack parameter (r4) to r5 --> store in r0
	stwcx. 0,0,3  # store new value (r0) if still reserved
	bne-   atomic # loop if failed
	li     3,123  # return value (r3)
	blr
	