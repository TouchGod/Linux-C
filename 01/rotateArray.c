#include <stdio.h>
void lotateLeft(int arr[],int n ,int k){
    for(int i=0;i<k;i++){
        int t=arr[0];
        for(int j=1;j<n;j++){
            arr[j-1]=arr[j];
        }
        arr[n-1]=t;
    }
}
int main()
{
    int arr[]={0,1,2,3,4,5,6,7,8};
    int k;
    scanf("%d",&k);
    lotateLeft(arr,9,k);
    for(int i=0;i<9;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}

