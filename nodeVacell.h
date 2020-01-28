#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "map_editor.h"
// typedef struct{
//     int x,y;
// }point;

struct node{
    int type;
    point position;
    int source;
    bool empty;
    struct node* north;
    struct node* north_east;
    struct node* north_west;
    struct node* south;
    struct node* south_east;
    struct node* south_west;
};

typedef struct node node;

node* create_node(int n,int xx,int yy)
{
    node* my_node;
    my_node=(node*)malloc(sizeof(node));
    my_node->type=n;
    my_node->source=0;
    if (n==1)
        my_node->source=100;
    my_node->position.x=xx;
    my_node->position.y=yy;
    my_node->empty=true;
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
    node* head=create_node(map[i][0],0,size-i-1);
    node* current=head;
    node* new_node;
    int f;
    for (f=1;f<size;f++)
    {
        new_node=create_node(map[i][f],f,size-i-1);
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
        else if (i%2==0)
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

int name_search(char* name,FILE* fp)
{
    FILE *ftemp=fp;
    fseek(ftemp,0,SEEK_SET);
    char* temp;
    temp=(char*)malloc(8*sizeof(char));
    while(1)
    {
        fread(temp,sizeof(char),8,ftemp);
        if (strcmp(name,temp)==0)
            return 1;
        if (feof(ftemp))
            return 0;
    }
}


char* rand_name(void)
{
    //FILE* ff=fopen("cell.bin","wb");
    //fclose(ff);
    FILE* fp=fopen("cell.bin","rb");
    if (fp==NULL)
    {
        printf("Cannot open cell list\n");
        exit(-1);
    }
    srand(time(0));
    char *name,c[2];
    int i,type;
    c[1]='\0';
    do
    {
        name=(char*)calloc(8,sizeof(char));
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
    fp=fopen("cell.bin","ab");
    fseek(fp,0,SEEK_END);
    fwrite(name,sizeof(char),8,fp);
    fclose(fp);
    return name;
}

struct cell{
    node* location;
    int energy;
    char* name;
    struct cell* next;
};

typedef struct cell cell;

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
    if (current->empty==false || current->type==3)          //inja bayad type e node ro ham lahaz koni
        return NULL;
    return current;
}

node* find_node_sp(point p,node* head) //copy tabe bala faghat baraye save ok shode
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
    cell* cell_head,*current;
    cell_head=(cell*) malloc (sizeof(cell));
    if (cell_head==NULL)
    {
        printf("problem with malloc\n");
        exit(-1);
    }
    cell_head->name=(char*)malloc(8*sizeof(char));
    cell_head->energy=0;
    cell_head->name=rand_name();
    cell_head->next=NULL;
    cell_head->location=NULL;

    while(cell_head->location==NULL)
    {
        p.x=rand()%size;
        p.y=rand()%size;
        cell_head->location=find_node(p,head);
    }
    (cell_head->location)->empty=false;         //por shodan node!
    current=cell_head;
    for (i=1;i<in_num;i++)
    {
        cell* new_cell;
        new_cell=(cell*)malloc(sizeof(cell));
        if (new_cell==NULL)
        {
            printf("problem with malloc\n");
            exit(-1);
        }
        new_cell->name=(char*)calloc(8,sizeof(char));
        new_cell->energy=0;
        new_cell->name=rand_name();
        new_cell->next=NULL;
        new_cell->location=NULL;

        while(new_cell->location==NULL)
        {
            p.x=rand()%size;        //bayad chek koni ke position esh tekrari nashe !
            p.y=rand()%size;
            new_cell->location=find_node(p,head);
        }
        (new_cell->location)->empty=false;          //por shodan node!
        current->next=new_cell;
        current=current->next;
    }
    return cell_head;
}

cell* find_cell(cell* head,int th)
{
    int i=1;
    cell* current=head;
    while (current!=NULL && i<th)
    {
        current=current->next;
        i++;
    }
    if (current==NULL){
        printf("wrong address\n");
    }
    return current;
}

bool allowd_place(node* place)
{
    if (place!=NULL && place->type!=3 && place->empty==true)
        return true;
    /*if (place==NULL)
        printf("NULL\n");
    else if (place->type==3)
        printf("forbidden\n");
    else if (place->empty==false)
        printf("taken\n");*/
    return false;
}

int rand_adj (node* place)
{
    int res=0,i;
    char c[2],*adj;
    c[1]='\0';

    adj=(char*)calloc(7,sizeof(char));
    if(allowd_place(place->north)==1)
    {
        c[0]='1';
        strcat(adj,c);
    }
    if(allowd_place(place->south)==1)
    {
        c[0]='2';
        strcat(adj,c);
    }
    if(allowd_place(place->north_east)==1)
    {
        c[0]='3';
        strcat(adj,c);
    }
    if(allowd_place(place->north_west)==1)
    {
        c[0]='4';
        strcat(adj,c);
    }
    if(allowd_place(place->south_east)==1)
    {
        c[0]='5';
        strcat(adj,c);
    }
    if(allowd_place(place->south_west)==1)
    {
        c[0]='6';
        strcat(adj,c);
    }
    if (strlen(adj)==0)
        return 0;
    i=rand()%strlen(adj);
    res=adj[i]-'0';
    return res;
}

void cell_move(cell* head,int th,int direction)
{
    cell* current=find_cell(head,th);
    if (current==NULL){
        return ;
    }
    node* place=current->location;
    switch(direction)
    {
        case 1:{
            place=place->north;
            break;
        }
        case 2:{
            place=place->south;
            break;
        }
        case 3:{
            place=place->north_east;
            break;
        }
        case 4:{
            place=place->north_west;
            break;
        }
        case 5:{
            place=place->south_east;
            break;
        }
        case 6:{
            place=place->south_west;
            break;
        }
        default:{
            printf("wrong input\n");
            return ;
            break;
        }
    }
    if (allowd_place(place)){
        (current->location)->empty=true;
        current->location=place;
        (current->location)->empty=false;
        printf("moved successfully\n");
    }
    else
    {
        printf("bad move\n");
    }
}

void print_cells(cell* head)
{
    int i=1;
    while(head!=NULL)
    {
        printf("%d) %s (%d, %d)\t %d\n",i,head->name,(head->location)->position.x,(head->location)->position.y,head->energy);
        head=head->next;
        i++;
    }
}

void delete_cell(cell** head,int th)
{
    if (th==1)
    {
        *head=(*head)->next;
    }
    else
    {
        cell* current=find_cell(*head,th-1);
        cell* next_cell;
        next_cell=current->next;
        current->next=next_cell->next;
    }
}

void split_cell(cell** headhead,int th)
{
    int dir;
    cell* head=*headhead;
    cell* current_c=find_cell(head,th);
    if (current_c==NULL)
        return ;
    node* current_n=current_c->location;
    dir=rand_adj(current_n);
    if (current_n->type!=2 || !dir || current_c->energy<80)//(|| current_c->energy<80)
    {
        printf("Can't be done\n");
        return ;
    }
    cell* new_cell1,*new_cell2;
    new_cell1=(cell*)malloc(sizeof(cell));
    new_cell2=(cell*)malloc(sizeof(cell));
    if (new_cell1==NULL)
    {
        printf("problem with malloc\n");
        exit(-1);
    }
    new_cell1->name=(char*)malloc(8*sizeof(char));
    new_cell1->energy=40;
    new_cell1->name=rand_name();
    new_cell1->next=NULL;
    new_cell1->location=current_n;
    (new_cell1->location)->empty=false;
    if (new_cell2==NULL)
    {
        printf("problem with malloc\n");
        exit(-1);
    }
    new_cell2->name=(char*)malloc(8*sizeof(char));
    new_cell2->energy=40;
    new_cell2->name=rand_name();
    new_cell2->next=NULL;
    new_cell2->location=current_n;
    (new_cell2->location)->empty=false;
    cell_move(new_cell2,1,dir);
    while(current_c->next!=NULL)
        current_c=current_c->next;
    current_c->next=new_cell1;
    new_cell1->next=new_cell2;
    delete_cell(headhead,th);
    printf("split completed\n");
}

void gain_energy(cell* head,int th)
{
    cell* current=find_cell(head,th);
    if (current==NULL)
        return ;
    int need=100-(current->energy);
    int left=(current->location)->source;
    if ((current->location)->type!=1)
    {
        printf("it is not a source cell\n");
        return ;
    }
    else if (need==0){
        printf("full energy!\n");
        return ;
    }
    else if (left==0)
    {
        printf("no energy left here\n");
        return ;
    }
    else if (left<15 && need <15)
    {
        if (left>=need)
        {
            current->energy=100;
            (current->location)->source-=need;
        }
        else
        {
            current->energy+=left;
            (current->location)->source=0;
        }
    }
    else if (left>=15 && need<15)
    {
        current->energy=100;
        (current->location)->source-=need;
    }
    else if (left<15 && need>=15)
    {
        current->energy+=left;
        (current->location)->source=0;
    }
    else
    {
        current->energy+=15;
        (current->location)->source-=15;
    }
    printf("successfull boost\n");
}

int cell_count(cell* cell_head)
{
    int res=0;
    while(cell_head!=NULL)
    {
        cell_head=cell_head->next;
        res++;
    }
    return res;
}
