#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef enum{
    OUTSIDE,
    INSIDE,
}State;

int main(int argc,char*argv[])
{
    if (argc!=2){
        fprintf(stderr,"Usage: WC filename\n");
        exit(1);
    }
    FILE*src=fopen(argv[1],"r");
    if(!src){
        fprintf(stderr,"Open %s failed!\n",argv[1]);
        exit(1);
    }

    int characters=0,words=0,lines=0;
    State state=OUTSIDE;

    int c;
    while((c=fgetc(src))!= EOF){
        characters++;
        if(isblank(c)){
            state=OUTSIDE;
        }else{
            if (state==OUTSIDE){
                words++;
            }
            state=INSIDE;
        }
        if (c=='\n'){
            lines++;
        }
    }

    printf("characters= %d,words= %d,lines= %d\n",characters,words,lines);
    fclose(src);

    return 0;
}

