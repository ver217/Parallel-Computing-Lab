#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

static double sqrt5;
static double inverse;
static double one;
static double two;
static long* nums;

static long buf[2];

#define fabn(n) \
    ((long)(inverse * (pow(one, n) - pow(two, n))))


/***** Begin *****/
int main(int argc, char* argv[]) {
    int numprocs, myid;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if (myid == 0) { // 0号进程总控
        int n;
        scanf("%d", &n);
        if (n == 1) {
            printf("1\n");
            buf[0] = 0;
            buf[1] = -1;
            for (int dest = 1; dest < numprocs; dest++)
                MPI_Send(buf, 2, MPI_LONG, dest, 99, MPI_COMM_WORLD);
        } else if (n == 2) {
            buf[0] = 0;
            buf[1] = -1;
            for (int dest = 1; dest < numprocs; dest++)
                MPI_Send(buf, 2, MPI_LONG, dest, 99, MPI_COMM_WORLD);
            printf("1 1\n");
        } else {
            printf("1 1");
            n -= 2;
            nums = (long*)malloc(sizeof(long) * n);
            int sub_procs = numprocs - 1;
            int start = 0;
            int n_per_proc = n % sub_procs == 0 ? n / sub_procs : n / sub_procs + 1;
            int end = n_per_proc - 1;
            for (int dest = 1; dest < numprocs; dest++) {
                buf[0] = start;
                buf[1] = end;
                MPI_Send(buf, 2, MPI_LONG, dest, 99, MPI_COMM_WORLD);
                start = end + 1;
                end += n_per_proc;
                if (end >= n)
                    end = n - 1;
            }
            int recv_cnt = 0;
            while (recv_cnt < n) {
                for (int src = 1; src < numprocs; src++) {
                    MPI_Recv(buf, 2, MPI_LONG, src, 99, MPI_COMM_WORLD, &status);
                    nums[buf[0]] = buf[1];
                    recv_cnt++;
                    if (recv_cnt >= n)
                        break;
                }
            }
            for (int i = 0; i < n; i++)
                printf(" %ld", nums[i]);
            printf("\n");
        }
    } else { // 其他进程计算
        MPI_Recv(buf, 2, MPI_LONG, 0, 99, MPI_COMM_WORLD, &status);
        int start = buf[0], end = buf[1];
        if (start <= end) {
            sqrt5 = sqrt(5.0);
            inverse = 1 / sqrt5;
            one = (1 + sqrt5) / 2;
            two = (1 - sqrt5) / 2;
            for (int i = start; i <= end; i++) {
                buf[0] = i;
                buf[1] = fabn(i + 3);
                MPI_Send(buf, 2, MPI_LONG, 0, 99, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Finalize();
    return 0;
}
/***** End *****/
