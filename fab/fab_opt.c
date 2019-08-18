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
static int N;
static int rest;

static int n_thread;
static int n_per_proc;

#define fabn(n) \
    ((long)(inverse * (pow(one, n) - pow(two, n))))

void* thread_handler(void* n_ptr) {
    int n = *((int *)n_ptr);
    int end = n + n_per_proc;
    if (n == 0)
        end += rest;
    nums[n] = fabn(n + 3);
    if (end - n > 1) {
        nums[n + 1] = fabn(n + 4);
        for (int i = n + 2; i < end; i++)
            nums[i] = nums[i - 1] + nums[i - 2];
    }
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
        n -= 2;
        N = n;
        n_thread = 4;
        n_per_proc = n / n_thread;
        rest = n - n_per_proc * n_thread;
        sqrt5 = sqrt(5.0);
        inverse = 1 / sqrt5;
        one = (1 + sqrt5) / 2;
        two = (1 - sqrt5) / 2;
        nums = (long*)malloc(sizeof(long) * n);
        for (int i = 0; i < n; i++)
            nums[i] = i;
        pthread_t* pid_arr = (pthread_t*)malloc(sizeof(pthread_t) * n);
        int thread_cnt = 0;
        for (int i = 0; i < n; i++, thread_cnt++) {
            if (i * n_per_proc + rest >= n)
                break;
            pthread_t t;
            if (i == 0)
                pthread_create(&t, NULL, thread_handler, (void*)(nums));
            else
                pthread_create(&t, NULL, thread_handler, (void*)(nums + i * n_per_proc + rest));
            pid_arr[i] = t;
        }
        for (int i = 0; i < thread_cnt; i++)
            pthread_join(pid_arr[i], NULL);
        for (int i = 0; i < n; i++)
            printf(" %ld", nums[i]);
        printf("\n");
    }
    printf("time: %lf\n", ((double)clock() - start) / CLOCKS_PER_SEC * 1000);
    return 0;
}
/***** End *****/
