#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientServerCommunication.h"

//Returns the bigger parameter
int max(int a, int b)
{
    if(a>b)
        return a;
    else
        return b;
}

//Calculates and returns the height of a node
int height(struct node *N)
{
    if(N==NULL)
        return 0;
    return (1+max(height(N->left), height(N->right)));
}

//Calculates and returns the balance of a node
int getBalance(struct node *N)
{
    if(N==NULL)
        return 0;
    return (height(N->left)-height(N->right));
}

//Does right rotation to balance the AVL tree
struct node *rightRotate(struct node *y)
{
    struct node *x=y->left;
    struct node *T2=x->right;

    x->right=y;
    y->left=T2;

    y->height=height(y);
    x->height=height(x);

    return x;
}

//Does left rotation to balance the AVL tree
struct node *leftRotate(struct node *x)
{
    struct node *y=x->right;
    struct node *T2=y->left;

    y->left=x;
    x->right=T2;

    x->height=height(x);
    y->height=height(y);

    return y;
}

struct node* insert(struct node * binary_tree, char city[500], int count, struct transactions* tran)
{
    struct node *tmp=NULL;
    if((binary_tree)==NULL)
    {
        tmp=(struct node *)malloc(sizeof(struct node));
        if(tmp==NULL)
            return NULL;

        tmp->left=tmp->right=NULL;
        strcpy(tmp->cityName, city);
        tmp->transactionCount=count;
        tmp->cityTransactions=tran;
        tmp->height=0;
        binary_tree=tmp;
        return (binary_tree);
    }

    if(strcmp(city, binary_tree->cityName)<0)
        binary_tree->left =insert(binary_tree->left, city, count, tran);
    else if(strcmp(city, binary_tree->cityName)>0)
        binary_tree->right=insert(binary_tree->right, city, count, tran);
    else
        return binary_tree;

    binary_tree->height = height(binary_tree);

    int balance = getBalance(binary_tree);

    if(balance>1 && strcmp(city, binary_tree->left->cityName)<0)
        return rightRotate(binary_tree);
    if(balance<-1 && strcmp(city, binary_tree->right->cityName)>0)
        return leftRotate(binary_tree);
    if(balance>1 && strcmp(city, binary_tree->left->cityName)>0)
    {
        binary_tree->left =  leftRotate(binary_tree->left);
        return rightRotate(binary_tree);
    }
    if(balance<-1 && strcmp(city, binary_tree->right->cityName)<0)
    {
        binary_tree->right = rightRotate(binary_tree->right);
        return leftRotate(binary_tree);
    }

    return binary_tree;
}

struct node *search(struct node ** binary_tree, char city[500])
{
    if(!(*binary_tree))
        return NULL;

    if(strcmp(city, (*binary_tree)->cityName)==0)
        return *binary_tree;
    else if(strcmp(city, (*binary_tree)->cityName)<0)
        return search(&((*binary_tree)->left), city);
    else
        return search(&((*binary_tree)->right), city);
}

void delete_tree(struct node * binary_tree)
{
    if(binary_tree!=NULL)
    {
        freePointer(binary_tree->cityTransactions);
        delete_tree(binary_tree->left);
        delete_tree(binary_tree->right);
        freePointer(binary_tree);
    }
}
