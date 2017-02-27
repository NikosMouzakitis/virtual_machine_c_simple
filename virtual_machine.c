#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

#define GENERAL_REGS 12 
#define STACK_SIZE 128

//	A virtual machine,
//	implementing an instruction set defined below.
//	______________________________
//	Mouzakitis Nikolaos,Crete 2017.

bool run_state = true;
int stack[STACK_SIZE];

struct reg_set
{
	int Register[GENERAL_REGS] ;
	int pc;		// program counter.
	int sp;		// stack pointer.
} vm_reg ;	// virtual machine registers.

typedef enum {

	SPSH,		// pushing integer on stack.
	SADD,		// adding the last two newly entered values on stack.
	SPOP,		// popping a value from stack.
	HLT,		// halting machine,execution stops.
	SPR,		// prints all elements in stack.
	j,		// jumps to following instruction index.
	add,		// add 1,2,3 	r2+r3-->r1
	sub,		// sub 1,2,3    r2-r3-->r1
	prR,		// prints on console the values stored in every register on system.
	li,		// li 1,4	4-->r1.
	mul,		// mul 1,2,3	r2*r3-->r1.
	igt,		// igt 1	get integer from user -->r1.
	ipr,		// ipr 3	prints on console the integer stored in r3.
	beq,		// beq 1,2,3	if r1==r2 it jumps to program index #3.
	bne		// bne 1,2,3	if r1!=r2 it jumps to program index #3.	

} InstructionSet;	// Machines instruction set.

	
		// Instructions to run on the virtual machine.
const int program[] = {	

	// Program gets from user 2 integers and then prints
	// all multiples of a to a*b,on console.
	
	// 	register usage:
	// $1=a,$2=b,$3=counter(init to 1)till a*b,$4=a*b,$5=1,$6=stores result to be printed.
	
	igt,1,		// 	getting a on $1.
	igt,2,	       	// 	getting b on $2.
	li,3,1,		// 	loading 1 on $3.
	li,5,1,		// 	loading 1 on $5.
	mul,4,1,2,	//	a*b--->	$4.
//	prR,		
	beq,3,2,30,	// 	if $4 != $2 jumps.
	mul,6,1,3,	// 	$6 <--- a * $3.
	ipr,6,		// 	printing $6.
	add,3,3,5,	// 	$3++.
	j,14,		// 	jumps at 14(literally means 14th element of program[].
	mul,6,1,3,	//      $6<--- a * $3
	ipr,6,		//	printing the last multiple.
	HLT		//	Program exits.

};

int fetch()
{
	return	program[vm_reg.pc];
}

void eval(int instr)
{
	int a,b,result,val_popped,i;

	switch(instr)
	{
		case beq:
			if(vm_reg.Register[program[++vm_reg.pc]] == vm_reg.Register[program[++vm_reg.pc]])
				vm_reg.pc = program[++vm_reg.pc] - 1;
			else
				vm_reg.pc++;	
			break;
		case bne:
			if(vm_reg.Register[program[++vm_reg.pc]] != vm_reg.Register[program[++vm_reg.pc]])
				vm_reg.pc = program[++vm_reg.pc] - 1;
			else
				vm_reg.pc++;	
			break;
		case ipr:
			printf("%d\n",vm_reg.Register[program[++vm_reg.pc]]);
			break;
		case igt:
			printf("enter integer: ");
			scanf("%d",&vm_reg.Register[program[++vm_reg.pc]]);
			break;
		case prR:
			printf("------------Registers--------------\n");	
			for( i = 0; i < GENERAL_REGS; i++)
				printf("r[%d] = %d\n",i,vm_reg.Register[i]);
			
			printf("program_cnt = %d\n",vm_reg.pc);
			printf("stack_pntr = %d\n",vm_reg.sp);
			printf("------------------------------------\n");
			break;
		case add:
			vm_reg.pc++ ;
			vm_reg.Register[ program[vm_reg.pc] ] = vm_reg.Register[ program[ ++vm_reg.pc] ] + vm_reg.Register[ program[ ++vm_reg.pc ] ];			
			break;
		case sub:
			vm_reg.pc++ ;
			vm_reg.Register[ program[vm_reg.pc] ] = vm_reg.Register[ program[ ++vm_reg.pc] ] - vm_reg.Register[ program[ ++vm_reg.pc ] ];			
			break;
		case mul:	
			vm_reg.pc++;
			vm_reg.Register[program[vm_reg.pc]] = vm_reg.Register[program[++vm_reg.pc]]*vm_reg.Register[program[++vm_reg.pc]];
			break;
		case li:		
			vm_reg.pc++;
			vm_reg.Register[ program[vm_reg.pc] ] = program[ ++vm_reg.pc ] ;			
			break;
		case j:	
			vm_reg.pc = program[++vm_reg.pc]-1;
			break;
		case SPR:
			printf("[op]:Stack Display:\n");
			for(i = 0; i <= vm_reg.sp; i++)
				printf("----------------stack[%d]: %d\n",i,stack[i]);
			break;
		case HLT:
			run_state = false;
			printf("System Halted\n");
			break;
		case SPSH:
			vm_reg.sp++;
			stack[vm_reg.sp] = program[++ vm_reg.pc];
		//	printf("[op]:pushed to stack: %d\n",stack[vm_reg.sp]);
			break;
		case SPOP:
			val_popped = stack[vm_reg.sp--];
			printf("[op]:popped: %d\n",val_popped);
			break;
		case SADD:
			a = stack[vm_reg.sp--];
			b = stack[vm_reg.sp--];
			result = a+b;
			vm_reg.sp++;
		//	printf("[op]:added.\n");
			stack[vm_reg.sp] = result;
			break;
	}
}

int main(int argc,char *argv[])
{
	int fetch_code;
	
	vm_reg.sp = -1;			// initializing register stack pointer.
	vm_reg.pc = 0;			// initializing program counter.
	
	while(run_state)
	{
		fetch_code = fetch();
		eval(fetch_code);
		vm_reg.pc++;
	}	

	return 0;
}