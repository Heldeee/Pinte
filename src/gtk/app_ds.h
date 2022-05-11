#ifndef DS_H
#define DS_H
#define STACK_MAX 10

#include <gtk/gtk.h>

struct Stack{
	GdkPixbuf *undo_buffer;
	struct Stack *next;
};


struct Queue{
	struct Point *point;
	struct Queue *next;
};

struct Point{
	gint x;
	gint y;
};

struct Color{
	gint red;
	gint green;
	gint blue;
};

extern struct Queue *front;
extern struct Stack *undo;
extern struct Color foreground;

gboolean is_full_stack(struct Stack*);
gboolean is_empty_stack(struct Stack*);
struct Stack *push_stack(GdkPixbuf *, struct Stack *);
GdkPixbuf *pop_stack(struct Stack **);
gboolean is_queue_empty();
struct Queue *push_queue(gint x, gint y, struct Queue *front);
struct Point *pop_queue();

#endif
