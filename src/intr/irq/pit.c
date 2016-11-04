#include <intr/irq/pit.h>
#include <io/vid.h>
#include <intr.h>

static u64 pit_ticks;

extern void irq_pit_hand(void);


static inline u16 get_reload(u32 freq) {
    if(freq < 18)      return 0xFFFF; // Frequency is too low
    if(freq > 1193181) return 0x0001; // Frequency is too high
    return (u16)(1193181 / freq);
}

void pit_init(u32 freq) {
    u16 reload = get_reload(freq);
    outb(0x43, 0x34);
    outb(0x40, (u8)reload);
    outb(0x40, (u8)(reload >> 8));
    set_irq_handler(IRQ_PIT, (u32)&irq_pit_hand);
    irq_enable(IRQ_PIT);
    pic_send_eoi(IRQ_PIT);
}

void pit_handler(void) __hot;

void pit_handler() {
    pit_ticks++;
    pic_send_eoi(IRQ_PIT);
}
