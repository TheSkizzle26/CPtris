PC_CC := clang

CALC_CC := sh4-elf-gcc
OBJCOPY := sh4-elf-objcopy
ASM := sh4-elf-as

CC_FLAGS := -std=c99 -Wall -Wextra -O3 -fwhole-program -flto -I../cplib
LD_FLAGS := -nostartfiles -nostdlib -T linker.ld

OUT_PC := cptris.elf
OUT_CALC := cptris.hhk

calc:
	$(ASM) ../cplib/cp_calc_div.s ../cplib/cp_calc_getKey.s -o cp_calc.o
	$(CALC_CC) $(CC_FLAGS) $(LD_FLAGS) -ffreestanding -m4a-nofpu -D SYSTEM_CALC main.c cp_calc.o -o $(OUT_CALC)
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(OUT_CALC) $(OUT_CALC)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(OUT_CALC) $(OUT_CALC)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(OUT_CALC) $(OUT_CALC)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(OUT_CALC) $(OUT_CALC)

pc:
	$(PC_CC) $(CC_FLAGS) -lraylib -D SYSTEM_PC main.c -o $(OUT_PC)

debug:
	$(PC_CC) $(CC_FLAGS) -Og -fno-inline -fsanitize=address -lraylib -D SYSTEM_PC main.c -o $(OUT_PC)

run: pc
	./$(OUT_PC)

.PHONY: calc pc run
