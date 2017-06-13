#include <stdio.h>
#include <malloc.h>
#include  <strsafe.h>
#include  <tchar.h>
#include  <windows.h>

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
void RB_Transplant(RBPtr self, RBNodePtr x, RBNodePtr n);
void rbt_delete(RBPtr self, RBNodePtr n);
void RB_delete_fixup(RBPtr self, RBNodePtr x);
RBNodePtr Tree_Minimum(RBPtr self, RBNodePtr n);
int get_node_count(RBPtr self, RBNodePtr n);
int get_blacknode_count(RBPtr self, RBNodePtr n);
int get_bh(RBPtr self, RBNodePtr n);
RBNodePtr RBT_Search(RBPtr self, RBNodePtr tree, int k);
void rbt_inorder_iter(RBPtr self, RBNodePtr tree);
RBNodePtr Tree_Successor(RBPtr self, RBNodePtr n);


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

RBNodePtr Tree_Successor(RBPtr self, RBNodePtr n) {
  if (n->right != self->nil) {
    return Tree_Minimum(self, n->right);
  }
  RBNodePtr x = n->parent;
  while (x != self->nil && n == x->right) {
    n = x;
    x = x->parent;
  }
  return x;
}


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
  if (y->left != self->nil)
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
  if (y->right != self->nil)
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

void RB_Transplant(RBPtr self, RBNodePtr x, RBNodePtr n) {
  if (x->parent == self->nil) {
    self->root = n;
  }
  else if (x == x->parent->left) {
    x->parent->left = n;
  }
  else x->parent->right = n;
  n->parent = x->parent;
}

RBNodePtr Tree_Minimum(RBPtr self, RBNodePtr n) {
  while (n->left != self->nil) {
    n = n->left;
  }
  return n;
}

void rbt_delete(RBPtr self, RBNodePtr n) {
  RBNodePtr x, y;
  int y_original_red;

  y = n;
  y_original_red = y->red;
  if (n->left == self->nil) {
    x = n->right;
    RB_Transplant(self, n, n->right);
  }
  else if (n->right == self->nil) {
    x = n->left;
    RB_Transplant(self, n, n->left);
  }
  else {
    y = Tree_Minimum(self, n->right);
    y_original_red = y->red;
    x = y->right;
    if (y->parent == n) {
      x->parent = y;
    }
    else {
      RB_Transplant(self, y, y->right);
      y->right = n->right;
      y->right->parent = y;
    }
    RB_Transplant(self, n, y);
    y->left = n->left;
    y->left->parent = y;
    y->red = n->red;
  }
  if (y_original_red == 0) {
    RB_delete_fixup(self, x);
  }
}

void RB_delete_fixup(RBPtr self, RBNodePtr x) {

  RBNodePtr w;

  while (x != self->root && x->red == 0) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->red == 1) {
        w->red = 0;
        x->parent->red = 1;
        left_rotate(self, x->parent);
        w = x->parent->right;
      }
      if (w->left->red == 0 && w->right->red == 0) {
        w->red = 1;
        x = x->parent;
      }
      else {
        if (w->right->red == 0) {
          w->left->red = 0;
          w->red = 1;
          right_rotate(self, w);
          w = x->parent->right;
        }
        w->red = x->parent->red;
        x->parent->red = 0;
        w->right->red = 0;
        left_rotate(self, x->parent);
        x = self->root;
      }
    }
    else {
      w = x->parent->left;
      if (w->red == 1) {
        w->red = 0;
        x->parent->red = 1;
        right_rotate(self, x->parent);
        w = x->parent->left;
      }
      if (w->right->red == 0 && w->left->red == 0) {
        w->red = 1;
        x = x->parent;
      }
      else {
        if (w->left->red == 0) {
          w->right->red = 0;
          w->red = 1;
          left_rotate(self, w);
          w = x->parent->left;
        }
        w->red = x->parent->red;
        x->parent->red = 0;
        w->left->red = 0;
        right_rotate(self, x->parent);
        x = self->root;
      }
    }
  }
  x->red = 0;
}

void rbt_inorder_iter(RBPtr self, RBNodePtr tree) {
  if (tree == self->nil) {
    return;
  }
  else {
    RBNodePtr q = tree;
    q = Tree_Minimum(self, q);
    while (q != self->nil)
    {
      printf("%d ", q->key);
      if (q->red == 1) {
        printf("R");
        printf("\n");
      }
      else {
        printf("B");
        printf("\n");
      }
      q = Tree_Successor(self, q);
    }
  }
}


int get_node_count(RBPtr self, RBNodePtr tree) {
  int count = 0;

  if (tree == self->nil) {
    return count;
  }
  RBNodePtr q = tree;
  q = Tree_Minimum(self, q);

  while (q != self->nil)
  {
    count += 1;
    q = Tree_Successor(self, q);
  }
  return count;
}

int get_blacknode_count(RBPtr self, RBNodePtr n) {
  int count = 0;

  if (n == self->nil) {
    return count;
  }
  RBNodePtr q = n;
  q = Tree_Minimum(self, q);
  if (q == n) {
    return count + 1;
  }
  while (q != self->nil)
  {
    if (q->red == 0) {
      count += 1;
    }
    q = Tree_Successor(self, q);
  }
  return count;
}

int get_bh(RBPtr self, RBNodePtr n) {
  int bh = 0;
  while (n != self->nil) {
    if (n->red == 0) {
      bh += 1;
    }
    n = n->left;
  }
  return bh;
}



RBNodePtr RBT_Search(RBPtr self, RBNodePtr tree, int k) {
  while (tree != self->nil && k != tree->key) {
    if (k < tree->key) {
      tree = tree->left;
    }
    else tree = tree->right;
  }
  return tree;
}


int main() {
  FILE *pFile, *fp;
  int num, insert_count = 0, delete_count = 0, miss_count = 0;
  WIN32_FIND_DATA ffd;
  TCHAR szDir[MAX_PATH];
  size_t length_of_arg;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  DWORD dwError = 0;

  RBPtr rbt = rbt_alloc();
  pFile = fopen("d:\\test03.txt", "r");
  if (pFile != NULL) {
    while (fscanf(pFile, "%d", &num) != EOF) {
      if (num > 0) {
        rbt_insert(rbt, node_alloc(num));
        insert_count += 1;
      }
      else if (num < 0) {
        num *= -1;
        RBNodePtr D = RBT_Search(rbt, rbt->root, num);
        if (D == rbt->nil) {
          fp = fopen("d:\\test.txt", "w");
          num *= -1;
          fprintf(fp, "NOT FOUND = %d\n", num);
          fclose(fp);
          miss_count += 1;
        }
        else {
          rbt_delete(rbt, D);
          delete_count += 1;
        }
      }
      else if (num == 0)
        break;
    }
    if (num == 0) {
      printf("filename = ");
      StringCchCopy(szDir, MAX_PATH, TEXT("d:"));
      StringCchCat(szDir, MAX_PATH, TEXT("\\test02.txt"));
      // Find the first file in the directory. 
      hFind = FindFirstFile(szDir, &ffd);
      if (INVALID_HANDLE_VALUE == hFind) {
        printf("Error: FindFirstFile\n");
        return dwError;
      }
      do
      {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          _tprintf(TEXT(" %s \n"), ffd.cFileName);
        }
        else {
          _tprintf(TEXT(" %s\n"), ffd.cFileName);
        }
      } while (FindNextFile(hFind, &ffd) != 0);

      dwError = GetLastError();
      if (dwError != ERROR_NO_MORE_FILES) {
        printf("Error: FindFirstFile\n");
      }
      FindClose(hFind);

      printf("total = ");
      printf("%d\n", get_node_count(rbt, rbt->root));

      printf("insert = ");
      printf("%d\n", insert_count);

      printf("delete = ");
      printf("%d\n", delete_count);

      printf("miss = ");
      printf("%d\n", miss_count);

      printf("nb = ");
      printf("%d\n", get_blacknode_count(rbt, rbt->root));

      printf("bh = ");
      printf("%d\n", get_bh(rbt, rbt->root));

      rbt_inorder_iter(rbt, rbt->root);
    }
  }
  else if (pFile == NULL)
    printf("입력 파일을 열지 못했습니다.\n");
  fclose(pFile);
  return dwError;
}

