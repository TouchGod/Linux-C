#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
    if (argc!=2){
        fprintf(stderr,"Error:false file\n");
    }
    FILE *src=fopen(argv[1],"rb");
    if(!src){
        fprintf(stderr,"open %s failed!",argv[1]);
    }

    fseek(src,0,SEEK_END);
    int fsize=ftell(src);
    rewind(src);

    char* buffer = (char*)malloc(fsize+1);
    int bread=fread(buffer,sizeof(char),fsize,src);
    if (bread<fsize){
        fprintf(stderr,"read failed\n");
    }
    buffer[bread]='\0';

    char c=buffer[0];
    int blank=0;
    int num=0;
    int chars=0;
    int no=0;
    int line=0;
    while(c != '\0'){
        switch(c){
        case '\t':
        case '\r':no++;break;
        case ' ':blank++;num++;break;
        case '\n':line++;no++;break;
        default:chars++;break;
        }
        buffer++;
        c=*buffer;
    }
    printf("字符个数: %d\n",chars);
    printf("空白字符: %d\n",blank);
    printf("单词个数: %d\n",num);
    printf("不可打印字符: %d\n",no);
    printf("行数: %d",line);
    return 0;
}

