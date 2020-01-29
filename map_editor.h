#include <stdio.h>
#include <stdlib.h>         //map editor

typedef struct{
    int x,y;
}point;

void show_map (int **arr,int n)
{
    int i,j;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            printf("%d ",arr[i][j]);
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
        /*if (i%n==0)
            arr[i/n]=(int*)malloc(n*sizeof(int));*/
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

FILE* map_opener(char** map_name)
{
    int i=1,n;
    FILE* fyle=fopen("maps.txt","r");
    char mapname[20];
    printf("which one of the following maps would you want ? \n");
    while(1)
    {
        fscanf(fyle,"%s",mapname);
        printf("%d) %s\n",i,mapname);
        i++;
        if (feof(fyle))
            break;
    }
    fseek(fyle,0,SEEK_SET);
    scanf("%d",&n);
    if (n>i){
        printf("out of range\n");
        *map_name=NULL;
        return NULL;
    }
    for (i=0;i<n;i++)
    {
        fscanf(fyle,"%s",mapname);
    }
    printf("%s it is\n",mapname);
    strcpy(*map_name,mapname);
    FILE* fp=fopen(mapname,"rb");
    if (fp==NULL)
        printf("couldn't be opened!\n");

    return fp;

}

void prnt_menu()
{
    printf("1)new map \n2)edit map\n3)save as text\n4)exit\n");
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
            int n,i;
            char *fn,c;
            fn=(char*)malloc(20*sizeof(char));
            printf("File name ? (max 20 chars) : ");
            scanf("%s",fn);

            FILE *fp=fopen(fn,"wb");

            printf("Number of R&C : ");
            scanf("%d",&n);
            getchar();
            fwrite(&n,sizeof(int),1,fp);
            for (i=0;i<n*n;i++)
            {
                scanf("%c",&c);
                fwrite(&c,sizeof(char),1,fp);
            }
            fclose(fp);
        }
        else if (act==2)
        {
            point p;
            int n,flag=1,**arr,temp;
            char *fn;
            fn=(char*)malloc(20*sizeof(char));
            printf("File name ? (max 20 chars) : ");
            scanf("%s",fn);
            FILE *fp=fopen(fn,"rb");
            if (fp==NULL){
                printf("Cannot open file\n");
                exit(-1);
            }
            fread(&n,sizeof(int),1,fp);
            arr=read_file(fp);
            fclose(fp);
            show_map(arr,n);
            do{
                printf("which position of x,y ? "); //baraye mahdoodash shart bezar !
                scanf("%d %d",&p.x,&p.y);
                printf("change to 1.regular 2.blocked 3.divide 4.source : ");
                getchar();
                scanf("%d",&temp);
                arr[n-p.y-1][p.x]=temp;

                show_map(arr,n);
                printf("press 1 to continue editing or 0 to leave and save map ");
                scanf("%d",&flag);
            }while(flag);
            fp=fopen(fn,"wb");
            if (fp==NULL){
                printf("Can't open file\n");
                exit(-1);
            }
            write_file(fp,arr,n);
            fclose(fp);

        }
        else if (act==3)
        {
            char block_name [][20]={"Energy","Mitosis","Forbidden","Normal"},bn[20];
            char* temp,c,mname[20];
            temp=(char*)malloc(20*sizeof(char));
            //mname=(char*)malloc(20*sizeof(char));
            FILE* mp,*tp;
            mp=map_opener(&temp);
            printf("%s\n",temp);
            strncpy(mname,temp,strlen(temp)-4);
            printf("%s\n",mname);
            strcat(mname,".txt");
            printf("%s\n",mname);
            tp=fopen(mname,"w+");
            int n,i,j,tempint;
            printf("SHIT");
            fseek(mp,0,SEEK_SET);
            fread(&n,sizeof(int),1,mp);
            printf("n=%d\n",n);
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
        }
        else if( act!=4)
            printf("Invalid num ! enter again \n");
    } while (act!=4);     //mitooni ye show map ham bezari !
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

