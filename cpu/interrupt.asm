; C fucntion to handle the interrupt request
[extern interrupt_handler]
[extern interrupt_request_handler]

; Common Interrupt Service Routine
isr_common:
    cli ; Disable interrupts
    ; Save the registers
    pusha
    mov ax, ds ; Save the data segment
    push eax; Save the data segment
    mov ax, 0x10 ; Set the data segment to 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the interrupt handler
    push esp ; Push the stack pointer
    call interrupt_handler
    pop eax ; Pop the stack pointer

    ; Restore the registers
    pop eax
    mov eax, 1
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa

    ;Clean the pushed error code
    add esp, 0x08


    ; Send the EOI
    mov al, 0x20
    out 0x20, al

    ;sti ; Enable interrupts


    iret ; Pops CS, EIP, EFLAGS, SS, ESP

; Interrupt Service Routine for IRQ0
irq_common:
    ; Save the registers
    pusha
    mov ax, ds ; Save the data segment
    push eax; Save the data segment
    mov ax, 0x10 ; Set the data segment to 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the interrupt handler
    push esp ; Push the stack pointer
    call interrupt_request_handler
    pop ebx ; Pop the stack pointer

    ; Restore the registers
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret ; Pops CS, EIP, EFLAGS, SS, ESP

; Global ISR
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; 0: Divide by zero
isr0:
    push byte 0
    push byte 0
    jmp isr_common

; 1: Single step
isr1:
    push byte 0
    push byte 1
    jmp isr_common

; 2: Non maskable interrupt
isr2:
    push byte 0
    push byte 2
    jmp isr_common

; 3: Breakpoint
isr3:
    push byte 0
    push byte 3
    jmp isr_common

; 4: Overflow
isr4:
    push byte 0
    push byte 4
    jmp isr_common

; 5: Out of bounds
isr5:
    push byte 0
    push byte 5
    jmp isr_common

; 6: Invalid opcode
isr6:
    push byte 0
    push byte 6
    jmp isr_common

; 7: No coprocessor
isr7:
    push byte 0
    push byte 7
    jmp isr_common

; 8: Double fault (With Error Code)
isr8:
    push byte 8
    jmp isr_common

; 9: Coprocessor segment overrun
isr9:
    push byte 0
    push byte 9
    jmp isr_common

; 10: Bad TSS (With Error Code)
isr10:
    push byte 10
    jmp isr_common

; 11: Segment not present (With Error Code)
isr11:
    push byte 11
    jmp isr_common

; 12: Stack fault (With Error Code)
isr12:
    push byte 12
    jmp isr_common

; 13: General protection fault (With Error Code)
isr13:
    push byte 13
    jmp isr_common

; 14: Page fault (With Error Code)
isr14:
    push byte 14
    jmp isr_common

; 15: Unknown interrupt
isr15:
    push byte 0
    push byte 15
    jmp isr_common

; 16: Floating point error
isr16:
    push byte 0
    push byte 16
    jmp isr_common

; 17: Alignment check
isr17:
    push byte 0
    push byte 17
    jmp isr_common

; 18: Machine check
isr18:
    push byte 0
    push byte 18
    jmp isr_common

; 19: SIMD floating point error
isr19:
    push byte 0
    push byte 19
    jmp isr_common

; 20: Virtualization exception
isr20:
    push byte 0
    push byte 20
    jmp isr_common

; 21: ControlProtectionException
isr21:
    push byte 0
    push byte 21
    jmp isr_common

; 22: Reserved
isr22:
    push byte 0
    push byte 22
    jmp isr_common

; 23: Reserved
isr23:
    push byte 0
    push byte 23
    jmp isr_common

; 24: Reserved
isr24:
    push byte 0
    push byte 24
    jmp isr_common

; 25: Reserved
isr25:
    push byte 0
    push byte 25
    jmp isr_common

; 26: Reserved
isr26:
    push byte 0
    push byte 26
    jmp isr_common

; 27: Reserved
isr27:
    push byte 0
    push byte 27
    jmp isr_common

; 28: HypervisorInjectionException
isr28:
    push byte 0
    push byte 28
    jmp isr_common

; 29: VMMCommunicationException
isr29:
    push byte 0
    push byte 29
    jmp isr_common

; 30: Security Exception
isr30:
    push byte 0
    push byte 30
    jmp isr_common

; 31: Reserved
isr31:
    push byte 0
    push byte 31
    jmp isr_common


; Global IRQ
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; IRQ handlers

; 0: Timer
irq0:
    push byte 0
    push byte 32
    jmp irq_common

; 1: Keyboard
irq1:
    push byte 0
    push byte 33
    jmp irq_common

; 2: Cascade
irq2:
    push byte 0
    push byte 34
    jmp irq_common

; 3: COM2
irq3:
    push byte 0
    push byte 35
    jmp irq_common

; 4: COM1
irq4:
    push byte 0
    push byte 36
    jmp irq_common

; 5: LPT2
irq5:
    push byte 0
    push byte 37
    jmp irq_common

; 6: Floppy Disk
irq6:
    push byte 0
    push byte 38
    jmp irq_common

; 7: LPT1
irq7:
    push byte 0
    push byte 39
    jmp irq_common

; 8: CMOS real time clock
irq8:
    push byte 0
    push byte 40
    jmp irq_common

; 9: Free for peripherals / legacy SCSI / NIC
irq9:
    push byte 0
    push byte 41
    jmp irq_common

; 10: Free for peripherals / SCSI / NIC
irq10:
    push byte 0
    push byte 42
    jmp irq_common

; 11: Free for peripherals / SCSI / NIC
irq11:
    push byte 0
    push byte 43
    jmp irq_common

; 12: PS2 Mouse
irq12:
    push byte 0
    push byte 44
    jmp irq_common

; 13: FPU / Coprocessor / Inter-processor
irq13:
    push byte 0
    push byte 45
    jmp irq_common

; 14: Primary ATA Hard Disk
irq14:
    push byte 0
    push byte 46
    jmp irq_common

; 15: Secondary ATA Hard Disk
irq15:
    push byte 0
    push byte 47
    jmp irq_common