/*******************************************************************************
 * Author: Mingxu Hu
 * Dependecy:
 * Test:
 * Execution:
 * Description:
 * ****************************************************************************/

#include <iostream>

#include "Projector.h"
#include "Reconstructor.h"
#include "FFT.h"

#define N 256
#define M 8

int main(int argc, char* argv[])
{

    int messageid = 2;
    int numprocs, myid, server, serverid[1];
    int numworkers, workerid;
    
    MPI_Comm world, workers;
    MPI_Group world_group, worker_group;

    MPI_Init(&argc, &argv);
    world = MPI_COMM_WORLD;
    MPI_Comm_size(world, &numprocs);
    MPI_Comm_rank(world, &myid);
    
   // server = 0;
   // MPI_Comm_group(world, &world_group);
   // serverid[0] = server;
   // MPI_Group_excl(world_group, 1, serverid, &worker_group);
   // MPI_Comm_create(world, worker_group, &workers);
   // 
   // MPI_Comm_rank(workers, &workerid);
   // MPI_Comm_size(workers, & numworkers);
    
    if (myid == messageid) {
        std::cout << "numprocs: " << numprocs << std::endl
                  << "myid: " << myid << std::endl
                  << "server is node " << server << std::endl
                  << "numworkers: " << numworkers << std::endl
                  << "workerid: " << workerid << std::endl;
    }



    Volume head(N, N, N, realSpace);
    if (myid == messageid) {
        std::cout << "Define a head" << std::endl;
    }
    for (int z = 0; z < N; z++)
        for (int y = 0; y < N; y++)
            for (int x = 0; x < N; x++)
            {
                if ((pow(x - N / 2, 2)
                   + pow(y - N / 2, 2)
                   + pow(z - N / 2, 2) < pow(N / 8, 2)) ||
                    (pow(x - N / 2, 2)
                   + pow(y - 3 * N / 8, 2)
                   + pow(z - 5 * N / 8, 2) < pow(N / 16, 2)) ||
                    (pow(x - N / 2, 2)
                   + pow(y - 5 * N / 8, 2) 
                   + pow(z - 5 * N / 8, 2) < pow(N / 16, 2)) ||
                    ((pow(x - N / 2, 2)
                    + pow(y - N / 2, 2) < pow(N / 16, 2)) &&
                     (z < 7 * N / 16) && (z > 5 * N / 16)))
                    head.setRL(1, x, y, z);
                else
                    head.setRL(0, x, y, z);
            }
    
    FFT fft;
    fft.fw(head);

    /***
    VOLUME_FOR_EACH_PIXEL_FT(head)
        printf("%f %f\n", REAL(head.getFT(i, j, k)),
                          IMAG(head.getFT(i, j, k)));
    ***/

    Projector projector;
    projector.setProjectee(head);

    char name[256];
    int counter = 0;

    Image image(N, N, fourierSpace);
    // Image image(N, N, realSpace);
    
  //  Reconstructor reconstructor;

    //try {
        for (int k = M / numworkers * workerid; k < M / numworkers * (workerid + 1); k++)
            for (int j = 0; j < M; j++)
                for (int i = 0; i < M; i++)
                {
                    printf("%02d %02d %02d\n", i, j, k);
                    sprintf(name, "%02d%02d%02d.bmp", i, j, k);
                    Coordinate5D coord(2 * M_PI * i / M,
                                       M_PI * j / M,
                                       2 * M_PI * k / M,
                                       0,
                                       0);
                    projector.project(image, coord);
                    //reconstructor.insert(image, coord);
                    /***
                    FFT fft;
                    fft.fw(image);
                    fft.bw(image);
                    ***/
                    /***
                    R2R_FT(image, sin(2));
                    ***/
                    /***
                    R2R_FT(image, image, projector.project(image,
                                                    2 * M_PI * i / M,
                                                    M_PI * j / M,
                                                    2 * M_PI * k / M));
                                                    ***/

                    // image.saveRLToBMP(name);
                    // image.saveFTToBMP(name, 0.1);
                
            }
    //} catch (Error& err) {
    //   std::cout << err;
    //}

    MPI_Finalize();
    return 0;
}
