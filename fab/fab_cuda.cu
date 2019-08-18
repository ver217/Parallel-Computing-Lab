#include <stdio.h>
#include <math.h>
#include <time.h>

__constant__ double params[4];

#define fabn(n) \
    ((long)(params[1] * (powf(params[2], n) - powf(params[3], n))))

__global__ static void fab(long *nums) {
    nums[threadIdx.x] = fabn((double)threadIdx.x + 3);
}

/***** Begin *****/
int main()
{
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
        double params_host[4] = {sqrt(5.0)};
        params_host[1] = 1 / params_host[0];
        params_host[2] = (1 + params_host[0]) / 2;
        params_host[3] = (1 - params_host[0]) / 2;
        cudaMemcpyToSymbol(params, params_host, sizeof(double) * 4);
        long* nums;
        cudaMalloc(&nums, sizeof(long) * n);
        fab<<<1, n>>>(nums);
        cudaDeviceSynchronize();
        long* nums_host = (long*)malloc(sizeof(long) * n);
        cudaMemcpy(nums_host, nums, sizeof(long) * n, cudaMemcpyDeviceToHost);
        cudaFree(nums);
        cudaFree(params);
        for (int i = 0; i < n; i++)
            printf(" %ld", nums_host[i]);
        printf("\n");
        free(nums_host);
    }
    printf("time: %lf\n", ((double)clock() - start) / CLOCKS_PER_SEC * 1000);
	return 0;
}
/***** End *****/