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


