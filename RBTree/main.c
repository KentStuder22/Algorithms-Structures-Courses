#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct nodeStruct{
    int data;
    char color;
    struct nodeStruct *parent;
    struct nodeStruct *left;
    struct nodeStruct *right;
}Node;

typedef struct treeStruct{
    Node *root;
    Node *leaf;
}RBTree;

#define MAX_LINE 255

//Prototypes
Node *createNode(int);
RBTree *initRBTree(void);
void leftRotate(RBTree*, Node*);
void rightRotate(RBTree*, Node*);
void RBInsert(RBTree*, int);
void RBFixUp(RBTree*, Node*);
void printRBTree(Node*);
int getTreeHeight(Node*);
int getTreeBH(Node*);
bool isBlack(Node*);
int max(int, int);
void freeTree(Node*);
int secondLargest(Node*);

int main(){
    
    RBTree *Tree = initRBTree();
    
    bool isRunning = true;
    
    while(isRunning){
        int temp;
        if(scanf("%d,",&temp) == 1){
            RBInsert(Tree, temp);
        }else{
            isRunning = false;
        }
    }
    printf("In-order traversal of the tree:\n");
    printRBTree(Tree->root);
    printf("\nThe height of the red and black tree is %d", getTreeHeight(Tree->root));
    printf("\nThe black height of the red and black tree is %d", getTreeBH(Tree->root));
    printf("\nThe second largest element of the tree is %d\n", secondLargest(Tree->root));
    
    freeTree(Tree->leaf);
    free((void*)Tree);
    
    return 0;
}
Node *createNode(int data){
    Node *new = malloc(sizeof(Node));
    if(new == NULL){
        return NULL;
    }else{
        new->left = new->right = new->parent = NULL;
        new->color = 'R';
        new->data = data;
        
        return new;
    }
}
RBTree *initRBTree(){
    RBTree *Tree = malloc(sizeof(RBTree));
    Node *NIL = malloc(sizeof(Node));
    if(Tree == NULL || NIL == NULL){
        return NULL;
    }else{
        NIL->left = NIL->right = NIL->parent = NULL;
        NIL->color = 'B';
        NIL->data = 0;
        
        Tree->leaf = NIL;
        Tree->root = Tree->leaf;
        
        return Tree;
    }
}
void leftRotate(RBTree * Tree, Node *X){ //taken from RB slides
    Node *y = X->right;
    X->right = y->left;
    if(y->left != Tree->leaf){
        y->left->parent = X;
    }
    y->parent = X->parent;
    if(X->parent == Tree->leaf){
        Tree->root = y;
    }else if(X == X->parent->left){
        X->parent->left = y;
    }else{
        X->parent->right = y;
    }
    y->left = X;
    X->parent = y;
}
void rightRotate(RBTree * Tree, Node *X){
    Node *y = X->left;
    X->left = y->right;
    if(y->right != Tree->leaf){
        y->right->parent = X;
    }
    y->parent = X->parent;
    if(X->parent == Tree->leaf){
        Tree->root = y;
    }else if(X == X->parent->right){
        X->parent->right = y;
    }else{
        X->parent->left = y;
    }
    y->right = X;
    X->parent = y;
}
void RBInsert(RBTree *Tree, int key){
    Node *Y = Tree->leaf;
    Node *X = Tree->root;
    Node *Z = createNode(key);
    
    while(X != Tree->leaf){
        Y = X;
        if(Z->data < X->data){
            X = X->left;
        }else{
            X = X->right;
        }
    }
    
    Z->parent = Y;
    
    if(Y == Tree->leaf){
        Tree->root = Z;
    }else if(Z->data < Y->data){
        Y->left = Z;
    }else{
        Y->right = Z;
    }
    Z->left = Tree->leaf;
    Z->right = Tree->leaf;
    Z->color = 'R';
    RBFixUp(Tree, Z);
}
void RBFixUp(RBTree *Tree, Node *Z){
    while(Z->parent->color == 'R'){
        if(Z->parent == Z->parent->parent->left){
            Node *Y = Z->parent->parent->right;
            
            if(Y->color == 'R'){
                Z->parent->color = 'B';
                Y->color = 'B';
                Z->parent->parent->color = 'R';
                Z = Z->parent->parent;
            }else{
                if(Z == Z->parent->right){
                    Z = Z->parent;
                    leftRotate(Tree, Z);
                }
                Z->parent->color = 'B';
                Z->parent->parent->color = 'R';
                rightRotate(Tree, Z->parent->parent);
            }
        }
        else{
            Node *Y = Z->parent->parent->left;
            if(Y->color == 'R'){
                Z->parent->color = 'B';
                Y->color = 'B';
                Z->parent->parent->color = 'R';
                Z = Z->parent->parent;
            }else{
                if(Z == Z->parent->left){
                    Z = Z->parent;
                    rightRotate(Tree, Z);
                }
                Z->parent->color = 'B';
                Z->parent->parent->color = 'R';
                leftRotate(Tree, Z->parent->parent);
            }
        }
    }
    Tree->root->color = 'B';
}
int getTreeBH(Node* root){ //this function is not working properly i think
    if(root == NULL){
        return 1;
    }
    int LHeight = getTreeBH(root->left);
    if(LHeight == 0){
        return LHeight;
    }
    int RHeight = getTreeBH(root->right);
    if(RHeight == 0){
        return RHeight;
    }
    if(RHeight != LHeight){
        return 0;
    }else{
        return LHeight + (isBlack(root) ? 1 : 0);
    }
}
int getTreeHeight(Node *root){
    if(root == NULL){
        return -1;
    }
    return max(getTreeHeight(root->left), getTreeHeight(root->right)) + 1;
}
void printRBTree(Node *X){
    if(X->left != NULL || X->right != NULL){
        printRBTree(X->left);
        printf("%d-%c; ", X->data, X->color);
        printRBTree(X->right);
    }
}
int max(int a, int b){
    return a>b ? a : b;
}
void freeTree(Node *root){
    if(root != NULL){
        freeTree(root->left);
        freeTree(root->right);
        free((void*)root);
    }
}
bool isBlack(Node *root){
    if(root->color == 'B'){
        return true;
    }else{
        return false;
    }
}
int secondLargest(Node *root){
    while(root->right->right->data != 0){
        root = root->right;
    }
    return root->data;
}


