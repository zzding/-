https://web.eecs.utk.edu/~smarz1/courses/ece356/notes/assembly/
https://msyksphinz-self.github.io/riscv-isadoc/html/rv64i.html#sd
https://shakti.org.in/docs/risc-v-asm-manual.pdf

> - a0 − a7 are used for function arguments.

| Register Name  | ABI Name | Description |
| :----: | :---: | :---:|
|x0 | zero |Hard-Wired Zero|
|x1 | ra |Return Address|
|x2 | sp | Stack Pointer
|x3 | gp | Global Pointer
|x4 | tp | Thread Pointer
|x5 | t0 | Temporary/Alternate Link Register
|x6-7 | t1-t2 | Temporary Register
|x8 | s0/fp | Saved Register (Frame Pointer)
|x9 | s1 | Saved Register
|x10-11 | a0-a1 | Function Argument/Return Value Registers
|x12-17 | a2-a7 | Function Argument Registers
|x18-27 | s2-s11 | Saved Registers
|x28-31 | t3-t6 | Temporary Registers

# 1、sd
> sd ra, 40(a0)
sd 用于复制寄存器64bits的值到内存，40为a0的偏移量

# 2、csrrw
> csrrw a0, sscratch, a0
    
CSR的上一个值被复制到目标寄存器，源寄存器(rs1)的值被复制到CSR，这是一个原子写操作。  
相当于交换

# 3、li
load immediate    将立即数载入寄存器

# csrr csrw
- csrr，读取一个 CSR 的值到通用寄存器。如：csrr t0, mstatus，读取 mstatus 的值到 t0 中。
- csrw，把一个通用寄存器中的值写入 CSR 中。如：csrw mstatus, t0，将 t0 的值写入 mstatus

# sepc
"sepc" 是 RISC-V CPU 的一个控制寄存器，用于保存触发陷阱（trap）时的程序计数器（program counter）的值。
  
"sepc" 是 Supervisor Exception Program Counter 的缩写，它是一个 32 位的寄存器。当发生陷阱事件（如系统调用、异常或中断）时，RISC-V CPU 会将当前指令的地址保存到 sepc 寄存器中。这样做是为了在陷阱处理完成后，能够恢复到原始的执行点继续执行


# stvec
在 RISC-V 中，当发生陷阱或中断时，处理器会根据当前权限级别（如用户态或内核态）和陷阱类型，跳转到陷阱向量表中相应位置的处理程序来处理该事件。
在 RISC-V 中，stvec CSR 用于指定用于处理超级权限级别陷阱的陷阱向量表的基地址。陷阱向量表包含各种异常和中断的处理程序的地址。当发生陷阱时，处理器会跳转到陷阱向量表中指定的相应处理程序的地址。

# SEPC（Supervisor Exception Program Counter）
SEPC（Supervisor Exception Program Counter）是RISC-V体系结构中的一个特殊寄存器，用于保存异常处理程序的返回地址。

在RISC-V处理器中，当发生异常或中断时，处理器会将当前指令的地址保存在SEPC寄存器中，并跳转到异常处理程序或中断处理程序。当异常处理程序执行完成后，通过将SEPC寄存器中的值恢复到PC（Program Counter）寄存器，可以使处理器从中断或异常的上下文中返回到原始的执行状态。

SEPC寄存器在处理器的特权级别之间切换时也起到重要作用。当从用户态切换到内核态时，处理器会将用户态的PC值保存到SEPC寄存器中，以便在处理完内核态任务后，能够从SEPC寄存器中恢复用户态的执行。


# SSATUS
SSTATUS（Supervisor Status Register）是RISC-V体系结构中的一个特殊寄存器，用于保存处理器的状态和控制特权级别。

SSTATUS寄存器包含了一系列的位字段，用于控制和表示处理器的状态。其中一些重要的位字段如下：

1. SIE（Supervisor Interrupt Enable）：该位用于控制是否允许中断（包括外部中断和软件中断）发生。当SIE位为1时，中断被允许；当SIE位为0时，中断被禁止。

2. SPIE（Supervisor Previous Interrupt Enable）：该位用于保存上一个特权级别（通常是用户态）的中断使能状态。在发生特权级别切换时，当前特权级别的中断使能状态会保存到SPIE位，以便在返回时能够恢复到正确的中断使能状态。

3. SPP（Supervisor Previous Privilege）：该位用于保存上一个特权级别的特权级别。在发生特权级别切换时，当前特权级别会保存到SPP位，以便在返回时能够恢复到正确的特权级别。

4. SIE（Supervisor Exception）：该位表示是否存在未处理的异常。当SIE位为1时，表示有未处理的异常需要被处理；当SIE位为0时，表示没有未处理的异常。

5. SUM（User Memory Access）：该位用于控制用户态对内存的访问权限。当SUM位为1时，表示允许用户态对内存进行读取和写入操作；当SUM位为0时，表示禁止用户态对内存的写入操作。