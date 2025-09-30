#include <stdio.h>
#include <stdbool.h>
#include <math.h>

bool isPrime(int n){
    if (n<=1){
        return false;
    }
    for(int i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}
int main()
{
    int a;
    scanf("%d",&a);
    if(isPrime(a)){
        printf("%d is a prime.\n",a);
    }else{
        printf("%d is not a prime.\n",a);
    }
    return 0;
}

