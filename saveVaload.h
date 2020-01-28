#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "nodeVacell.h"


char* now()
{
    char* buff;
    buff=(char*)malloc(25*sizeof(char));
    time_t t=time(NULL);
    struct tm tm= *localtime(&t);
    sprintf(buff,"now: %d-%02d-%02d %02d:%02d:%02d\0",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    return buff;
}

void save_game(node* head_node,cell* head_cell1,cell*head_cell2,int** ma_p,int size)
{
    FILE* fp;
    if (head_cell2==NULL)
        fp=fopen("single_player_save.bin","w+b");//append beshe
    else
        fp=fopen("multi_player_save.bin","w+b");//append beshe
    node* temp;
    point pt;
    char date[25],t_name[8];

    int s_count,i,j,temp_s,dum,c_count,temp_e;
    strcpy(date,now());
    fwrite(date,sizeof(char),25,fp);        //inja mishe esme map moteghayer bashe
    //fwrite("map.bin",sizeof(char),8,fp);
    s_count=source_count(ma_p,size);
    fwrite(&s_count,sizeof(int),1,fp);
    for (i=0;i<size;i++){
        for(j=0;j<size;j++){
            if (ma_p[i][j]==1)
            {
                dum=size-i-1;
                fwrite(&j,sizeof(int),1,fp);
                fwrite(&dum,sizeof(int),1,fp);
                pt.x=j;
                pt.y=dum;
                temp=find_node_sp(pt,head_node);
                temp_s=temp->source;
                fwrite(&temp_s,sizeof(int),1,fp);
            }
        }
    }
    c_count=cell_count(head_cell1);
    fwrite(&c_count,sizeof(int),1,fp);
    while(head_cell1!=NULL)
    {
        pt=(head_cell1->location)->position;
        temp_e=head_cell1->energy;
        //t_name=(char*)calloc(8,sizeof(char));
        //t_name=head_cell1->name;
        i=pt.y;
        j=pt.x;
        fwrite(&j,sizeof(int),1,fp);
        fwrite(&i,sizeof(int),1,fp);
        fwrite(&temp_e,sizeof(int),1,fp);
        strcpy(t_name,head_cell1->name);
        fwrite(t_name,sizeof(char),8,fp);
        head_cell1=head_cell1->next;
    }
    if (head_cell2==NULL)
    {
        fclose(fp);
        return ;
    }
    c_count=cell_count(head_cell2);
    fwrite(&c_count,sizeof(int),1,fp);
    while(head_cell2!=NULL)
    {
        pt=(head_cell2->location)->position;
        temp_e=head_cell2->energy;
        //t_name=(char*)calloc(8,sizeof(char));
        //t_name=head_cell1->name;
        i=pt.y;
        j=pt.x;
        fwrite(&j,sizeof(int),1,fp);
        fwrite(&i,sizeof(int),1,fp);
        fwrite(&temp_e,sizeof(int),1,fp);
        strcpy(t_name,head_cell2->name);
        fwrite(t_name,sizeof(char),8,fp);
        head_cell2=head_cell2->next;
    }
    fclose(fp);
}

void load_game (node** head_node,cell** head_cell1,cell** head_cell2,int ***ma_p,int* size,int som)
{
    //inja bayad onvane save haro baraye karbar biare
    //inja bayad tori she ke bege chandomin save ro biare
    FILE* sp;
    if (som==1)
        sp=fopen("single_player_save.bin","r+b");
    else
        sp=fopen("multi_player_save.bin","r+b");
    if (sp==NULL){
        printf("couldn't open saved files\n");
        exit(-1);
    }

    char date[25],*map_name,*cell_name;
    int c_size,s_count,i,j,temp_s,c_count,temp_e;
    point pt;
    node* temp;
    fread(date,sizeof(char),25,sp);
    FILE* fp=fopen("map.bin","r+b");         //map mitoone moteghayer bashe
    if (fp==NULL){
        exit(-1);
    }

    fread(size,sizeof(int),1,fp);
    *ma_p=read_file(fp);
    fclose(fp);
    *head_node=make_network(*ma_p,*size);
    fread(&s_count,sizeof(int),1,sp);
    while (s_count>0)
    {
        fread(&j,sizeof(int),1,sp);
        fread(&i,sizeof(int),1,sp);
        pt.x=j;
        pt.y=i;
        fread(&temp_s,sizeof(int),1,sp);
        temp=find_node(pt,*head_node);
        temp->source=temp_s;
        s_count--;
    }
    fread(&c_count,sizeof(int),1,sp);
    cell* ch;
    ch=(cell*)malloc(sizeof(cell));
    ch->name=(char*)malloc(8*sizeof(char));
    fread(&j,sizeof(int),1,sp);
    fread(&i,sizeof(int),1,sp);
    pt.x=j;
    pt.y=i;             //chon ke moghe ye zakhire in daghighan location e cell e na jash rooye map
    fread(&temp_e,sizeof(int),1,sp);
    cell_name=(char*)calloc(8,sizeof(char));
    fread(cell_name,sizeof(char),8,sp);
    ch->energy=temp_e;
    ch->name=cell_name;
    ch->location=find_node(pt,*head_node);
    ch->next=NULL;
    c_count--;
    cell* cc=ch;
    while(c_count>0)
    {
        cell* nc;
        nc=(cell*)malloc(sizeof(cell));
        nc->name=(char*)malloc(8*sizeof(char));
        fread(&j,sizeof(int),1,sp);
        fread(&i,sizeof(int),1,sp);
        pt.x=j;
        pt.y=i;
        fread(&temp_e,sizeof(int),1,sp);
        cell_name=(char*)calloc(8,sizeof(char));
        fread(cell_name,sizeof(char),8,sp);
        nc->name=cell_name;
        nc->energy=temp_e;
        nc->location=find_node(pt,*head_node);
        nc->next=NULL;
        cc->next=nc;
        cc=cc->next;
        c_count--;
    }
    *head_cell1=ch;
    if (som==1)
        return ;
    fread(&c_count,sizeof(int),1,sp);
    ch=(cell*)calloc(1,sizeof(cell));
    ch->name=(char*)calloc(8,sizeof(char));
    fread(&j,sizeof(int),1,sp);
    fread(&i,sizeof(int),1,sp);
    pt.x=j;
    pt.y=i;             //chon ke moghe ye zakhire in daghighan location e cell e na jash rooye map
    fread(&temp_e,sizeof(int),1,sp);
    cell_name=(char*)calloc(8,sizeof(char));
    fread(cell_name,sizeof(char),8,sp);
    ch->energy=temp_e;
    ch->name=cell_name;
    ch->location=find_node(pt,*head_node);
    ch->next=NULL;
    c_count--;
    cc=ch;
    while(c_count>0)
    {
        cell* nc;
        nc=(cell*)malloc(sizeof(cell));
        nc->name=(char*)malloc(8*sizeof(char));
        fread(&j,sizeof(int),1,sp);
        fread(&i,sizeof(int),1,sp);
        pt.x=j;
        pt.y=i;
        fread(&temp_e,sizeof(int),1,sp);
        cell_name=(char*)calloc(8,sizeof(char));
        fread(cell_name,sizeof(char),8,sp);
        nc->name=cell_name;
        nc->energy=temp_e;
        nc->location=find_node(pt,*head_node);
        nc->next=NULL;
        cc->next=nc;
        cc=cc->next;
        c_count--;
    }
    *head_cell2=ch;

}
