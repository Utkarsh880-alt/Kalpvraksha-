#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);
    int matrix[n][n];
    int *p = &matrix[0][0];
    srand(time(0)); // it will give different values each time
    printf("\nOriginal Matrix:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            *(p+i*n+j) = rand() % 256;
            printf("%4d ", *(p+i*n+j));
        }
        printf("\n");
    }
    // logic for rotation
    int temp;
    for(int layer=0; layer<n/2; layer++){
        int first = layer;
        int last = n- layer-1;
        for(int i=first; i<last; i++){
            int offset = i-first;
            int *top =(p+first*n+i);
            int *left =(p +(last-offset)*n + first);
            int *bottom =(p + last*n +(last-offset));
            int *right  =(p + i*n + last);
            temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
    printf("\nMatrix After Rotation:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%4d ", *(p+i*n+j));
        }
        printf("\n");
    }
    //logic for smoothing 
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int sum= 0, count =0;
            for(int r=i-1; r<=i+1; r++){
                for(int c=j-1; c<=j+1; c++){
                    if (r>=0 && r<n && c>=0 && c<n) {
                        int neighbour = *(p + r*n + c);
                        neighbour &= 0xFFFF;
                        sum += neighbour;
                        count++;
                }
            }
        }
        int newValue = sum/count;
        int *cell = p + i*n + j;
        int orig = (*cell) & 0xFFFF;
        *cell = orig | (newValue << 16);
        }
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int packed = *(p+i*n+j);
            *(p+i*n+j) = (packed >> 16) & 0xFFFF;
        }
    }

    printf("\nMatrix After Smoothing:\n");
    for (int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%4d ", *(p+i*n+j));
        }
        printf("\n");
    }
    return 0;
}