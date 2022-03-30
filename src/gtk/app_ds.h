#ifndef DS_H
#define DS_H
#define STACK_MAX 10

#include <gtk/gtk.h>

struct Stack{
	GdkPixbuf *undo_buffer;
	struct Stack *next;
};

extern struct Stack *undo;

gboolean is_full_stack(struct Stack*);
gboolean is_empty_stack(struct Stack*);
struct Stack *push_stack(GdkPixbuf *, struct Stack *);
GdkPixbuf *pop_stack(struct Stack **);

#endif