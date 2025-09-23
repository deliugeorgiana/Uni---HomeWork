#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct nnode{
    int value;
    struct nnode *next;
    struct nnode *previous;
};

typedef struct nnode node;

void rem(node *item);
void create(node **item, int val);

int main(int argc, char **argv){
    int i;
    int n=atoi(argv[1]);
    node *item, *start,*interm;
    interm=NULL;


    for(i=0;i<n;i++){
        if((item=malloc(sizeof(node)))==NULL) printf("Malloc error\n");
        if(i==0)    start=item;
        if(i!=0) interm->next=item;
        item->value=i;
        item->previous=interm;
        item->next=NULL;
        interm=item;
    }

    printf("This is the list :\n");
    item=start;
    i=0;
    do{
        printf("item %i value %i\n",i,item->value);
        item=item->next;
        i++;
    } while (item!=NULL);
    printf("\n\n");

    item=start;
    for(i=0;i<atoi(argv[3])-1;i++)
        item=item->next;
    create(item, atoi(argv[4]));

    void rem(node *item){
      node *previous, *next;
      previous=item->previous;
      next=item->next;
      free(item);
      next->previous=previous;
        previous->next=next;
    }

    void create(node **item, int val){
        new->value=val;
        new->previous=item;
        if(item->next==NULL) new->next=NULL;
        else new->next=item->next;
        item->next=new;
        (item->next)->previous=new;
    }

    return 0;
}

