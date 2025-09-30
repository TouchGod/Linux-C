#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int sum(int *arr, int n);


int* getPrimes(int n);


bool isPrime(int x);

// Usage: a.out [n]
// 如果用户有传入第二个命令行参数，则生成用户指定个数的素数
int main(int argc, char **argv) {

    int n = 10;         //default to the first 10 primes
    if(argc == 2) {
        n=atoi(argv[1]);
    }
    int *primes = getPrimes(n);

    int s = sum(primes, n);
    printf("The sum of the first %d primes is %d\n", n, s);

    return 0;
}

int sum(int *arr, int n) {
    int i;
    int total=0;
    for(i=0; i<n; i++) {
        total += arr[i];
    }
    return total;
}

int* getPrimes(int n) {
    int* result = malloc(n*sizeof(int));
    int i = 0;
    int x = 2;
    while(i < n) {
        if(isPrime(x)) {
            result[i] = x;
            i++;
        }
        x++;
    }
    return result;
}

bool isPrime(int x) {
    if(x % 2 == 0 && x!=2) {
        return false;
    }
    for(int i=3; i<=sqrt(x); i+=2) {
        if(x % i == 0) {
            return false;
        }
    }
    return true;
}
