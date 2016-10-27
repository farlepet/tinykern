MAINDIR    = $(CURDIR)
SRC        = $(MAINDIR)/src

SRCS       = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c) $(wildcard $(SRC)/*/*/*.c)
ASSRCS     = $(wildcard $(SRC)/*.s) $(wildcard $(SRC)/*/*.s) $(wildcard $(SRC)/*/*/*.s)
OBJS       = $(patsubst %.c,%.o,$(SRCS))
ASOBJS     = $(patsubst %.s,%.o,$(ASSRCS))

CC         = clang

CFLAGS     = -m32 -I$(MAINDIR)/inc -nostdlib -nostdinc -ffreestanding \
			 -Wall -Wextra -Werror -DARCH_X86 -pipe -g
ASFLAGS    = -m32
LDFLAGS    = -melf_i386 -T link_x86.ld

ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
CFLAGS    += -Weverything \
			 -Wno-date-time
endif




link:   $(OBJS) $(ASOBJS)
	@echo -e "\033[33m  \033[1mLinking kernel\033[0m"
	@ld $(LDFLAGS) -o tinykern.kern $(ASOBJS) $(OBJS)
	

iso: CD/boot/grub/stage2_eltorito
	@echo -e "\033[33m  \033[1mGenerating InitCPIO\033[0m"
	@cd initrd; find . | cpio -o -v -O../CD/initrd.cpio &> /dev/null
	@echo -e "\033[33m  \033[1mCreating ISO\033[0m"
	@cp tinykern.kern CD/tinykern.kern
	@genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 \
		-boot-info-table -o tinykern.iso CD
		

CD/boot/grub/stage2_eltorito:
	@echo -e "\033[33m	\033[1mDownloading GRUB stage 2 binary\033[0m"
	@curl -o CD/boot/grub/stage2_eltorito https://arabos.googlecode.com/files/stage2_eltorito

all: printinfo link
	



printinfo:
	@echo -e "\033[32mBuilding kernel\033[0m"



%.o: %.c
	@echo -e "\033[32m  \033[1mCC\033[21m  \033[34m$<\033[0m"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	@echo -e "\033[32m  \033[1mAS\033[21m  \033[34m$<\033[0m"
	@$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	@echo -e "\033[33m  \033[1mCleaning sources\033[0m"
	@rm -f $(OBJS) $(ASOBJS) $(LLVMBC)
	@rm -f tinykern.kern tinykern.o
	@rm -f CD/tinykern.kern CD/initrd.cpio tinykern.iso

