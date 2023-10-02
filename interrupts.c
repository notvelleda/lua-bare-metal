#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "interrupts.h"

/* http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html */
static void make_idt_entry(struct idt_entry *entry, uint32_t base, uint16_t sel, uint8_t flags) {
    entry->base_lo = base & 0xFFFF;
    entry->base_hi = (base >> 16) & 0xFFFF;

    entry->sel     = sel;
    //entry->always0 = 0;
    entry->flags   = flags;
} 

void init_idt() {
    struct idt_entry *idt = malloc(sizeof(struct idt_entry) * IDT_ENTRIES);

    if (idt == NULL) {
        printf("couldn't allocate memory!");
        while(1);
    }

    printf("allocated IDT at %08x\n", idt);

    memset(idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);

    make_idt_entry(&idt[0], (uint32_t) isr0, 0x08, 0x8e);
    make_idt_entry(&idt[1], (uint32_t) isr1, 0x08, 0x8e);
    make_idt_entry(&idt[2], (uint32_t) isr2, 0x08, 0x8e);
    make_idt_entry(&idt[3], (uint32_t) isr3, 0x08, 0x8e);
    make_idt_entry(&idt[4], (uint32_t) isr4, 0x08, 0x8e);
    make_idt_entry(&idt[5], (uint32_t) isr5, 0x08, 0x8e);
    make_idt_entry(&idt[6], (uint32_t) isr6, 0x08, 0x8e);
    make_idt_entry(&idt[7], (uint32_t) isr7, 0x08, 0x8e);
    make_idt_entry(&idt[8], (uint32_t) isr8, 0x08, 0x8e);
    make_idt_entry(&idt[9], (uint32_t) isr9, 0x08, 0x8e);
    make_idt_entry(&idt[10], (uint32_t) isr10, 0x08, 0x8e);
    make_idt_entry(&idt[11], (uint32_t) isr11, 0x08, 0x8e);
    make_idt_entry(&idt[12], (uint32_t) isr12, 0x08, 0x8e);
    make_idt_entry(&idt[13], (uint32_t) isr13, 0x08, 0x8e);
    make_idt_entry(&idt[14], (uint32_t) isr14, 0x08, 0x8e);
    make_idt_entry(&idt[15], (uint32_t) isr15, 0x08, 0x8e);
    make_idt_entry(&idt[16], (uint32_t) isr16, 0x08, 0x8e);
    make_idt_entry(&idt[17], (uint32_t) isr17, 0x08, 0x8e);
    make_idt_entry(&idt[18], (uint32_t) isr18, 0x08, 0x8e);
    make_idt_entry(&idt[19], (uint32_t) isr19, 0x08, 0x8e);
    make_idt_entry(&idt[20], (uint32_t) isr20, 0x08, 0x8e);
    make_idt_entry(&idt[21], (uint32_t) isr21, 0x08, 0x8e);
    make_idt_entry(&idt[22], (uint32_t) isr22, 0x08, 0x8e);
    make_idt_entry(&idt[23], (uint32_t) isr23, 0x08, 0x8e);
    make_idt_entry(&idt[24], (uint32_t) isr24, 0x08, 0x8e);
    make_idt_entry(&idt[25], (uint32_t) isr25, 0x08, 0x8e);
    make_idt_entry(&idt[26], (uint32_t) isr26, 0x08, 0x8e);
    make_idt_entry(&idt[27], (uint32_t) isr27, 0x08, 0x8e);
    make_idt_entry(&idt[28], (uint32_t) isr28, 0x08, 0x8e);
    make_idt_entry(&idt[29], (uint32_t) isr29, 0x08, 0x8e);
    make_idt_entry(&idt[30], (uint32_t) isr30, 0x08, 0x8e);
    make_idt_entry(&idt[31], (uint32_t) isr31, 0x08, 0x8e);

    struct idt_ptr idt_ptr = {
        .limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1,
        .base = (uint32_t) idt
    };

    __asm__ __volatile__ (
        "lidt %0"
        :
        : "m" (idt_ptr)
    );
}

/* https://wiki.osdev.org/Exceptions */
const char *interrupt_names[32] = {
    "division error",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "overflow",
    "bound range exceeded",
    "invalid opcode",
    "device not available",
    "double fault",
    "coprocessor segment overrun",
    "invalid TSS",
    "segment not present",
    "stack segment fault",
    "general protection fault",
    "page fault",
    "reserved",
    "x87 floating point exception",
    "alignment check",
    "machine check",
    "SIMD floating point exception",
    "virtualization exception",
    "control protection exception",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "hypervisor injection exception",
    "VMM communication exception",
    "security exception",
    "reserved"
};

const char *get_exception_name(uint32_t exception) {
    if (exception < 32)
        return interrupt_names[exception];
    else
        return "unknown";
}

void isr_handler(struct int_registers registers) {
    switch (registers.int_no) {
        case 3:
            printf("caught breakpoint interrupt!\n");
            break;
        default:
            printf("fatal exception %d (%s) @ %08x, error code %08x\n", registers.int_no, get_exception_name(registers.int_no), registers.eip, registers.err_code);
            printf("eax = %08x, ebx = %08x, ecx = %08x, edx = %08x\n", registers.eax, registers.ebx, registers.ecx, registers.edx);
            printf("esi = %08x, edi = %08x, ebp = %08x, esp = %08x\n", registers.esi, registers.edi, registers.ebp, registers.useresp);
            printf("eip = %08x, efl = %08x\n", registers.eip, registers.eflags);
            printf("cs = %04x, ds = %04x, ss = %04x\n", registers.cs, registers.ds, registers.ss);

            while (1)
                __asm__ __volatile__ ("cli; hlt");
    }
}