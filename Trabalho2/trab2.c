// Comando p/ compilar:
// gcc trab2.c -o trab2 -lm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

int main()
{
  pid_t pids[3];
  int j;
  int n = 3;

  // Laço para criar os filhos
  for (j = 0; j < n; ++j) {
   if ((pids[j] = fork()) < 0) {
     printf("erro no fork\n");
     exit(1);
   } else if (pids[j] == 0) {

     // Contabiliza tempo de inicio
     struct timeval begin, end;
     gettimeofday(&begin, 0);


     long int i;
     for(i=0; i<20000000000; i++);/* busy waiting com 100% de CPU – demora 25s */

     // Contabiliza tempo de fim e faz a difereça
     gettimeofday(&end, 0);
     long seconds = end.tv_sec - begin.tv_sec;
     long microseconds = end.tv_usec - begin.tv_usec;
     double elapsed = seconds + microseconds*1e-6;

     printf("Filho de PID: %d, tempo de execução: %.5f segundos \n", getpid(),  elapsed);
     exit(0);
   }
  }

  /*
    IMPLEMENTAR O ESCALONADOR DOS FILHOS
  */
  exit(0);
}
