#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    long long int counter = 0;

    // TODO: init MPI
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    srand(time(0) + world_rank);
    for (long long int i = 0; i < tosses/world_size; ++i)
    {
        double x = ((double)rand()) / RAND_MAX;
        double y = ((double)rand()) / RAND_MAX;
        double z = sqrt(x * x + y * y);
        if (z <= 1)
        {
            counter++;
        }
    }
    // Calculate pow of 2
    int pow_of_2 = (int)(log(world_size) / log(2));

    for (int i = 0; i < pow_of_2; ++i)
    {
        int last_idx = -1;
        for (int j = 0; j < world_size; j += pow(2, i))
        {
            if (last_idx == -1)
            {
                last_idx = j;
            }
            else
            {
                if (last_idx == world_rank)
                {
                    long long int buffer = 0;
                    MPI_Recv(&buffer, 1, MPI_LONG_LONG_INT,
                             j, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    
                    counter += buffer;
                }
                if (j == world_rank)
                {
                    MPI_Send(&counter, 1, MPI_LONG_LONG_INT, last_idx, 1, MPI_COMM_WORLD);
                }
                last_idx = -1;
            }
        }
    }
    // TODO: binary tree redunction

    if (world_rank == 0)
    {
        // TODO: PI result
        pi_result = (double)counter / (double)(tosses) * 4.0;
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
