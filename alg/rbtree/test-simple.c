#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rbtree.h"

#define offset_of(type, member) (&((type *)0)->member - 0)
#define container_of(ptr, type, member) \
    ((type *)((const char *)ptr - (int)offset_of(type, member)))
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

typedef struct mydata {
	struct rb_node rbnode;
	int data;
} mydata;

struct mydata * rb_insert_data(struct rb_root * root,
		int data, struct rb_node * node)
{
	struct rb_node ** p = &root->rb_node;
	struct rb_node * parent = NULL;
	struct mydata * mydata;

	while (*p) {
		parent = *p;
		mydata = rb_entry(parent, struct mydata, rbnode);

		if (data < mydata->data)
			p = &(*p)->rb_left;
		else if (data > mydata->data)
			p = &(*p)->rb_right;
		else
			return mydata;
	}

	rb_link_node(node, parent, p);
	rb_insert_color(node, root);
	return NULL;
}

struct mydata * rb_search(struct rb_root * root, int data)
{
	struct rb_node * n = root->rb_node;
	struct mydata * d;

	while (n) {
		d = rb_entry(n, struct mydata, rbnode);

		if (data < d->data)
			n = n->rb_left;
		else if (data > d->data)
			n = n->rb_right;
		else
			return d;
	}
	return NULL;
}

void print_rbtree(struct rb_root * root)
{
	struct rb_node * node = rb_first(root);
	struct mydata * d;

	while (node) {
		d = rb_entry(node, struct mydata, rbnode);
		printf("data = %d\n", d->data);
		node = rb_next(node);
	}
}

void insert100(void)
{
	struct rb_root root = RB_ROOT;
	struct mydata * new;
	int data;
	int i;

	for (i = 0; i < 100; i++) {
		data = i;
		new = (struct mydata *)malloc(sizeof(*new));
		new->data = data;
		rb_insert_data(&root, data, &new->rbnode);
	}

	print_rbtree(&root);
	rb_search(&root, 75);
}

int main(int argc, char * argv[])
{
	struct rb_root root = RB_ROOT;
	struct mydata * new;
	int data;
	int i;

	if (argc == 1) {
		insert100();
		return 0;
	}

	for (i = 1; i < argc; i++) {
		data = atoi(argv[i]);
		new = (struct mydata *)malloc(sizeof(*new));
		new->data = data;
		rb_insert_data(&root, data, &new->rbnode);
	}

	print_rbtree(&root);
	return 0;
}
