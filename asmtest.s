.macro movi reg, val
	lui \reg, %hi(\val)
	ori \reg, \reg, %lo(\val)
.endm

.section .text

.align 4
.global answer_to_life_the_universe_and_everything
answer_to_life_the_universe_and_everything:
	movi	$v0, 42
	jr	$ra
