#include <stdio.h>
#include <pthread.h>
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

void* thread_handler(void* n_ptr) {
    int n = *((int *)n_ptr);
    nums[n] = fabn(n + 3);
    return NULL;
}

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
        for (int i = 0; i < n - 2; i++)
            nums[i] = i;
        pthread_t* pid_arr = (pthread_t*)malloc(sizeof(pthread_t) * (n - 2));
        for (int i = 0; i < n - 2; i++) {
            pthread_t t;
            pthread_create(&t, NULL, thread_handler, (void*)(nums + i));
            pid_arr[i] = t;
        }
        for (int i = 0; i < n - 2; i++)
            pthread_join(pid_arr[i], NULL);
        for (int i = 0; i < n - 2; i++)
            printf(" %ld", nums[i]);
        printf("\n");
    }
    printf("time: %lf\n", ((double)clock() - start) / CLOCKS_PER_SEC * 1000);
    return 0;
}
/***** End *****/
