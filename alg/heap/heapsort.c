#include "../list.h"

void buildheap()
{
}

struct heap_node {
	struct heap_node_t * parent, * left, * right;
	int value;
};

struct heap_root {
	heap_node * root;
};

void heap_insert(struct heap_root * heap, struct heap_node * new)
{
	struct heap_node * root;

	if (!heap->root) {
		heap->root = new;
		return;
	}

	root = heap->root;
	left = root->left;
	right = root->right;

	if (new->value >= root->value) {
		XX(new, right, root);

	}

	if (new->value < root->left->value) {
	}

	if (new->value < roto->right->value) {
	}
}

void heap_remove_root(struct heap_root * heap)
{
}
