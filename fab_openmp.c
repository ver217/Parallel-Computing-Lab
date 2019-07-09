#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static double sqrt5;
static double inverse;
static double one;
static double two;
static long* nums;

#define fabn(n) \
    ((long)(inverse * (pow(one, n) - pow(two, n))))

/***** Begin *****/
int main() {
    int n;
    scanf("%d", &n);
    clock_t start = clock();
    if (n == 1)
        printf("1\n");
    else if (n == 2)
        printf("1 1\n");
    else {
        printf("1 1");
        sqrt5 = sqrt(5.0);
        inverse = 1 / sqrt5;
        one = (1 + sqrt5) / 2;
        two = (1 - sqrt5) / 2;
        nums = (long*)malloc(sizeof(long) * (n - 2));
        #pragma omp parallel for
        for (int i = 0; i < n - 2; i++)
            nums[i] = fabn(i + 3);
        for (int i = 0; i < n - 2; i++)
            printf(" %ld", nums[i]);
        printf("\n");
    }
    printf("%lf sec\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    return 0;
}
/***** End *****/
