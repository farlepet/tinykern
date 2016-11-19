#include <intr/exceptions.h>
#include <intr/idt.h>

extern void excep_df(void);
extern void excep_gpf(void);
extern void excep_pf(void);
extern void excep_ts(void);

void exceptions_init() {
    idt_create_entry(&IDT[EXCEPT_DF],  (u32)excep_df,  0x08, IDT_FLAG_TYPE_INTR32 | IDT_FLAG_PRIV_RING0 | IDT_FLAG_PRESENT);
    idt_create_entry(&IDT[EXCEPT_GPF], (u32)excep_gpf, 0x08, IDT_FLAG_TYPE_INTR32 | IDT_FLAG_PRIV_RING0 | IDT_FLAG_PRESENT);
    idt_create_entry(&IDT[EXCEPT_PF],  (u32)excep_pf,  0x08, IDT_FLAG_TYPE_INTR32 | IDT_FLAG_PRIV_RING0 | IDT_FLAG_PRESENT);
    idt_create_entry(&IDT[EXCEPT_TS],  (u32)excep_ts,  0x08, IDT_FLAG_TYPE_INTR32 | IDT_FLAG_PRIV_RING0 | IDT_FLAG_PRESENT);
}

