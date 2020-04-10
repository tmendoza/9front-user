#include <u.h>
#include <libc.h>
#include "olist.h"

Node *
nodeinit(void *data, int type)
{
	Node *node = (Node *) malloc(sizeof(Node));

	if (node == nil) {
		werrstr("nodeinit: could not alloc mem for Node *: %r");
		return nil;
	}

	node->type = type;
	node->data = data;
	node->next = nil;

	return node;
}

Node * 
nodeappend(Node *head, void *d, int t)
{
	Node *end;
	Node *data;

	if (head == nil) {
		werrstr("nodeappend: head node nil.");
		return nil;
	}

	/* Go to end of the list */
	end=head;

	while (end->next != nil) 
		end=end->next;

 	data = nodeinit(d, t);

	end->next = data;

	return data;
}

void 
nodepush(Node **h, void *data, int type)
{
	Node *n = (Node *) malloc(sizeof(Node));

	n->data = data;
	n->type = type;

	n->next = (*h);
	(*h) = n;
}

void
nodeprintall(Node *h, objprinter p)
{
	Node *i;
	for (i=h; i != nil; i=i->next)
		p(i);
	return;
}

int 
nodecount(Node *n)
{
	if (n == nil)
		return 0;
	return 1 + nodecount(n->next);
}

typedef void (*objdestroy) (void *);


void 
nodefreelist(Node **n, objdestroy destructor)
{
	Node *cur = *n;
	Node *next;

	while(cur != nil)
	{
		next = cur->next;

		if (cur->data)
			destructor(cur->data);

		free(cur);
		cur = next;
	}

	*n = nil;
}

