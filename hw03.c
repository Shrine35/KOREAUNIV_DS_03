1.
41[black]



2.
41[black]

    38[red]


3. 

    41[red]

38[black]

    31[red]



4.

    41[black]

38[black]

    31[black]

        12[red]



5. 

    41[black]

38[black]

        31[red]

    19[black]

        12[red]


6.

    41[black]

38[black]

        31[black]

    19[red]

        12[black]

            8[red]

#include <stdio.h>
#include <malloc.h>

typedef struct RB_Node* RBNodePtr;
typedef struct RB_Node {
	int key;
	int red;   //red = 0 일시 black 
	RBNodePtr left;
	RBNodePtr right;
	RBNodePtr parent;
}RB_Node;

typedef struct RB_Tree* RBPtr;
typedef struct RB_Tree {
	RBNodePtr root;
	RBNodePtr nil;
}RB_Tree;


void rbt_insert(RBPtr self, RBNodePtr n);
void rbt_insert_fixup(RBPtr self, RBNodePtr n);
void left_rotate(RBPtr self, RBNodePtr n);
void right_rotate(RBPtr self, RBNodePtr n);
void rbt_print(RBPtr self, RBNodePtr tree, int level);



RBPtr rbt_alloc() {
	RBPtr self = (RBPtr)malloc(sizeof(struct RB_Tree));
	self->nil = (RBNodePtr)malloc(sizeof(struct RB_Node));
	self->nil->red = 0;
	self->root = self->nil; //RB_Tree.root = RB_Tree.nil
	return self;
}

RBNodePtr node_alloc(int newkey) {
	RBNodePtr self = (RBNodePtr)malloc(sizeof(struct RB_Node));
	self->key = newkey;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
	self->red = 1;
	return self;
};



void rbt_insert(RBPtr self, RBNodePtr n) {

	RBNodePtr x, y;

	y = self->nil;
	x = self->root;
	while (x != self->nil) {
		y = x;
		if (n->key < x->key) {
			x = x->left;
		}
		else x = x->right;
	}
	n->parent = y;
	if (y == self->nil) {
		self->root = n;
	}
	else if (n->key < y->key) {
		y->left = n;
	}
	else y->right = n;
	n->left = self->nil;
	n->right = self->nil;
	n->red = 1;
	rbt_insert_fixup(self, n);
}

void rbt_insert_fixup(RBPtr self, RBNodePtr n) {

	RBNodePtr y;

	while (n->parent->red == 1) {
		if (n->parent == n->parent->parent->left) {
			y = n->parent->parent->right;
			if (y->red == 1) {
				n->parent->red = 0;
				y->red = 0;
				n->parent->parent->red = 1;
				n = n->parent->parent;
			}
			else {
				if (n == n->parent->right) {
					n = n->parent;
					left_rotate(self, n);
				}
				n->parent->red = 0;
				n->parent->parent->red = 1;
				right_rotate(self, n->parent->parent);
			}
		}
		else {
			y = n->parent->parent->left;

			if (y->red == 1) {
				n->parent->red = 0;
				y->red = 0;
				n->parent->parent->red = 1;
				n = n->parent->parent;
			}
			else {
				if (n == n->parent->left) {
					n = n->parent;
					right_rotate(self, n);
				}
				n->parent->red = 0;
				n->parent->parent->red = 1;
				left_rotate(self, n->parent->parent);
			}
		}
	}
	self->root->red = 0;
}

void left_rotate(RBPtr self, RBNodePtr n) {
	RBNodePtr y;

	y = n->right;
	n->right = y->left;
	if (n->left != self->nil)
		y->left->parent = n;
	y->parent = n->parent;
	if (n->parent == self->nil)
		self->root = y;
	else if (n == n->parent->left)
		n->parent->left = y;
	else n->parent->right = y;
	y->left = n;
	n->parent = y;
}

void right_rotate(RBPtr self, RBNodePtr n) {
	RBNodePtr y;

	y = n->left;
	n->left = y->right;
	if (n->right != self->nil)
		y->right->parent = n;
	y->parent = n->parent;
	if (n->parent == self->nil)
		self->root = y;
	else if (n == n->parent->right)
		n->parent->right = y;
	else n->parent->left = y;
	y->right = n;
	n->parent = y;
}

void rbt_print(RBPtr self, RBNodePtr tree, int level) {
	if (tree->right != self->nil)
		rbt_print(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%d", tree->key);
	if (tree->red == 1)
		printf("[red]\n\n");
	else printf("[black]\n\n");
	if (tree->left != self->nil)
		rbt_print(self, tree->left, level + 1);
}

int main() {
	RBPtr rbt = rbt_alloc();
	rbt_insert(rbt, node_alloc(41));
	rbt_insert(rbt, node_alloc(38));
	rbt_insert(rbt, node_alloc(31));
	rbt_insert(rbt, node_alloc(12));
	rbt_insert(rbt, node_alloc(19));
	rbt_insert(rbt, node_alloc(8));
	rbt_print(rbt, rbt->root, 0);
	printf("\n");
}


