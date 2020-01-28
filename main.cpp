#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "saveVaload.h"

int clr[4]={14,2,4,1};
int clr2[4]={14,10,12,9};
int r=30;
int TazMabda=2*r,AazMabda=3*r;

point whereitat(point pos)
{
    point res;
    res.x=TazMabda+pos.x*r*sqrt(3);
    if (pos.x%2==0)
        res.y=TazMabda+2*r*pos.y;
    else
        res.y=TazMabda+2*r*pos.y+r;
    return res;
}



void draw_map(int** ma_p,int size)
{
    point temp;
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            temp.x=i,temp.y=j;
            temp=whereitat(temp);
            setfillstyle(8,clr[ma_p[j][i]-1]);
            fillellipse(temp.x,temp.y,r,r);
            setfillstyle(1,BLACK);
        }
    }
}

void show_cells(cell* cell_head1,cell* cell_head2,int size,int player)
{
    int cell_r,t;
    if (player==2)
        t=1;
    else
        t=2;
    const int color[2]={0,15};
    point temp;
    while(cell_head1!=NULL)
    {
        temp=(cell_head1->location)->position;
        cell_r=(r*3/5.0)*(cell_head1->energy/100.0)+r*(1/5.0);
        temp.y=size-temp.y-1;
        temp=whereitat(temp);
        setfillstyle(1,color[player-1]);
        fillellipse(temp.x,temp.y,cell_r,cell_r);
        cell_head1=cell_head1->next;
    }
    if (cell_head2==NULL)
        return ;
    while(cell_head2!=NULL)
    {
        temp=(cell_head2->location)->position;
        cell_r=(r*3/5.0)*(cell_head2->energy/100.0)+r*(1/5.0);
        temp.y=size-temp.y-1;
        temp=whereitat(temp);

        setfillstyle(1,color[t-1]);
        fillellipse(temp.x,temp.y,cell_r,cell_r);
        cell_head2=cell_head2->next;
    }
}

void play_that(node** head,cell** cell_head1,cell** cell_head2,int** ma_p,int size,int player,bool* loop)
{
    //initwindow(800,800);
    //setfillstyle(0,0);
    setfillstyle(1,BLACK);
    bar(0,0,800,800);
    printf("PLAYER%d 's turn\n",player);
    int p,i,j;
    //show_map(ma_p,size);
    draw_map(ma_p,size);
    show_cells(*cell_head1,*cell_head2,size,player);
    printf("which one of the following cells ?\n");
    print_cells(*cell_head1);
    scanf("%d",&p);
    if (find_cell(*cell_head1,p)==NULL){
        return ;
    }

    printf("[1]move cell\n[2]split cell\n[3]boost energy\n[4]save\n[5]exit\n");
    scanf("%d",&i);
    if (i==1)
    {
        printf("in which direction ?\n[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
        scanf("%d",&j);
        cell_move(*cell_head1,p,j);
    }
    else if (i==2)
    {
        split_cell(cell_head1,p);
    }
    else if (i==3)
    {
        gain_energy(*cell_head1,p);
    }
    else if (i==4)
    {
        if (player==1)
            save_game(*head,*cell_head1,*cell_head2,ma_p,size);
        else
            save_game(*head,*cell_head2,*cell_head1,ma_p,size);
    }
    else if (i==5){
        *loop=false;
        closegraph();
    }
    else
        printf("invalid input\n");
    //closegraph();
}

int main()
{
    srand(time(0));
    bool outloop=true;
    while(outloop){
        bool loop=true;
        int size,cell_num;

        int choice,num_players;
        printf("[1]Load\n[2]New single player game\n[3]New multiplayer game\n[4]Exit\n");
        scanf("%d",&choice);
        int **ma_p;
        cell *cell_head1=NULL;
        cell *cell_head2=NULL;
        node *head;
        if (choice==1)
        {
            printf("[1]single player save\n[2]multiplayer save\n");
            scanf("%d",&num_players);
            load_game(&head,&cell_head1,&cell_head2,&ma_p,&size,num_players);
        }
        else if (choice==2)
        {
            FILE* fp=fopen("map.bin","rb");
            fread(&size,sizeof(int),1,fp);
            ma_p=read_file(fp);
            fclose(fp);
            head=make_network(ma_p,size);
            printf("please enter initial number of cells : ");
            scanf("%d",&cell_num);
            cell_head1=create_cell(cell_num,size,head);
        }
        else if (choice==3)
        {
            FILE* fp=fopen("map.bin","rb");
            fread(&size,sizeof(int),1,fp);
            ma_p=read_file(fp);
            fclose(fp);
            head=make_network(ma_p,size);
            printf("PLAYER1 : please enter initial number of cells : ");
            scanf("%d",&cell_num);
            cell_head1=create_cell(cell_num,size,head);
            printf("PLAYER2 : please enter initial number of cells : ");
            scanf("%d",&cell_num);
            cell_head2=create_cell(cell_num,size,head);
        }
        else if (choice==4)
        {
            loop=false;
            outloop=false;
        }

        initwindow(800,800);
        setfillstyle(1,BLACK);
        bar(0,0,800,800);
        /*show_cells(cell_head1,size,1);
        if (cell_head2!=NULL)
            show_cells(cell_head2,size,1);*/
        while (loop)
        {
            play_that(&head,&cell_head1,&cell_head2,ma_p,size,1,&loop);
            if (loop==false)
                break;
            if(cell_head2!=NULL)
                play_that(&head,&cell_head2,&cell_head1,ma_p,size,2,&loop);
        }
    }
    closegraph();
    return 0;
}
