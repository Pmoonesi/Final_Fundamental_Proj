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

void save_game(node* head_node,cell* head_cell1,cell*head_cell2,int** ma_p,int size,char* map_name,int turn)
{
    char savename[50];
    printf("name of save ? : (.bin) ");
    scanf("%s",savename);
    FILE* fyle;
    if (head_cell2==NULL)
        fyle=fopen("single_player_saves.txt","a+");
    else
        fyle=fopen("multiplayer_saves.txt","a+");
    fprintf(fyle,"%s %s\n",tobin(savename),map_name);
    fclose(fyle);
    FILE* fp=fopen(tosavesfolder(tobin(savename)),"w+b");
    node* temp;
    point pt;
    char date[25],t_name[8];
    int s_count,i,j,temp_s,dum,c_count,temp_e;
    strcpy(date,now());
    fwrite(date,sizeof(char),25,fp);
    fwrite(&turn,sizeof(int),1,fp);
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

FILE* choosesave (int som,FILE** mp,char** map_name)
{
    int i=1,n;
    FILE* fyle;
    char loadname[50],mapname[20];
    if (som==1)
        fyle=fopen("single_player_saves.txt","r");
    else
        fyle=fopen("multiplayer_saves.txt","r");
    while(1)
    {
        if (feof(fyle))
            break;
        fscanf(fyle,"%s",loadname);
        fscanf(fyle,"%s",mapname);
        if (feof(fyle))
            break;
        printf("%d) %s\t%s\n",i,toraw(loadname),toraw(mapname));
        i++;

    }
    if (i==1)
    {
        printf("there is no saves in here !\n");
        *map_name=NULL;
        *mp=NULL;
        return NULL;
    }
    scanf("%d",&n);
    if (n>=i){
        printf("out of range\n");
        *map_name=NULL;
        *mp=NULL;
        return NULL;
    }
    fseek(fyle,0,SEEK_SET);
    for (i=0;i<n;i++)
    {
        fscanf(fyle,"%s",loadname);
        fscanf(fyle,"%s",mapname);
    }
    strcpy(*map_name,mapname);
    *mp=fopen(tomapsfolder(mapname),"r+b");
    if (*mp==NULL)
        printf("map could not be opened!\n");
    FILE* fp=fopen(tosavesfolder(loadname),"r+b");
    if (fp==NULL)
        printf("save couldn't be opened!\n");
    return fp;
}

void load_game (node** head_node,cell** head_cell1,cell** head_cell2,int ***ma_p,int* size,int som,char** map_name,int* turn)
{
    FILE* fp,*sp;
    sp=choosesave(som,&fp,map_name);

    if (sp==NULL)
        return ;

    char date[25],*cell_name;
    int c_size,s_count,i,j,temp_s,c_count,temp_e;
    point pt;
    node* temp;
    fread(date,sizeof(char),25,sp);
    if (fp==NULL){
        return ;
    }

    fread(size,sizeof(int),1,fp);
    *ma_p=read_file(fp);
    fclose(fp);
    *head_node=make_network(*ma_p,*size);
    fread(turn,sizeof(int),1,sp);
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
    pt.y=i;
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
    pt.y=i;
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
