#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // --- DON'T TOUCH ---
  MPI_Init(&argc, &argv);
  double start_time = MPI_Wtime();
  double pi_result;
  long long int tosses = atoi(argv[1]);
  int world_rank, world_size;
  double x, y, z;
  long long int count = 0;


  // ---

  // TODO: MPI init
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  unsigned int seed = time(NULL)+world_rank;
  long long int recv_cnt[world_size];
  // TODO: use MPI_Reduce
  for (long long int i = 0; i < tosses/world_size; ++i) {

    x = (double)rand_r(&seed) / RAND_MAX;
    y = (double)rand_r(&seed) / RAND_MAX;
    z = (x * x) + (y * y);

    if (z <= 1) {
      ++count;
    }
  }

  MPI_Gather(&count, 1, MPI_LONG_LONG_INT, recv_cnt, 1, MPI_LONG_LONG_INT, 0,
             MPI_COMM_WORLD);
  if (world_rank == 0) {
    long long int counter_sum = 0;
    // TODO: PI result
    for (int i = 0; i < world_size; ++i) {
      counter_sum += recv_cnt[i];
    }
    pi_result = ((double)(counter_sum*4) / (double)(tosses));
    // --- DON'T TOUCH ---
    double end_time = MPI_Wtime();
    printf("%lf\n", pi_result);
    printf("MPI running time: %lf Seconds\n", end_time - start_time);
    // ---
  }

  MPI_Finalize();
  return 0;
}
