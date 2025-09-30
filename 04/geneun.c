#include <stdio.h>
#include <stdlib.h>

long long* generateUglyNumbers(int n){
    long long* ugly=malloc((n+1)*sizeof(long long));
    ugly[0]=1;

    int idx2=0,idx3=0,idx5=0;
    int i=1;
    while(i<n){
        long long x2=ugly[idx2]*2;
        long long x3=ugly[idx3]*3;
        long long x5=ugly[idx5]*5;

        long long min=x2;
        if(x3<min){
            min=x3;
        }
        if(x5<min){
            min=x5;
        }
        ugly[i++]=min;

        if(min==x2)idx2++;
        if(min==x3)idx3++;
        if(min==x5)idx5++;
    }
    return ugly;
}

int main(void){
    int n;
    scanf("%d",&n);
    long long* result=generateUglyNumbers(n);
    for(int i=0;i<n;i++){
        printf("%lld ",result[i]);
    }
    printf("\n");
    free(result);
    return 0;
}
