MAINDIR    = $(CURDIR)
SRC        = $(MAINDIR)/src

SRCS       = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c) $(wildcard $(SRC)/*/*/*.c)
ASSRCS     = $(wildcard $(SRC)/*.s) $(wildcard $(SRC)/*/*.s) $(wildcard $(SRC)/*/*/*.s)
OBJS       = $(patsubst %.c,%.c.o,$(SRCS))
ASOBJS     = $(patsubst %.s,%.s.o,$(ASSRCS))

CC         = clang

CFLAGS     = -m32 -I$(MAINDIR)/inc -nostdlib -nostdinc -ffreestanding \
			 -Wall -Wextra -Werror -DARCH_X86 -pipe -g -mno-sse -O0 -fno-omit-frame-pointer
ASFLAGS    = -m32
LDFLAGS    = -melf_i386 -T link_x86.ld

ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
CFLAGS    += -Weverything \
			 -Wno-date-time -Wno-gnu-binary-literal -Wno-language-extension-token \
			 -Wno-reserved-id-macro -Wno-packed -Wno-bad-function-cast
endif


link:   $(OBJS) $(ASOBJS)
	@echo -e "\033[33m  \033[1mLinking kernel\033[0m"
	@ld $(LDFLAGS) -r -o tinykern.o $(ASOBJS) $(OBJS)
	@echo -e "\033[33m  \033[1mCreating symbol table\033[0m"
	@scripts/symbols > symbols.c
	@$(CC) $(CFLAGS) -c -o symbols.o symbols.c
	@ld $(LDFLAGS) -o tinykern.kern $(ASOBJS) $(OBJS) symbols.o
	
all: link


%.c.o: %.c
	@echo -e "\033[32m  \033[1mCC\033[21m  \033[34m$<\033[0m"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.s.o: %.s
	@echo -e "\033[32m  \033[1mAS\033[21m  \033[34m$<\033[0m"
	@$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	@echo -e "\033[33m  \033[1mCleaning sources\033[0m"
	@rm -f $(OBJS) $(ASOBJS) $(LLVMBC)
	@rm -f tinykern.kern tinykern.o
	@rm -f symbols.c symbols.o

