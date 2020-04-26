#include <stdlib.h>
#include <string.h>

int sort_ascending_order(int *r, int *rs)
{
    int x[9999];
    int xs = *rs;

    for (size_t i = 0; i < xs; i++)
    {
        x[i] = r[i];
    }

    for (size_t i = 0; i < xs; i++)
    {
        for (size_t j = 0; j < xs; j++)
        {
            if(x[i] < x[j]){
                int temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    for (size_t i = 0; i < xs; i++)
    {
        r[i] = x[i];
    }
    *rs = xs;
    return -117;
}

int remove_duplicates(int *r, int *rs){
    sort_ascending_order(r,rs);
     // Return, if array is empty 
    // or contains a single element 
    if (*rs==0 || *rs==1) 
        return *rs; 
  
    int temp[999]; 
    int n = *rs;
    // Start traversing elements 
    int j = 0; 
    for (int i=0; i<n-1; i++) 
  
        // If current element is not equal 
        // to next element then store that 
        // current element 
        if (r[i] != r[i+1]) 
            temp[j++] = r[i]; 
  
    // Store the last element as whether 
    // it is unique or repeated, it hasn't 
    // stored previously 
    temp[j++] = r[n-1]; 
  
    // Modify original array 
    for (int i=0; i<j; i++) 
        r[i] = temp[i]; 

    *rs = j;
  
    return j; 
}

int main(){
    int a[] = {0,0,0,0,04,564,89,6213,89,49,49,564,19,564,1694,94,919,1,1,4564,684,654,984,65419,8465,198,456,498465,498456,1496,495,189498,1496,8498,498,46,498,46,164,564,981,654,91,9654,94,979,4,6549,819,84,984,949,6465,49,41,98,496,491,651,984,654,984,1,624,9823,4,6574,61,654,651,6547,65,23498,4,16,49,1,198,456,1654,320,0,1,964,98,1654,91,6,4,0,546,949,156,4,8949,789,65,3416,49,165,4,9,9,9,416,498,1,4,654,98496,194,1,9746,14567,649,46,165,765,3798,65,1984,9165,4,64};
    int as = 26;
    remove_duplicates(a, &as);
    for (size_t i = 0; i < as; i++)
    {
        printf("%d-", a[i]);
    }
    int min = a[0];
    for(int i = 1; i<as; i++){
        if(a[i] < a[i-1]){
            printf("WOHHPE\n %d\n", i);
        }
    }
    
}