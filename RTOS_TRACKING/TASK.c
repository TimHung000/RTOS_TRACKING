#include "TASK.h"


/**
 * Insert a new entry between two known consecutive entries. 
 */
static void __list_add(struct listNode *new, struct listNode *prev, struct listNode *next) {
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static void list_add(struct listNode *new, struct listNode *head) {
	__list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static void list_add_tail(struct listNode *new, struct listNode *head) {

	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 */
static inline void __list_del(struct listNode *prev, struct listNode *next) {
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct listNode *entry) {
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}
