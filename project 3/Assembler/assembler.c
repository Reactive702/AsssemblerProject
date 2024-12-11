/*



    CS 365 - System Programming
    Project 3 - MIPS Assembler
    
    Mustafa Eren SAĞLAM
    210201056
    Computer Science
    Antalya International Bilim University



*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// R-type instruction structure
typedef struct {
    char opcode[10];
    int rs;    // register source
    int rt;    // register target
    int rd;    // register destination
    int shamt; // shift amount
} RTypeInstruction;

// I-type instruction structure
typedef struct {
    char opcode[10];
    int rs;
    int rt;
    int immediate; // 16 bit
} ITypeInstruction;

// J-type instruction structure
typedef struct {
    char opcode[10];
    int address; // 26 bit
} JTypeInstruction;

// function to convert decimal to binary with fixed bits
void decToBinNBits(int n, int nBits, char *bin) {
    for (int i = 0; i < nBits; i++) bin[i] = '0';
    bin[nBits] = '\0';

    if (n < 0) {
        n = n + (1 << nBits); // Handle negative numbers
    }

    for (int i = nBits - 1; n > 0; i--) {
        bin[i] = '0' + (n % 2);
        n /= 2;
    }
}

// maps function names to their binary representations
const char *funcToBin(const char *func) {
    if (strcmp(func, "ADD") == 0) return "100000";
    if (strcmp(func, "SUB") == 0) return "100010";
    if (strcmp(func, "MUL") == 0) return "011000";
    if (strcmp(func, "AND") == 0) return "100100";
    if (strcmp(func, "OR") == 0) return "100101";
    if (strcmp(func, "XOR") == 0) return "100110";
    if (strcmp(func, "SLL") == 0) return "000000";
    if (strcmp(func, "SRL") == 0) return "000010";
    return NULL;
}

// maps opcodes to their binary representations
const char *opcodeToBin(const char *opcode) {
    if (strcmp(opcode, "ADDI") == 0) return "001000";
    if (strcmp(opcode, "ANDI") == 0) return "001100";
    if (strcmp(opcode, "LW") == 0) return "100011";
    if (strcmp(opcode, "SW") == 0) return "101011";
    if (strcmp(opcode, "BEQ") == 0) return "000100";
    if (strcmp(opcode, "BNE") == 0) return "000101";
    if (strcmp(opcode, "BLEZ") == 0) return "000110";
    if (strcmp(opcode, "BGTZ") == 0) return "000111";
    if (strcmp(opcode, "J") == 0) return "000010";
    if (strcmp(opcode, "JAL") == 0) return "000011";
    return NULL;
}

// convert R-Type instruction to binary
void rTypeToBin(RTypeInstruction *inst, char *result) {
    char rs[6], rt[6], rd[6], shamt[6];
    decToBinNBits(inst->rs, 5, rs);
    decToBinNBits(inst->rt, 5, rt);
    decToBinNBits(inst->rd, 5, rd);
    decToBinNBits(inst->shamt, 5, shamt);

    snprintf(result, 50, "000000 %s %s %s %s %s",
             rs, rt, rd, shamt, funcToBin(inst->opcode));
}

// convert I-Type instruction to binary
void iTypeToBin(ITypeInstruction *inst, char *result) {
    char rs[6], rt[6], imm[17];
    decToBinNBits(inst->rs, 5, rs);
    decToBinNBits(inst->rt, 5, rt);
    decToBinNBits(inst->immediate, 16, imm);

    snprintf(result, 50, "%s %s %s %s",
             opcodeToBin(inst->opcode), rs, rt, imm);
}

// convert J-Type instruction to binary
void jTypeToBin(JTypeInstruction *inst, char *result) {
    char address[27];
    decToBinNBits(inst->address, 26, address);

    snprintf(result, 50, "%s %s", opcodeToBin(inst->opcode), address);
}

// helper functions to identify instruction types
bool isRType(const char *opcode) {
    return strcmp(opcode, "ADD") == 0 || strcmp(opcode, "SUB") == 0 || strcmp(opcode, "MUL") == 0 ||
           strcmp(opcode, "AND") == 0 || strcmp(opcode, "OR") == 0 || strcmp(opcode, "XOR") == 0 ||
           strcmp(opcode, "SLL") == 0 || strcmp(opcode, "SRL") == 0;
}

bool isIType(const char *opcode) {
    return strcmp(opcode, "ADDI") == 0 || strcmp(opcode, "ANDI") == 0 || strcmp(opcode, "LW") == 0 ||
           strcmp(opcode, "SW") == 0 || strcmp(opcode, "BEQ") == 0 || strcmp(opcode, "BNE") == 0 ||
           strcmp(opcode, "BLEZ") == 0 || strcmp(opcode, "BGTZ") == 0;
}

bool isJType(const char *opcode) {
    return strcmp(opcode, "J") == 0 || strcmp(opcode, "JAL") == 0;
}

int main() {
    FILE *inputFile = fopen("Code.asm", "r");
    FILE *outputFile = fopen("Code.obj", "w");

    if (!inputFile || !outputFile) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    // arrays to store instructions
    RTypeInstruction rInsts[100];
    ITypeInstruction iInsts[100];
    JTypeInstruction jInsts[100];
    int rCnt = 0, iCnt = 0, jCnt = 0;   // counters for the number of insturction

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {  //reads each line of the Code.asm file into line
        char parts[10][20];
        int n = sscanf(line, "%s %s %s %s %s", parts[0], parts[1], parts[2], parts[3], parts[4]);

        if (isRType(parts[0])) {        //create an instance,parse,
            RTypeInstruction inst;
            strcpy(inst.opcode, parts[0]);
            inst.rd = atoi(parts[1] + 1);
            inst.rs = atoi(parts[2] + 1);
            inst.rt = atoi(parts[3] + 1);
            inst.shamt = 0;
            rInsts[rCnt++] = inst;
        } else if (isIType(parts[0])) {
            ITypeInstruction inst;
            strcpy(inst.opcode, parts[0]);
            inst.rs = atoi(parts[1] + 1);
            inst.rt = atoi(parts[2] + 1);
            inst.immediate = atoi(parts[3]);
            iInsts[iCnt++] = inst;
        } else if (isJType(parts[0])) {
            JTypeInstruction inst;
            strcpy(inst.opcode, parts[0]);
            inst.address = atoi(parts[1]);
            jInsts[jCnt++] = inst;
        }
    }
    
    //Loop through instances and use functions to convert each instruction to binary.
    //Write the binary result to the output file.
    char binary[50];
    for (int i = 0; i < rCnt; i++) {
        rTypeToBin(&rInsts[i], binary);
        fprintf(outputFile, "%s\n", binary);
    }
    for (int i = 0; i < iCnt; i++) {
        iTypeToBin(&iInsts[i], binary);
        fprintf(outputFile, "%s\n", binary);
    }
    for (int i = 0; i < jCnt; i++) {
        jTypeToBin(&jInsts[i], binary);
        fprintf(outputFile, "%s\n", binary);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("\nAssemble Completed. Check the files.\n");

    return 0;
}
