#ifndef _KERNEL_H
#define _KERNEL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define MAX_TASKS 3
#define MAX_PARTITIONS 3
#define STACK_SIZE 256


typedef struct Partition {
	uint8_t stack[STACK_SIZE];
	uint16_t stackSize;
	uint8_t used;
} Partition;


typedef struct TaskControlBlock {
    void (*taskFunctionPtr)(void*);
    void *stackPtr;
	uint8_t used;
} TaskControlBlock;

uint8_t addTask(void (*taskFunctionPtr)(void *arg), void *arg);

void init_sharing_time1();

ISR(TIMER1_COMPA_vect, ISR_NAKED);

void saveContext1(void);

void restoreContext1(void);

void saveContext(void);

void restoreContext(void);

#endif  // _KERNEL_H
