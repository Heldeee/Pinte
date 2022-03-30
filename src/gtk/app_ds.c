#include "app_ds.h"

// UNDO Stack
gboolean is_full_stack(struct Stack *base){
/*
	This checks whether the stack is full.
*/
	struct Stack *current;
	int count = 0;
	for(current = base, count = 0; current->next != NULL; count++, current = current->next);
	return (count == STACK_MAX);
}

gboolean is_empty_stack(struct Stack *base){
/*
	This checks whether the stack is empty.
*/
	return (base == NULL);
}

struct Stack *push_stack(GdkPixbuf *buffer, struct Stack *base){
/*
	The following function pushes data into it by checking whether it is full.
	Incase it is full, the base pointer is incremented to next Pixbuf and the
	previous one is freed.
*/
	struct Stack *current, *new_buffer, *temp;
	new_buffer = (struct Stack *)malloc(sizeof(struct Stack *));
	new_buffer->undo_buffer = buffer;
	new_buffer->next = NULL;
	if(is_empty_stack(base)){
		return new_buffer;
	}
	if(is_full_stack(base)){
		temp = base;
		base = base->next;
		free(temp);
	}
	for(current = base; current->next != NULL; current = current->next);
	current->next = new_buffer;
	return base;
}

GdkPixbuf *pop_stack(struct Stack **base){
/*
	This pops the Pixbuf of the stack.
*/
	struct Stack *current, *temp;
	GdkPixbuf *buffer;
	if((*base)->next == NULL){
		buffer = (*base)->undo_buffer;
		*base = NULL;
	}
	else{
		for(current = *base, temp = *base; current->next != NULL ; temp = current, current = current->next);
		buffer = current->undo_buffer;
		temp->next = NULL;
	}
	return buffer;
}