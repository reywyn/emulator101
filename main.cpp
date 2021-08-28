#include <cstdio>
#include <cstdlib>
#include "disassembler.h"

#define WORD(x, y) ((x<<8) | y)

// condition codes
uint8_t z = 1;       //zero
uint8_t s = 1;       //sign
uint8_t p = 1;       //parity
uint8_t cy = 1;      //carry
uint8_t ac = 1;      //aux carry
uint8_t pad = 3;

// registers
uint8_t a;
uint8_t b;
uint8_t c;
uint8_t d;
uint8_t e;
uint8_t h;
uint8_t l;
uint16_t sp;
uint16_t pc;         //program counter
uint8_t *memory;
uint8_t int_enable;


uint8_t parity(uint8_t x) {
    int i;
    int parity = 0;
    x = (x & ((1 << 8) - 1));
    for (i = 0; i < 8; i++) {
        if (x & 0x1) parity++;
        x = x >> 1;
    }
    return (0 == (parity & 0x1));
}

void condition_codes(uint16_t x) {
    z = ((x & 0xff) == 0);
    s = ((x & 0x80) != 0);
    cy = (x > 0xff);
    p = parity(x & 0xff);
}

void add(const uint8_t *x) {
    uint16_t answer = (uint16_t) a + (uint16_t) *x;
    condition_codes(answer);
    a = answer & 0xff;
}

void adc(const uint8_t *x) {
    uint16_t answer = (uint16_t) a + (uint16_t) *x + (uint16_t) cy;
    condition_codes(answer);
    a = answer & 0xff;
}

void sub(const uint8_t *x) {
    uint16_t answer = (uint16_t) a - (uint16_t) *x;
    condition_codes(answer);
    a = answer & 0xff;
}

void sbb(const uint8_t *x) {
    uint16_t answer = (uint16_t) a - (uint16_t) *x - (uint16_t) cy ;
    condition_codes(answer);
    a = answer & 0xff;
}

void ana(const uint8_t *x) {
    uint16_t answer = (uint16_t) a & (uint16_t) *x;
    condition_codes(answer);
    a = answer & 0xff;
}

void xra(const uint8_t *x) {
    uint16_t answer = (uint16_t) a ^ (uint16_t) *x;
    condition_codes(answer);
    a = answer & 0xff;
}

void ora(const uint8_t *x) {
    uint16_t answer = (uint16_t) a | (uint16_t) *x;
    condition_codes(answer);
    a = answer & 0xff;
}

void cmp(const uint8_t *x) {
    uint16_t answer = (uint16_t) a - (uint16_t) *x;
    condition_codes(answer);
}

int emulate_step() {
    unsigned char *opcode = &memory[pc];
    switch (*opcode) {
        case 0x00: // NOP
        case 0x08:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x28:
        case 0x30:
        case 0x38:
        case 0xcb:
        case 0xd9:
        case 0xdd:
        case 0xed:
        case 0xfd:
            break;
        case 0x01:
            b = opcode[2];
            c = opcode[1];
            pc += 2;
        case 0x40: //MOV B,B
            b = b;
            break;
        case 0x41: //MOV B,C
            b = c;
            break;
        case 0x42: //MOV B,D
            b = d;
            break;
        case 0x43: //MOV B,E
            b = e;
            break;
        case 0x44: //MOV B,H
            b = h;
            break;
        case 0x45: //MOV B,L
            b = l;
            break;
        case 0x46: //MOV B,M
            b = memory[WORD(h, l)];
            break;
        case 0x47: //MOV B,A
            b = a;
            break;
        case 0x48: //MOV C,B
            c = b;
            break;
        case 0x49: //MOV C,C
            c = c;
            break;
        case 0x4a: //MOV C,D
            c = d;
            break;
        case 0x4b: //MOV C,E
            c = e;
            break;
        case 0x4c: //MOV C,H
            c = h;
            break;
        case 0x4d: //MOV C,L
            c = l;
            break;
        case 0x4e: //MOV C,M
            c = memory[WORD(h, l)];
            break;
        case 0x4f: //MOV C,A
            c = a;
            break;
        case 0x50: //MOV D,B
            d = b;
            break;
        case 0x51: //MOV D,C
            d = c;
            break;
        case 0x52: //MOV D,D
            d = d;
            break;
        case 0x53: //MOV D,E
            d = e;
            break;
        case 0x54: //MOV D,H
            d = h;
            break;
        case 0x55: //MOV D,L
            d = l;
            break;
        case 0x56: //MOV D,M
            d = memory[WORD(h, l)];
            break;
        case 0x57: //MOV D,A
            d = a;
            break;
        case 0x58: //MOV E,B
            e = b;
            break;
        case 0x59: //MOV E,C
            e = c;
            break;
        case 0x5a: //MOV E,D
            e = d;
            break;
        case 0x5b: //MOV E,E
            e = e;
            break;
        case 0x5c: //MOV E,H
            e = h;
            break;
        case 0x5d: //MOV E,L
            e = l;
            break;
        case 0x5e: //MOV E,M
            e = memory[WORD(h, l)];
            break;
        case 0x5f: //MOV E,A
            e = a;
            break;
        case 0x60: //MOV H,B
            h = b;
            break;
        case 0x61: //MOV H,C
            h = c;
            break;
        case 0x62: //MOV H,D
            h = d;
            break;
        case 0x63: //MOV H,E
            h = e;
            break;
        case 0x64: //MOV H,H
            h = h;
            break;
        case 0x65: //MOV H,L
            h = l;
            break;
        case 0x66: //MOV H,M
            h = memory[WORD(h, l)];
            break;
        case 0x67: //MOV H,A
            h = a;
            break;
        case 0x68: //MOV L,B
            l = b;
            break;
        case 0x69: //MOV L,C
            l = c;
            break;
        case 0x6a: //MOV L,D
            l = d;
            break;
        case 0x6b: //MOV L,E
            l = e;
            break;
        case 0x6c: //MOV L,H
            l = h;
            break;
        case 0x6d: //MOV L,L
            l = l;
            break;
        case 0x6e: //MOV L,M
            l = memory[WORD(h, l)];
            break;
        case 0x6f: //MOV L,A
            l = a;
            break;
        case 0x70: //MOV M,B
            memory[WORD(h, l)] = b;
            break;
        case 0x71: //MOV M,C
            memory[WORD(h, l)] = c;
            break;
        case 0x72: //MOV M,D
            memory[WORD(h, l)] = d;
            break;
        case 0x73: //MOV M,E
            memory[WORD(h, l)] = e;
            break;
        case 0x74: //MOV M,H
            memory[WORD(h, l)] = h;
            break;
        case 0x75: //MOV M,L
            memory[WORD(h, l)] = l;
            break;
        case 0x77: //MOV M,A
            memory[WORD(h, l)] = a;
            break;
        case 0x78: //MOV M,B
            a = b;
            break;
        case 0x79: //MOV A,C
            a = c;
            break;
        case 0x7a: //MOV A,D
            a = d;
            break;
        case 0x7b: //MOV A,E
            a = e;
            break;
        case 0x7c: //MOV A,H
            a = h;
            break;
        case 0x7d: //MOV A,L
            a = l;
            break;
        case 0x7e: //MOV A,M
            a = memory[WORD(h, l)];
            break;
        case 0x7f: //MOV A,A
            a = a;
            break;
        case 0x76: // HLT
            exit(1);
            break;


        case 0x80:// ADD B
            add(&b);
            break;
        case 0x81:// ADD C
            add(&c);
            break;
        case 0x82:// ADD D
            add(&d);
            break;
        case 0x83:// ADD E
            add(&e);
            break;
        case 0x84:// ADD H
            add(&h);
            break;
        case 0x85:// ADD L
            add(&l);
            break;
        case 0x86:// ADD M
            add(&memory[WORD(h, l)]);
            break;
        case 0x87:// ADD A
            add(&a);
            break;
        case 0x88:// ADC B
            adc(&b);
            break;
        case 0x89:// ADC C
            adc(&c);
            break;
        case 0x8a:// ADC D
            adc(&d);
            break;
        case 0x8b:// ADC E
            adc(&e);
            break;
        case 0x8c:// ADC H
            adc(&h);
            break;
        case 0x8d:// ADC L
            adc(&l);
            break;
        case 0x8e:// ADC M
            adc(&memory[WORD(h, l)]);
            break;
        case 0x8f:// ADC A
            adc(&a);
            break;
        case 0x90:// SUB B
            sub(&b);
            break;
        case 0x91:// SUB C
            sub(&c);
            break;
        case 0x92:// SUB D
            sub(&d);
            break;
        case 0x93:// SUB E
            sub(&e);
            break;
        case 0x94:// SUB H
            sub(&h);
            break;
        case 0x95:// SUB L
            sub(&l);
            break;
        case 0x96:// SUB M
            sub(&memory[WORD(h,l)]);
            break;
        case 0x97:// SUB A
            sub(&a);
            break;
        case 0x98:// SBB B
            sbb(&b);
            break;
        case 0x99:// SBB C
            sbb(&c);
            break;
        case 0x9a:// SBB D
            sbb(&d);
            break;
        case 0x9b:// SBB E
            sbb(&e);
            break;
        case 0x9c:// SBB H
            sbb(&h);
            break;
        case 0x9d:// SBB L
            sbb(&l);
            break;
        case 0x9e:// SBB M
            sbb(&memory[WORD(h,l)]);
            break;
        case 0x9f:// SBB A
            sbb(&a);
            break;
        case 0xa0:// ANA B
            ana(&b);
            break;
        case 0xa1:// ANA C
            ana(&c);
            break;
        case 0xa2:// ANA D
            ana(&d);
            break;
        case 0xa3:// ANA E
            ana(&e);
            break;
        case 0xa4:// ANA H
            ana(&h);
            break;
        case 0xa5:// ANA L
            ana(&l);
            break;
        case 0xa6:// ANA M
            ana(&memory[WORD(h,l)]);
            break;
        case 0xa7:// ANA A
            ana(&a);
            break;
        case 0xa8:// XRA B
            xra(&b);
            break;
        case 0xa9:// XRA C
            xra(&c);
            break;
        case 0xaa:// XRA D
            xra(&d);
            break;
        case 0xab:// XRA E
            xra(&e);
            break;
        case 0xac:// XRA H
            xra(&h);
            break;
        case 0xad:// XRA L
            xra(&l);
            break;
        case 0xae:// XRA M
            xra(&memory[WORD(h,l)]);
            break;
        case 0xaf:// XRA A
            xra(&a);
            break;
        case 0xb0:// ORA B
            ora(&b);
            break;
        case 0xb1:// ORA C
            ora(&c);
            break;
        case 0xb2:// ORA D
            ora(&d);
            break;
        case 0xb3:// ORA E
            ora(&e);
            break;
        case 0xb4:// ORA H
            ora(&h);
            break;
        case 0xb5:// ORA L
            ora(&l);
            break;
        case 0xb6:// ORA M
            ora(&memory[WORD(h,l)]);
            break;
        case 0xb7:// ORA A
            ora(&a);
            break;
        case 0xb8:// CMP B
            cmp(&b);
            break;
        case 0xb9:// CMP C
            cmp(&c);
            break;
        case 0xba:// CMP D
            cmp(&d);
            break;
        case 0xbb:// CMP E
            cmp(&e);
            break;
        case 0xbc:// CMP H
            cmp(&h);
            break;
        case 0xbd:// CMP L
            cmp(&l);
            break;
        case 0xbe:// CMP M
            cmp(&memory[WORD(h,l)]);
            break;
        case 0xbf:// CMP A
            cmp(&a);
            break;


        case 0x02:
            printf("STAX B");
            break;
        case 0x03:
            printf("INX B");
            break;
        case 0x04:
            printf("INR B");
            break;
        case 0x05:
            printf("DCR B");
            break;
        case 0x06:
            printf("MVI B,#$");
            READ_1_BYTE;
            break;
        case 0x07:
            printf("RLC");
            break;
        case 0x09:
            printf("DAD B");
            break;
        case 0x0a:
            printf("LDAX B");
            break;
        case 0x0b:
            printf("DCX B");
            break;
        case 0x0c:
            printf("INR C");
            break;
        case 0x0d:
            printf("DCR C");
            break;
        case 0x0e:
            printf("MVI C,#$");
            READ_1_BYTE;
            break;
        case 0x0f:
            printf("RRC");
            break;

        case 0x11:
            printf("LXI D,#$");
            READ_2_BYTE;
            break;
        case 0x12:
            printf("STAX D");
            break;
        case 0x13:
            printf("INX D");
            break;
        case 0x14:
            printf("INR D");
            break;
        case 0x15:
            printf("DCR D");
            break;
        case 0x16:
            printf("MVI D,#$");
            READ_1_BYTE;
            break;
        case 0x17:
            printf("RAL");
            break;

        case 0x19:
            printf("DAD D");
            break;
        case 0x1a:
            printf("LDAX D");
            break;
        case 0x1b:
            printf("DCX D");
            break;
        case 0x1c:
            printf("INR E");
            break;
        case 0x1d:
            printf("DCR E");
            break;
        case 0x1e:
            printf("MVI E,#$");
            READ_1_BYTE;
            break;
        case 0x1f:
            printf("RAR");
            break;

        case 0x21:
            printf("LXI H,#$");
            READ_2_BYTE;
            break;
        case 0x22:
            printf("SHLD $");
            READ_2_BYTE;
            break;
        case 0x23:
            printf("INX H");
            break;
        case 0x24:
            printf("INR H");
            break;
        case 0x25:
            printf("DCR H");
            break;
        case 0x26:
            printf("MVI H,#$");
            READ_1_BYTE;
            break;
        case 0x27:
            printf("DAA");
            break;

        case 0x29:
            printf("DAD H");
            break;
        case 0x2a:
            printf("LHLD $");
            READ_2_BYTE;
            break;
        case 0x2b:
            printf("DCX H");
            break;
        case 0x2c:
            printf("INR L");
            break;
        case 0x2d:
            printf("DCR L");
            break;
        case 0x2e:
            printf("MVI L,#$");
            READ_1_BYTE;
            break;
        case 0x2f:
            printf("CMA");
            break;

        case 0x31:
            printf("LXI SP,#$");
            READ_2_BYTE;
            break;
        case 0x32:
            printf("STA $");
            READ_2_BYTE;
            break;
        case 0x33:
            printf("INX SP");
            break;
        case 0x34:
            printf("INR M");
            break;
        case 0x35:
            printf("DCR M");
            break;
        case 0x36:
            printf("MVI M,#$");
            READ_1_BYTE;
            break;
        case 0x37:
            printf("STC");
            break;

        case 0x39:
            printf("DAD SP");
            break;
        case 0x3a:
            printf("LDA $");
            READ_2_BYTE;
            break;
        case 0x3b:
            printf("DCX SP");
            break;
        case 0x3c:
            printf("INR A");
            break;
        case 0x3d:
            printf("DCR A");
            break;
        case 0x3e:
            printf("MVI A,#$");
            READ_1_BYTE;
            break;
        case 0x3f:
            printf("CMC");
            break;


        case 0xc0:
            printf("RNZ");
            break;
        case 0xc1:
            printf("POP B");
            break;
        case 0xc2:
            printf("JNZ $");
            READ_2_BYTE;
            break;
        case 0xc3:
            printf("JMP $");
            READ_2_BYTE;
            break;
        case 0xc4:
            printf("CNZ $");
            READ_2_BYTE;
            break;
        case 0xc5:
            printf("PUSH B");
            break;
        case 0xc6:
            printf("ADI #$");
            READ_1_BYTE;
            break;
        case 0xc7:
            printf("RST 0");
            break;
        case 0xc8:
            printf("RZ");
            break;
        case 0xc9:
            printf("RET");
            break;
        case 0xca:
            printf("JZ $");
            READ_2_BYTE;
            break;

        case 0xcc:
            printf("CZ $");
            READ_2_BYTE;
            break;
        case 0xcd:
            printf("CALL $");
            READ_2_BYTE;
            break;
        case 0xce:
            printf("ACI #$");
            READ_1_BYTE;
            break;
        case 0xcf:
            printf("RST 1");
            break;
        case 0xd0:
            printf("RNC");
            break;
        case 0xd1:
            printf("POP D");
            break;
        case 0xd2:
            printf("JNC $");
            READ_2_BYTE;
            break;
        case 0xd3:
            printf("OUT #$");
            READ_1_BYTE;
            break;
        case 0xd4:
            printf("CNC $");
            READ_2_BYTE;
            break;
        case 0xd5:
            printf("PUSH D");
            break;
        case 0xd6:
            printf("SUI $");
            READ_1_BYTE;
            break;
        case 0xd7:
            printf("RST 2");
            break;
        case 0xd8:
            printf("RC");
            break;

        case 0xda:
            printf("JC $");
            READ_2_BYTE;
            break;
        case 0xdb:
            printf("IN $");
            READ_1_BYTE;
            break;
        case 0xdc:
            printf("CC $");
            READ_2_BYTE;
            break;

        case 0xde:
            printf("SBI $");
            READ_1_BYTE;
            break;
        case 0xdf:
            printf("RST 3");
            break;
        case 0xe0:
            printf("RPO");
            break;
        case 0xe1:
            printf("POP H");
            break;
        case 0xe2:
            printf("JPO $");
            READ_2_BYTE;
            break;
        case 0xe3:
            printf("XTHL");
            break;
        case 0xe4:
            printf("CPO $");
            READ_2_BYTE;
            break;
        case 0xe5:
            printf("PUSH H");
            break;
        case 0xe6:
            printf("ANI #$");
            READ_1_BYTE;
            break;
        case 0xe7:
            printf("RST 4");
            break;
        case 0xe8:
            printf("RPE");
            break;
        case 0xe9:
            printf("PCHL");
            break;
        case 0xea:
            printf("JPE $");
            READ_2_BYTE;
            break;
        case 0xeb:
            printf("XCHG");
            break;
        case 0xec:
            printf("CPE $");
            READ_2_BYTE;
            break;

        case 0xee:
            printf("XRI #$");
            READ_1_BYTE;
            break;
        case 0xef:
            printf("RST 5");
            break;
        case 0xf0:
            printf("RP");
            break;
        case 0xf1:
            printf("POP PSW");
            break;
        case 0xf2:
            printf("JP $");
            READ_2_BYTE;
            break;
        case 0xf3:
            printf("DI");
            break;
        case 0xf4:
            printf("CP $");
            READ_2_BYTE;
            break;
        case 0xf5:
            printf("PUSH PSW");
            break;
        case 0xf6:
            printf("ORI #$");
            READ_1_BYTE;
            break;
        case 0xf7:
            printf("RST 6");
            break;
        case 0xf8:
            printf("RM");
            break;
        case 0xf9:
            printf("SPHL");
            break;
        case 0xfa:
            printf("JM $");
            READ_2_BYTE;
            break;
        case 0xfb:
            printf("EI");
            break;
        case 0xfc:
            printf("CM $");
            READ_2_BYTE;
            break;

        case 0xfe:
            printf("CPI #$");
            READ_1_BYTE;
            break;
        case 0xff:
            printf("RST 7");
            break;
        default:
            printf("Unimplemented instruction\n");
            exit(1);
    }
    _pc++;  //increment pc
}

int main(int argc, char **argv) {

    FILE *f = fopen(argv[1], "rb");
    if (f == nullptr) {
        printf("error: Couldn't open %s\n", argv[1]);
        exit(1);
    }

    //Get the file size and read it into a memory buffer
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    auto *buffer = static_cast<unsigned char *>(malloc(fsize));

    fread(buffer, fsize, 1, f);
    fclose(f);

    int pc = 0;

    while (pc < fsize) {
        pc += disassemble(buffer, pc);
    }
    return 0;
}
