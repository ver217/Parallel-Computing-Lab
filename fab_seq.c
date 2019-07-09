#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_fab(int n) {
    if (n <= 0)
        return;
    if (n == 1)
        printf("1\n");
    else if (n == 2)
        printf("1 1\n");
    else {
        printf("1 1");
        long* nums = (long*)malloc(sizeof(long) * n);
        nums[0] = 1;
        nums[1] = 1;
        for (int i = 2; i < n; i++) {
            nums[i] = nums[i - 1] + nums[i - 2];
            printf(" %ld", nums[i]);
        }
        printf("\n");
    }
}

/***** Begin *****/
int main() {
    int n;
    scanf("%d", &n);
    clock_t start = clock();
    print_fab(n);
    printf("%lf sec\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    return 0;
}
/***** End *****/