#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveVaload.h"

void play_that(node** head,cell** cell_head1,cell** cell_head2,int ** ma_p,int size,int player,bool* loop)
{
    printf("PLAYER%d 's turn\n",player);
    int p,i,j;
    show_map(ma_p,size);
    printf("which one of the following cells ?\n");
    print_cells(*cell_head1);
    scanf("%d",&p);
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
            save_game(*head,*cell_head1,*cell_head2,"map.bin",ma_p,size);
        else 
            save_game(*head,*cell_head2,*cell_head1,"map.bin",ma_p,size);
    }
    else if (i==5){
        *loop=false;
    }
    else 
        printf("invalid input\n");
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
            load_game(&head,&cell_head1,&cell_head2,&ma_p,num_players);
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
        while (loop)
        {
            play_that(&head,&cell_head1,&cell_head2,ma_p,size,1,&loop);
            if (loop==false)
                break;
            if(cell_head2!=NULL)
                play_that(&head,&cell_head2,&cell_head1,ma_p,size,2,&loop);
        }
    }
    return 0;
}
