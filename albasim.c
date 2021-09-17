#include <stdlib.h>
#include <stdio.h>

#define si short int

si r0[] = {0};
si r1[] = {0};
si r2[] = {0};
si r3[] = {0};
si r4[] = {0};
si r5[] = {0};
si r6[] = {0};
si r7[] = {0};
si r8[] = {0};
si r9[] = {0};
si r10[] = {0};
si r11[] = {0};
si r12[] = {0};
si r13[] = {0};
si r14[] = {0};
si r15[] = {0};
si PC = 0x0000;

char memory[0xffff * 2];

int get_register_num(si *reg);

void add(si *rw, si *ra, si *rb)
{
	(*rw) = *ra + *rb;
	printf("add r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void sub(si *rw, si *ra, si *rb)
{
	(*rw) = *ra - *rb;
	printf("sub r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void and(si *rw, si *ra, si *rb)
{
	(*rw) = *ra & *rb;
	printf("and r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void or(si *rw, si *ra, si *rb)
{
	(*rw) = *ra | *rb;
	printf("or r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void not(si *rw, si *ra)
{
	(*rw) = ~*ra;
	printf("not r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rw), *rw);
}

void shl(si *rw, si *ra, si *rb)
{
	(*rw) = *ra << *rb;
	printf("shl r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void shr(si *rw, si *ra, si *rb)
{
	(*rw) = *ra >> *rb;
	printf("shr r%d, r%d, r%d\t\t| r%d = 0x%hi\n", get_register_num(rw), get_register_num(ra), get_register_num(rb), get_register_num(rw), *rw);
}

void ldi(si *rw, si imm8)
{
	unsigned int and_mask = 0xffff;
	imm8 = imm8 & and_mask;
	*rw = imm8;
	printf("ldi r%d, %hi\t\t| r%d = 0x%hi\n", get_register_num(rw), imm8, get_register_num(rw), imm8);
}

void ld(si *rw, si imm4, si *rb)
{
	unsigned int and_mask = 0xffff;
	imm4 = imm4 & and_mask;

	si *mempoint = (si *)memory;

	(*rw) = mempoint[(*rb) + imm4];
	printf("ld r%d, r%d, %d\t\t| r%d = M[%hx] = 0x%hx\n", get_register_num(rw), get_register_num(rb), imm4, get_register_num(rw), (unsigned short int)((*rb) + imm4), *rw);
}

void st(si imm4, si *ra, si *rb)
{
	unsigned int and_mask = 0xffff;
	imm4 = imm4 & and_mask;

	si *mempoint = (si *)memory;
	mempoint[ (unsigned short int)((*rb) + imm4)] = (*ra);
	printf("st r%d, r%d, %d\t\t| M[%hx] = 0x%hx\n", get_register_num(ra), get_register_num(rb), imm4, (unsigned short int)((*rb) + imm4), *ra);
}

void br(si imm8)
{
	printf("IMM8 %hi ", imm8 >> 7);
	if (imm8 >> 7 == 0x1) imm8 = imm8 | 0xff00;
	unsigned int and_mask = 0xffff;
	imm8 = imm8 & and_mask;
	PC += imm8 - 1;
	
	printf("br %hi\n", imm8);
}

void bz(si imm8, si *rb)
{
	if (*rb == 0)
	{
		unsigned int and_mask = 0xffff;
		imm8 = imm8 & and_mask;
		PC += imm8 - 1;
	}
	printf("IMM8 %hi", imm8);
	printf("bz r%d, %hi\n", get_register_num(rb), imm8);
}

void bn(si imm8, si *rb)
{
	if (*rb < 0)
	{
		unsigned int and_mask = 0xffff;
		imm8 = imm8 & and_mask;
		PC += imm8 - 1;
	}
	printf("bn r%d, %hi\n", get_register_num(rb), imm8);
}

void jal(si imm12)
{
	unsigned int and_mask = 0xffff;
	unsigned int PC_mask = 0xf000;
	imm12 = imm12 & and_mask;
	*r15 = PC;
	PC = (PC & PC_mask) | imm12 - 1;
	printf("jal %hi\t\t| r15 = %hx\n", imm12, *r15);
}

void jr(si *ra)
{
	PC = *ra;
	printf("jr r%d\n", get_register_num(ra));
}

void print_registers_dec()
{
	printf( "r0: %d r1: %d r2: %d r3: %d\n"
			"r4: %d r5: %d r6: %d r7: %d\n"
			"r8: %d r9: %d r10: %d r11: %d\n"
			"r12: %d r13: %d r14: %d r15: %d\n"
			"PC: %d\n",
			*r0, *r1, *r2, *r3, *r4, *r5, *r6, *r7, *r8,
			*r9, *r10, *r11, *r12, *r13, *r14, *r15, PC);
}

void print_registers_hex()
{
	printf( "r0: %hx r1: %hx r2: %hx r3: %hx\n"
			"r4: %hx r5: %hx r6: %hx r7: %hx\n"
			"r8: %hx r9: %hx r10: %hx r11: %hx\n"
			"r12: %hx r13: %hx r14: %hx r15: %hx\n"
			"PC: %d\n",
			*r0, *r1, *r2, *r3, *r4, *r5, *r6, *r7, *r8,
			*r9, *r10, *r11, *r12, *r13, *r14, *r15, PC);
}

si *get_register(si reg)
{
	switch (reg)
	{
		case 0: return r0; break;
		case 1: return r1; break;
		case 2: return r2; break;
		case 3: return r3; break;
		case 4: return r4; break;
		case 5: return r5; break;
		case 6: return r6; break;
		case 7: return r7; break;
		case 8: return r8; break;
		case 9: return r9; break;
		case 10: return r10; break;
		case 11: return r11; break;
		case 12: return r12; break;
		case 13: return r13; break;
		case 14: return r14; break;
		case 15: return r15; break;
	}
}

int get_register_num(si *reg)
{
	if (reg == r0) return 0;
	if (reg == r1) return 1;
	if (reg == r2) return 2;
	if (reg == r3) return 3;
	if (reg == r4) return 4;
	if (reg == r5) return 5;
	if (reg == r6) return 6;
	if (reg == r7) return 7;
	if (reg == r8) return 8;
	if (reg == r9) return 9;
	if (reg == r10) return 10;
	if (reg == r11) return 11;
	if (reg == r12) return 12;
	if (reg == r13) return 13;
	if (reg == r14) return 14;
	if (reg == r15) return 15;
}

int decode_opcode(si *mempoint)
{
	unsigned int instruction = 0xf000;
	unsigned int a = 0x0f00;
	unsigned int b = 0x00f0;
	unsigned int c = 0x000f;
	si a_code = ((*mempoint) & a) >> 8;
	si b_code = ((*mempoint) & b) >> 4;
	si c_code = ((*mempoint) & c);
	printf("%4hx: %hx  ", PC, mempoint[0]);
	//printf("%d %d %d %d\n", ((*mempoint) & instruction) >> 12, a_code, b_code, c_code);
	switch ( ((*mempoint) & instruction) >> 12 )
	{
		case 0x0: add(get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x1: sub(get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x2: and(get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x3: or (get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x4: not(get_register(a_code), get_register(b_code)); break;
		case 0x5: shl(get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x6: shr(get_register(a_code), get_register(b_code), get_register(c_code)); break;
		case 0x7: ldi(get_register(a_code), ((*mempoint) & b) | ((*mempoint) & c)); break;
		case 0x8: ld(get_register(a_code), b_code, get_register(c_code)); break;
		case 0x9: st(a_code, get_register(b_code), get_register(c_code)); break;
		case 0xa: br((((*mempoint) & a) | ((*mempoint) & b)) >> 4); break;
		case 0xb: bz((((*mempoint) & a) | ((*mempoint) & b)) >> 4, get_register(c_code)); break;
		case 0xc: bn((((*mempoint) & a) | ((*mempoint) & b)) >> 4, get_register(c_code)); break;
		case 0xd: jal(((*mempoint) & a) | ((*mempoint) & b) | ((*mempoint) & c)); break;
		case 0xe: jr(get_register(b_code)); break;
		case 0xf: return 1;
	}
	return 0;
}

int main()
{
	si *mempoint = (si*)memory;
	FILE *input_mem = fopen("jr.mem", "r");
	int memory_address = 0;
	char input[200];
	si temp;
	while (!feof(input_mem))
	{
		fgets(input, 200, input_mem);
		sscanf(input, "@%*s %x // %*s", &mempoint[memory_address]);

		memory_address++;
	}
	int code = 0;
	while (code == 0)
	{
		//print_registers_dec();
		code = decode_opcode(mempoint + PC);
		PC++;
	}
	

	print_registers_dec();
	
	return 0;
}
