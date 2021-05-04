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
  int iter = 0;
  long counter = 0;
  

  // ---

  // TODO: init MPI
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  unsigned int seed = time(NULL)+world_rank;

  long long int times = tosses/world_size;
  for (long long int i = 0; i < times; ++i) {
    double x = ((double)rand_r(&seed)) / RAND_MAX;
    double y = ((double)rand_r(&seed)) / RAND_MAX;
    double z = x * x + y * y;
    if (z <= 1) {
      counter++;
    }
  }

  if (world_rank > 0) {

    MPI_Send(&counter, 1, MPI_LONG_LONG_INT, 0, 1, MPI_COMM_WORLD);


  } else if (world_rank == 0) {
    long long int receiver_counter[world_size];
    long long int finalcounter = 0;
    // only for 0 processor
    finalcounter += counter;
    for (int i = 1; i < world_size; ++i) {
      MPI_Recv(&receiver_counter[i], 1, MPI_LONG_LONG_INT,
               i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      finalcounter += receiver_counter[i];
    }

    pi_result = ((finalcounter*4) / (double)(tosses));

    // --- DON'T TOUCH ---
    double end_time = MPI_Wtime();
    printf("%lf\n", pi_result);
    printf("MPI running time: %lf Seconds\n", end_time - start_time);
    // ---
  }

  MPI_Finalize();
  return 0;
}
