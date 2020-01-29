#include <stdio.h>
#include <stdlib.h>         //map editor

typedef struct{
    int x,y;
}point;

char* tomapsfolder(char* src)
{
    char* temp="maps\\";
    char* res;
    res=(char*)malloc((strlen(src)+strlen(temp)+10)*sizeof(char));
    strcpy(res,temp);
    strcat(res,src);
    return res;
}

char* tosavesfolder(char* src)
{
    char* temp="saves\\";
    char* res;
    res=(char*)malloc((strlen(src)+strlen(temp)+10)*sizeof(char));
    strcpy(res,temp);
    strcat(res,src);
    return res;
}

char* toraw (char* src)
{
    char temp[20];
    char* res;
    res=(char*)malloc((strlen(src)-2)*sizeof(char));
    strncpy(temp,src,strlen(src)-4);
    temp[strlen(src)-4]='\0';
    strcpy(res,temp);
    return res;
}
char* tobin (char* src)
{
    char* res;
    res=(char*)malloc((strlen(res)+4)*sizeof(char));
    strcpy(res,src);
    strcat(res,".bin");
    return res;
}
char* totxt (char* src)
{
    char* res;
    res=(char*)malloc((strlen(res)+4)*sizeof(char));
    strcpy(res,src);
    strcat(res,".txt");
    return res;
}

void show_map (int **arr,int n)
{
    char block_name [][20]={"Energy","Mitosis","Forbidden","Normal"};
    int i,j;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            printf("%10s ",block_name[arr[i][j]-1]);
        }
        printf("\n");
    }
}

int** read_file (FILE* fporig)
{
    FILE* fp=fporig;
    fseek(fp,0,SEEK_SET);
    char c;
    int n,i;
    fread(&n,sizeof(int),1,fp);
    int **arr;
    arr=(int**)malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
        arr[i]=(int*)malloc(n*sizeof(int));
    for (i=0;i<n*n;i++)
    {
        fread(&c,sizeof(char),1,fp);
        arr[i/n][i%n]=c-'0';
    }
    return arr;
}

void write_file(FILE* fp,int ** arr,int n)
{
    fwrite(&n,sizeof(int),1,fp);
    int i,j;
    char temp;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            temp=arr[i][j]+48;
            fwrite(&temp,sizeof(char),1,fp);
        }
    }
}

FILE* map_opener(char** map_name) //return of map name
{
    int i=1,n;
    FILE* fyle=fopen("maps.txt","r+"),*fp;
    char mapname[20];
    printf("which one of the following maps would you want ? (.bin)\n");
    while(1)
    {
        fscanf(fyle,"%s",mapname);
        printf("%d) %s\n",i,toraw(mapname));
        i++;
        if (feof(fyle))
            break;
    }
    printf("%d) select by name \n",i);
    scanf("%d",&n);
    if (n>i){
        printf("out of range\n");
        *map_name=NULL;
        return NULL;
    }
    if (n==i) //new name need
    {
        fseek(fyle,0,SEEK_END);
        printf("map name ? (.bin) \n");
        scanf("%s",*map_name);
        fprintf(fyle,"\n%s",tobin(*map_name));
        fp=fopen(tomapsfolder(tobin(*map_name)),"rb");
    }
    else{
        fseek(fyle,0,SEEK_SET);
        for (i=0;i<n;i++)
        {
            fscanf(fyle,"%s",mapname);
        }
        printf("%s it is\n",mapname);
        strcpy(*map_name,mapname);
        fp=fopen(tomapsfolder(mapname),"rb");
    }
    if (fp==NULL)
        printf("couldn't be opened!\n");
    return fp;

}

void prnt_menu()
{
    printf("[1]New map\n[2]Edit map\n[3]Save as text\n[4]Exit\n");
}

void map_editor(void)
{
    int act;
    do
    {
        prnt_menu();
        scanf ("%d",&act);
        if (act==1)
        {
            int n,i=0;
            char *fn,c,*temp,*temp2;
            fn=(char*)malloc(20*sizeof(char));
            printf("File name ? (max 20 chars) (.bin) : ");
            scanf("%s",fn);
            temp=tobin(fn);
            temp2=tomapsfolder(temp);
            FILE *fp=fopen(temp2,"wb");
            printf("Number of R&C : ");
            scanf("%d",&n);
            getchar();
            fwrite(&n,sizeof(int),1,fp);
            do
            {
                scanf("%c",&c);
                if (c>='0' && c<='4'){
                    i++;
                    fwrite(&c,sizeof(char),1,fp);
                }
            }while(i<n*n);
            fclose(fp);
        }
        else if (act==2) // uses opener
        {
            point p;
            int n,flag=1,**arr,temp;
            char *fn;
            fn=(char*)malloc(20*sizeof(char));
            FILE *fp;
            fp=map_opener(&fn);
            if (fp==NULL){
                printf("Cannot open file\n");
                exit(-1);
            }
            fread(&n,sizeof(int),1,fp);
            arr=read_file(fp);
            fclose(fp);
            show_map(arr,n);
            do{
                printf("which position of x y ? ");
                scanf("%d %d",&p.x,&p.y);
                while(p.x<0 || p.x>=n||p.y<0||p.y>=n)
                {
                    printf("wrong address\n");
                    printf("which position of x y ? ");
                    scanf("%d %d",&p.x,&p.y);
                }
                printf("change to [1]Energy\t[2]Mitosis\t[3]Forbidden\t[4]Normal : ");
                getchar();
                scanf("%d",&temp);
                arr[n-p.y-1][p.x]=temp;
                show_map(arr,n);
                printf("Enter 1 to continue editing or 0 to leave and save map ");
                scanf("%d",&flag);
            }while(flag);
            fp=fopen(tomapsfolder(fn),"wb");
            if (fp==NULL){
                printf("Can't open file\n");
                exit(-1);
            }
            write_file(fp,arr,n);
            fclose(fp);

        }
        else if (act==3) //uses opener
        {
            char block_name [][20]={"Energy","Mitosis","Forbidden","Normal"},bn[20];
            char* temp,c,*mname;
            temp=(char*)malloc(20*sizeof(char));
            mname=(char*)malloc(50*sizeof(char));
            FILE* mp,*tp;
            mp=map_opener(&temp);
            //strncpy(mname,temp,strlen(temp)-4);
            //strcat(mname,".txt");
            mname=toraw(temp);
            mname=totxt(mname);
            mname=tomapsfolder(mname);
            tp=fopen(mname,"w+");
            int n,i,j,tempint;
            fseek(mp,0,SEEK_SET);
            fread(&n,sizeof(int),1,mp);
            for (i=0;i<n;i++)
            {
                for (j=0;j<n;j++)
                {
                    fread(&c,sizeof(char),1,mp);
                    tempint=c-'1';
                    strcpy(bn,block_name[tempint]);
                    fprintf(tp,"%15s  ",bn);
                }
                fprintf(tp,"\n");
            }
            fclose(mp);
            fclose(tp);
            printf("Saved successfully\n");
        }
        else if( act!=4)
            printf("Invalid num ! enter again \n");
    } while (act!=4);
}

int source_count(int **ma_p,int size)
{
    int i,j,res=0;
    for (i=0;i<size;i++){
        for (j=0;j<size;j++){
            if (ma_p[i][j]==1)
                res++;
        }
    }
    return res;
}
