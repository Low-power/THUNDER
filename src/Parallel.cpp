/*******************************************************************************
 * Author: Hongkun Yu, Mingxu Hu
 * Dependecy:
 * Test:
 * Execution:
 * Description:
 * ****************************************************************************/

#include "Parallel.h"

#include <exception>

Parallel::Parallel() {}

Parallel::~Parallel() {}

void Parallel::setMPIEnv()
{
    MPI_Group wGroup, aGroup, bGroup;

    MPI_Comm A, B;
    
    MPI_Comm_size(MPI_COMM_WORLD, &_commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &_commRank);

    int sizeA = _commSize / 2;
    int sizeB = _commSize - 1 - sizeA;

    int* a = new int[sizeA];
    int* b = new int[sizeB];

    for (int i = 0; i < sizeA; i++) a[i] = 2 * i + 1;
    for (int i = 0; i < sizeB; i++) b[i] = 2 * i + 2;

    MPI_Comm_group(MPI_COMM_WORLD, &wGroup);

    MPI_Group_incl(wGroup, sizeA, a, &aGroup);
    MPI_Group_incl(wGroup, sizeB, b, &bGroup);

    /* It should be noticed that the parameters of MPI_Comm_create should be
     * exactly the same in all process. Otherwise, an error will happen. */
    MPI_Comm_create(MPI_COMM_WORLD, aGroup, &A);
    MPI_Comm_create(MPI_COMM_WORLD, bGroup, &B);

    if (A != MPI_COMM_NULL) { _hemi = A; };
    if (B != MPI_COMM_NULL) { _hemi = B; };

    MPI_Group_free(&wGroup);
    MPI_Group_free(&aGroup);
    MPI_Group_free(&bGroup);
    
    delete[] a;
    delete[] b;
}

void Parallel::setMPIEnv(const int commSize,
                         const int commRank,
                         const MPI_Comm& hemi)
{
    setCommSize(commSize);
    setCommRank(commRank);
    setHemi(hemi);
}

bool Parallel::isMaster() const
{
    return (_commRank == MASTER_ID);
}

int Parallel::commSize() const
{
    return _commSize;
}

void Parallel::setCommSize(const int commSize)
{
    _commSize = commSize;
}

int Parallel::commRank() const
{
    return _commRank;
}

void Parallel::setCommRank(const int commRank)
{
    _commRank = commRank;
}

MPI_Comm Parallel::hemi() const
{
    return _hemi;
}

void Parallel::setHemi(const MPI_Comm& hemi)
{
    _hemi = hemi;
}

void display(const Parallel& parallel)
{
    if (parallel.isMaster())
        printf("Master Process\n");
    else
        printf("Process %4d of %4d Processes\n",
               parallel.commRank(),
               parallel.commSize());
}

void MPI_Recv_Large(void* buf,
                    size_t count,
                    MPI_Datatype datatype,
                    int source,
                    int tag,
                    MPI_Comm comm)
{
    MPI_Status status;

    int nBlock = (count - 1) / INT_MAX + 1;

    if (nBlock != 1)
        CLOG(INFO, "LOGGER_SYS") << "MPI_Recv_Large: Transmitting "
                                 << nBlock
                                 << " Block(s).";
    
    int type_size;
    if (MPI_Type_size(datatype, &type_size) != MPI_SUCCESS)
        std::terminate();

    for (int i = 0; i < nBlock; i++)
    {
        int blockSizeCheck;
        int blockSize = (i != nBlock - 1)
                      ? INT_MAX
                      : count - INT_MAX * (nBlock - 1);

        MPI_Recv(static_cast<char*>(buf) + type_size * i * INT_MAX,
                 blockSize,
                 datatype,
                 source,
                 tag * nBlock + i,
                 comm,
                 &status);

        // check the integrity
        MPI_Get_count(&status, datatype, &blockSizeCheck);

        if (blockSizeCheck != blockSize)
            CLOG(FATAL, "LOGGER_SYS") << "MPI_Recv_Large: Incomplete Transmission";
    }
}

void MPI_Ssend_Large(const void* buf,
                     size_t count,
                     MPI_Datatype datatype,
                     int dest,
                     int tag,
                     MPI_Comm comm)
{
    int nBlock = (count - 1) / INT_MAX + 1;

    if (nBlock != 1)
        CLOG(INFO, "LOGGER_SYS") << "MPI_Ssend_Large: Transmitting "
                                 << nBlock
                                 << " Block(s).";
    
    int type_size;
    if (MPI_Type_size(datatype, &type_size) != MPI_SUCCESS)
        std::terminate();

    for (int i = 0; i < nBlock; i++)
    {
        int blockSize = (i != nBlock - 1)
                      ? INT_MAX
                      : count - INT_MAX * (nBlock - 1);

        MPI_Ssend(static_cast<const char*>(buf) + type_size * i * INT_MAX,
                  blockSize,
                  datatype,
                  dest,
                  tag * nBlock + i,
                  comm);
    }
}

void MPI_Bcast_Large(void* buf,
                     size_t count,
                     MPI_Datatype datatype,
                     int root,
                     MPI_Comm comm)
{
    int nBlock = (count - 1) / INT_MAX + 1;

    if (nBlock != 1)
        CLOG(INFO, "LOGGER_SYS") << "MPI_Bcast_Large: Transmitting "
                                 << nBlock
                                 << " Block(s).";

    int type_size;
    if (MPI_Type_size(datatype, &type_size) != MPI_SUCCESS)
        std::terminate();
    
    for (int i = 0; i < nBlock; i++)
    {
        int blockSize = (i != nBlock - 1)
                      ? INT_MAX
                      : count - INT_MAX * (nBlock - 1);

        MPI_Bcast(static_cast<char*>(buf) + type_size * i * INT_MAX,
                  blockSize,
                  datatype,
                  root,
                  comm);
    }
}

void MPI_Allreduce_Large(const void* sendbuf,
                         void* recvbuf,
                         size_t count,
                         MPI_Datatype datatype,
                         MPI_Op op,
                         MPI_Comm comm)
{
    int nBlock = (count - 1) / INT_MAX + 1;

    if (nBlock != 1)
        CLOG(INFO, "LOGGER_SYS") << "MPI_Allreduce_Large: Transmitting "
                                 << nBlock
                                 << " Block(s).";

    int type_size;
    if (MPI_Type_size(datatype, &type_size) != MPI_SUCCESS)
        std::terminate();
    
    for (int i = 0; i < nBlock; i++)
    {
        int blockSize = (i != nBlock - 1)
                      ? INT_MAX
                      : count - INT_MAX * (nBlock - 1);

        MPI_Allreduce(static_cast<const char*>(sendbuf) + type_size * i * INT_MAX,
                      static_cast<char*>(recvbuf) + type_size * i * INT_MAX,
                      blockSize,
                      datatype,
                      op,
                      comm);
    }
}
