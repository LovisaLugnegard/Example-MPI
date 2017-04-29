/* File: Demo_vector.c
 * This is an example for MPI_Type_vector
 * Shows how to use MPI_Type_vector to send noncontiguous blocks of data
 * Author: Jing Liu @ TDB,LMB, Uppsala University
 * Contact: jing.liu@it.uu.se , jing.liu@icm.it.uu.se
 * Date: Jan, 2015, last update: Jan 2016
 *  output
         numx=5  extent=5 stride=1
         got 1 elements of type MY_TYPE
         which contained 5 elements of type MPI_DOUBLE
         0 0
         5 5
         10 10
         15 15
         20 20
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#define SIZE 64
int main(int argc,char *argv[])
{
  int myid, numprocs,sqnumprocs;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);


  double sendbuf[SIZE];//,recvbuf[SIZE/numprocs];
  int i,j,bonk2,count,stride,blocklength,N;
  MPI_Datatype MPI_LEFT_RITE;
  double *recvbuf;//,*sendbuf;
  recvbuf = malloc(sizeof(int)*SIZE);
  // sendbuf = malloc(sizeof(int)*SIZE);

  N=sqrt(SIZE);
  sqnumprocs = sqrt(numprocs);
  stride=N;
  count=N/sqnumprocs;
  blocklength=N/sqnumprocs;

  MPI_Status status[numprocs];
  MPI_Request request[numprocs];

  if(myid != 0){
    printf("count=%d  blocklength=%d stride=%d SIZE=%d\n",count,blocklength,stride,SIZE);
  }
  MPI_Type_vector(count,blocklength,stride,MPI_DOUBLE,&MPI_LEFT_RITE);
  MPI_Type_commit(&MPI_LEFT_RITE);
 
 if(myid == 0){
    printf("send buf = ");
    for (i=0;i<SIZE;i++){
      sendbuf[i]=i;
      printf("%d ", i);
    }
    printf("\n");
    for (i=0;i<sqnumprocs;i++){
      for (j=0;j<sqnumprocs;j++){
	//använder COMM_WORLD eftersom det sak skickas ut till alla
	MPI_Isend(&sendbuf[(N*N/numprocs)*2*i+N/sqnumprocs*j],1,MPI_LEFT_RITE,i*sqnumprocs+j,100,MPI_COMM_WORLD,&request[i*sqnumprocs+j]);
      }      
    }

  }
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Recv(recvbuf,N*N/numprocs,MPI_DOUBLE,0,100,MPI_COMM_WORLD,&status[myid]);


  MPI_Get_elements(&status[myid],MPI_DOUBLE,&bonk2);
  printf("proc %d recv %d elements of type MPI_DOUBLE\n",myid,bonk2);
  for (i=0;i<bonk2;i++)
    if(recvbuf[i] != -1)
      printf("proc: %d  %g\n",myid,recvbuf[i]);

  MPI_Finalize();
}
