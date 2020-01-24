#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct{
    int x,y;
}point;

int name_search(char* name,FILE* fp)
{
    FILE *ftemp=fp;
    fseek(ftemp,0,SEEK_SET);
    char* temp=malloc(8*sizeof(char));
    while(fscanf(ftemp,"%s",temp)!=EOF)
    {
        if (strcmp(name,temp)==0)
            return 1;
    }
    return 0;
}

void write_name(FILE* fp,char* name)
{
    int i;
    for (i=0;i<7;i++)
        fputc(name[i],fp);
    fputc('\n',fp);
}

char* rand_name(void)
{
    FILE* fp=fopen("cell.txt","a+");
    if (fp==NULL)
    {
        printf("Cannot open cell list\n");
        exit(-1);
    }
    srand(time(0));
    char *name,c[2];
    name=(char*)malloc(8*sizeof(char));
    int i,type;
    do
    {
        c[1]='\0';
        for (i=0;i<7;i++)
        {
            type=rand()%3;
            if (type==0)
                c[0]=rand()%26+'a';
            else if (type==1)
                c[0]=rand()%26+'A';
            else
                c[0]=rand()%10+'0';
            strcat(name,c);
        }
    }while(name_search(name,fp));
    fclose(fp);
    fp=fopen("cell.txt","a");
    fprintf(fp,"%s\n",name);
    fclose(fp);
    return name;
}

typedef struct{
    node* location;
    point position;
    int energy;
    char* name;
    struct cell* next;
}cell;

node* find_node(point p,node* head)
{
    node* current=head;
    int i;
    for (i=0;i<p.y;i++)
        current=current->north;
    for (i=0;i<p.x;i++)
    {
        if (i%2==0)
            current=current->south_east;
        else
            current=current->north_east;
    }
    return current;
}

cell* create_cell(int in_num,int size,node* head)
{
    int i;
    srand(time(0));
    point p;
    cell* cell_head = malloc (sizeof(cell)),*current;
    cell_head->energy=0;
    cell_head->name=rand_name();
    cell_head->next=NULL;
    p.x=rand()%size;
    p.y=rand()%size;
    cell_head->position=p;
    cell_head->location=find_node(p,head);
    current=cell_head;
    for (i=1;i<in_num;i++)
    {
        cell* new_cell=malloc(sizeof(cell));
        new_cell->energy=0;
        sleep(1);
        new_cell->name=rand_name();
        new_cell->next=NULL;
        p.x=rand()%size;
        p.y=rand()%size;
        new_cell->position=p;
        new_cell->location=find_node(p,head);
        current->next=new_cell;
        current=current->next;
    }
    return cell_head;
}

