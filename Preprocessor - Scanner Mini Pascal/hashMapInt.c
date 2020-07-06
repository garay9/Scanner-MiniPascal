#include <stdio.h>
#include <stdlib.h>

struct node{
    int key;
    int val;
    struct node *next;
};
struct tableInt{
    int size;
    struct node **list;
};
struct tableInt *createTableInt(int size){
    struct tableInt *t = (struct tableInt*)malloc(sizeof(struct tableInt));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}

int hashCodeInt(struct tableInt *t,int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}

void insertInt(struct tableInt *t,int key,int val){
    int pos = hashCodeInt(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}

int lookupInt(struct tableInt *t,int key){
    int pos = hashCodeInt(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}
