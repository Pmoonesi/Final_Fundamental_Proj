#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int type;
    struct node* north;
    struct node* north_east;
    struct node* north_west;
    struct node* south;
    struct node* south_east;
    struct node* south_west;
} node;

node* create_node(int n)
{
    node* my_node;
    my_node=(node*)malloc(sizeof(node));
    my_node->type=n;
    my_node->south_west=NULL;
    my_node->south_east=NULL;
    my_node->south=NULL;
    my_node->north_west=NULL;
    my_node->north_east=NULL;
    my_node->north=NULL;
    return my_node;

}

node* create_node_line (int i,int **map,int size) //i shomare khat, pointer be avalin node e oon khat ro bar migardoone
{
    node* head=create_node(map[i][0]);
    node* current=head;
    node* new_node;
    int f;
    for (f=1;f<size;f++)
    {
        new_node=create_node(map[i][f]);
        if (f%2==1)
        {
            current->south_east=new_node;
            new_node->north_west=current;
            current=current->south_east;
        }
        else
        {
            current->north_east=new_node;
            new_node->south_west=current;
            current=current->north_east;
        }

    }
    return head;
}

void merge_lines(node* head1,node* head2,int size)
{
    int i;
    node* current1=head1,*current2=head2;
    current2->north=current1;
    current1->south=current2;
    current1=current1->south_east;
    current2->north_east=current1;
    current1->south_west=current2;
    current2=current2->south_east;
    for (i=1;i<size;i++)
    {
        if (i%2==1)
        {
            current2->north=current1;
            current1->south=current2;
            current2=current2->north_east;
        }
        else if (i!=size-1)
        {
            current2->north_west=current1;
            current1->south_east=current2;
            current1=current1->north_east;
            current2->north=current1;
            current1->south=current2;
            if (i!=size-1){
                current1=current1->south_east;
                current2->north_east=current1;
                current1->south_west=current2;
                current2=current2->south_east;
            }
        }
    }
}

node* make_network(int **map,int size)
{
    int i;
    node *node1=create_node_line(0,map,size),*node2;
    for (i=1;i<size;i++)
    {
        node2=create_node_line(i,map,size);
        merge_lines(node1,node2,size);
        node1=node2;
    }
    return node1;
}

int main()
{
    int i,j,n;
    scanf("%d",&n);
    int **ma_p;
    ma_p=(int**)malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
    {
        ma_p[i]=(int*)malloc(n*sizeof(int));
        for (j=0;j<n;j++)
            ma_p[i][j]=i+j;

    }
    node* zerozero=make_network(ma_p,n);
    printf("%d",zerozero->type);
    zerozero=zerozero->north;
    printf("%d",zerozero->type);
    return 0;
}
