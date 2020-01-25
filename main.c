#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodeVacell.h"


int main()
{
    srand(time(0));
    bool loop=true;
    int p;
    int i,j,n,c;
    scanf("%d",&n);
    int **ma_p;
    ma_p=(int**)malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
    {
        ma_p[i]=(int*)malloc(n*sizeof(int));
        for (j=0;j<n;j++){
            ma_p[i][j]=rand()%4+1;
            printf("%d ",ma_p[i][j]);
        }
        printf("\n");
    }
    node* head=make_network(ma_p,n);
    
    scanf("%d",&c);
    cell* cell_head=create_cell(c,n,head);

    /*while (loop)
    {
        printf("[1]move cell\n[2]split cell\n[3]boost energy\n[4]exit\n");
        scanf("%d",&i);
        if (i==1)
        {
            printf("which one of the following cells ?\n");
            print_cells(cell_head);
            scanf("%d",&p);
            printf("in which direction ?\n[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
            scanf("%d",&j);
            cell_move(cell_head,p,j);
        }
        else if (i==2)
        {
            printf("which one of the following cells ?\n");
            print_cells(cell_head);
            scanf("%d",&p);
            split_cell(&cell_head,p);
        }
        else if (i==3)
        {
            printf("which one of the following cells ?\n");
            print_cells(cell_head);
            scanf("%d",&p);
            gain_energy(cell_head,p);
        }
        else if (i==4)
            loop=false;
        else 
            printf("invalid input\n");
    }*/
    while (loop)
    {
        printf("which one of the following cells ?\n");
        print_cells(cell_head);
        scanf("%d",&p);
        printf("[1]move cell\n[2]split cell\n[3]boost energy\n[4]exit\n");
        scanf("%d",&i);
        if (i==1)
        {
            printf("in which direction ?\n[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
            scanf("%d",&j);
            cell_move(cell_head,p,j);
        }
        else if (i==2)
        {
            split_cell(&cell_head,p);
        }
        else if (i==3)
        {
            gain_energy(cell_head,p);
        }
        else if (i==4)
            loop=false;
        else 
            printf("invalid input\n");
    }
    return 0;
}
