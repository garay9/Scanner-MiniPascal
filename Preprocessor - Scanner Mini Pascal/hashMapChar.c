#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
    int key;
    char val[127];
    struct node *next;
};
struct tableChar{
    int size;
    struct node **list;
};
struct tableChar *createTableChar(int size){
    struct tableChar *t = (struct tableChar*)malloc(sizeof(struct tableChar));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}

int hashCodeChar(struct tableChar *t,int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}

void insertChar(struct tableChar *t,int key,char val[127]){
    int pos = hashCodeChar(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            strcpy(temp->val, val);
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    strcpy(newNode->val, val);
    newNode->next = list;
    t->list[pos] = newNode;
}

char* lookupChar(struct tableChar *t,int key){
    int pos = hashCodeChar(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return "";
}

/*int main(int argc, char **argv){
    struct tableChar* tabla = createTableChar(10);
    char prueba[127] = "salu3";
    insertChar(tabla, 0, prueba);
    printf("%s", lookupChar(tabla, 0));

    return 0;
}*/
