#include "kernel.h"

#include <avr/interrupt.h>
#include <stdlib.h>

volatile TaskControlBlock tasks[MAX_TASKS];
volatile uint8_t totalTask = 0;
volatile uint8_t curTaskId = -1;

Partition partitions[MAX_PARTITIONS] = {
	{.stackSize = STACK_SIZE, .used = 0},
	{.stackSize = STACK_SIZE, .used = 0},
	{.stackSize = STACK_SIZE, .used = 0},
};

int8_t getAvailPartition() {
	for(int8_t i = 0; i < MAX_PARTITIONS; ++i) {
		if (partitions[i].used == 0) {
			partitions[i].used = 1;
			return i;
		}
	}
	return -1;
}

void releasePartition(uint8_t pId) {
	partitions[pId].used = 0;
}

int8_t getAvailTaskHolder() {
	for (int8_t i = 0; i < MAX_TASKS; ++i) {
		if (tasks[i].used == 0) {
			tasks[i].used = 1;
			return i;
		}
	}
	return -1;
}

void releaseTaskHolder(uint8_t tId) {
	tasks[tId].taskFunctionPtr = NULL;
	tasks[tId].stackPtr = NULL;
	tasks[tId].used = 0;
}

uint8_t addTask(void (*taskFunctionPtr)(void *arg), void *arg) {
	int8_t tId = getAvailTaskHolder();
	if (tId == -1)
		return 1;
	int8_t pId = getAvailPartition();
	if (pId == -1) {
		releaseTaskHolder(tId);
		return 1;
	}

	tasks[tId].taskFunctionPtr = taskFunctionPtr;
	tasks[tId].stackPtr = partitions[pId].stack + partitions[pId].stackSize;
	++totalTask;
	uint16_t stackPtr = SP;
	SP = (uint16_t)tasks[tId].stackPtr;
	asm volatile(	
		"push %0\n\t"	// PC - low byte (start function)
		"push %1\n\t"	// PC - high byte
		"push __tmp_reg__\n\t"	// r0
		"in __tmp_reg__,__SREG__\n\t"
		"push __tmp_reg__\n\t"	// SREG
		"in __tmp_reg__,0x3b\n\t"	
		"push __tmp_reg__\n\t"	// RAMPZ

		"push __zero_reg__\n\t"	//r1
		"push r2\n\t"	
		"push r3\n\t"	
		"push r4\n\t"	
		"push r5\n\t"	
		"push r6\n\t"	
		"push r7\n\t"	
		"push r8\n\t"	
		"push r9\n\t"	
		"push r10\n\t"	
		"push r11\n\t"	
		"push r12\n\t"	
		"push r13\n\t"	
		"push r14\n\t"	
		"push r15\n\t"	
		"push r16\n\t"	
		"push r17\n\t"	
		"push r18\n\t"	
		"push r19\n\t"	
		"push r20\n\t"	
		"push r21\n\t"	
		"push r22\n\t"	
		"push r23\n\t"	
		"push %2\n\t"	// r24 start function arg - low byte (avr-gcc call convention)
		"push %3\n\t"	// r25 start function arg - high byte
		"push r26\n\t"	
		"push r27\n\t"	
		"push r28\n\t"	
		"push r29\n\t"	
		"push r30\n\t"	
		"push r31\n\t"	
		:
		: 
		"r" ((uint16_t)taskFunctionPtr),		// PC - low byte (start function)
		"r" (((uint16_t)taskFunctionPtr) >> 8),// PC - high byte
		"r" ((uint16_t)arg),			// start function arg - low byte
		"r" (((uint16_t)arg) >> 8)		// start function arg - high byte
	);
	tasks[tId].stackPtr  = (void*)SP;
	SP = stackPtr;

	return 0;
}


ISR(TIMER1_COMPA_vect, ISR_NAKED) {
    asm volatile(
        "push __tmp_reg__\n\t"   // r0
        "in __tmp_reg__,__SREG__\n\t"
        "push __tmp_reg__\n\t"
        "in __tmp_reg__,0x3b\n\t" // RAMPZ
        "push __tmp_reg__\n\t"

        // Save the CPU registers
        "push __zero_reg__\n\t"  // r1
        "clr __zero_reg__\n\t"
        "push r2\n\t"
        "push r3\n\t"
        "push r4\n\t"
        "push r5\n\t"
        "push r6\n\t"
        "push r7\n\t"
        "push r8\n\t"
        "push r9\n\t"
        "push r10\n\t"
        "push r11\n\t"
        "push r12\n\t"
        "push r13\n\t"
        "push r14\n\t"
        "push r15\n\t"
        "push r16\n\t"
        "push r17\n\t"
        "push r18\n\t"
        "push r19\n\t"
        "push r20\n\t"
        "push r21\n\t"
        "push r22\n\t"
        "push r23\n\t"
        "push r24\n\t"
        "push r25\n\t"
        "push r26\n\t"
        "push r27\n\t"
        "push r28\n\t"
        "push r29\n\t"
        "push r30\n\t"
        "push r31\n\t"
    );
    if (curTaskId != -1)
        tasks[curTaskId].stackPtr = (void*)SP;

    curTaskId = (curTaskId + 1) % totalTask;

    SP = (uint16_t)tasks[curTaskId].stackPtr;
    asm volatile(
        "pop r31\n\t"
        "pop r30\n\t"
        "pop r29\n\t"
        "pop r28\n\t"
        "pop r27\n\t"
        "pop r26\n\t"
        "pop r25\n\t"
        "pop r24\n\t"
        "pop r23\n\t"
        "pop r22\n\t"
        "pop r21\n\t"
        "pop r20\n\t"
        "pop r19\n\t"
        "pop r18\n\t"
        "pop r17\n\t"
        "pop r16\n\t"
        "pop r15\n\t"
        "pop r14\n\t"
        "pop r13\n\t"
        "pop r12\n\t"
        "pop r11\n\t"
        "pop r10\n\t"
        "pop r9\n\t"
        "pop r8\n\t"
        "pop r7\n\t"
        "pop r6\n\t"
        "pop r5\n\t"
        "pop r4\n\t"
        "pop r3\n\t"
        "pop r2\n\t"
        "pop __zero_reg__\n\t"             // r1

        "pop __tmp_reg__\n\t"              // RAMPZ
        "out 0x3b,__tmp_reg__\n\t"
        "pop __tmp_reg__\n\t"              // SREG
        "out __SREG__, __tmp_reg__\n\t"
        "pop __tmp_reg__\n\t"              // r0
    );

    asm volatile ("reti\n\t");
}

// Initialize Timer1 for time-sharing, 64ms for 1 slice
void init_sharing_time1() {
    // Set Timer1 Mode to CTC (Clear Timer on Compare Match)
    TCCR1A = 0;

    // Set Timer1 Mode to CTC; Set the Timer1 prescaler to 1024    
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);

    // Set the compare value for a 1 ms time slice
    OCR1A = F_CPU / 1000 - 1;

    // Clear Timer1 Compare Match A interrupt flag
    TIFR |= (1 << OCF1A);

    // Enable the Timer1 Compare Match A interrupt
    TIMSK |= (1 << OCIE1A);
    
}
