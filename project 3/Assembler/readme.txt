
    Mustafa Eren SAĞLAM - 210201056
    Antalya Bilim Üniversitesi - Computer Science


Program reads MIPS assembly instructions from a file named Code.asm, 
decodes them into binary format, and writes the binary instructions to a file named Code.obj

Opcodes: Each instruction type has a unique opcode, which is a binary code that represents the operation to be performed.
R-Type: Each R-type instruction has a unique 6-bit function code that tells the processor which operation to perform.
I-Type: These instructions involve immediate values and often perform operations like 
addition with a constant, loading, and storing values.
J-Type: These are jump instructions that specify a target address for the program to continue execution.

R-type: opCode(6bit)    rs(5bit)    rt(5bit)    rd(5bit)    shamt(5bit)     funct(6bit)
I-type: opCode(6bit)    rs(5bit)    rt(5bit)    immediate(16bit)
J-type: opCode(6bit)    address(26bit)


