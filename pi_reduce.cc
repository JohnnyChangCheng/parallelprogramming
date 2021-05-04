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
  long long int reducedcount = 0;
  // ---

  // TODO: MPI init
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  unsigned int seed = time(NULL)+world_rank;
  // TODO: use MPI_Reduce
  for (long long int i = 0; i < tosses/world_size; ++i) {

    x = (double)rand_r(&seed) / RAND_MAX;
    y = (double)rand_r(&seed) / RAND_MAX;
    z = (x * x) + (y * y);

    if (z <= 1) {
      ++count;
    }
  }

  MPI_Reduce(&count, &reducedcount, 1, MPI_LONG_LONG_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (world_rank == 0) {
    // TODO: PI result
    pi_result = ((double)(reducedcount*4) / (double)(tosses));
    // --- DON'T TOUCH ---
    double end_time = MPI_Wtime();
    printf("%lf\n", pi_result);
    printf("MPI running time: %lf Seconds\n", end_time - start_time);
    // ---
  }

  MPI_Finalize();
  return 0;
}
