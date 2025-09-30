#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool isPrime(long long n){
    for(long long i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}

long long* generatePrimes(int n){
    long long*primes=malloc(n*sizeof(long long));
    primes[0]=2;
    int i=0;
    while(i<n){
        long long l=primes[i];
        while(!isPrime(l)){
            l++;
        }
        primes[i++]=l;
    }
    return primes;
}
int main()
{
    int n;
    scanf("%d",&n);
    long long* prime=generatePrimes(n);
    for(int i=0;i<n;i++){
        printf("%lld ",prime[i]);
    }
    return 0;
}

