#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define TAM 3                               // Quantidade de filhos


/* Função que vai tratar quando um filho mandar sinal p/ o pai (SIGCHLD)
	O filho manda sinal sempre que muda de estado, logo tem que checar se
	o sinal é decorrente do fim da execução
*/
void handler (int j) {
    int status=-1, pid,i;
    // Variavel estática restante, que serve p/ saber se ainda há filho em execução
    static int restante = TAM;
    // waitpid que avalia todos os processos (-1) e não é bloqueante (WNOHANG)
    pid = waitpid(-1,&status, WNOHANG);

    // checa se o processo que mandou o status de fato terminou a execução
    if (status == 0){
      // decrementa a quantidade restante de filhos
      restante--;
      // se não há mais filhos executando, finaliza o processo pai
      if(restante==0)
        exit(0);
    }
}


int main()
{
  int j;                                    // variavel p/ loops
  pid_t pids[TAM];                          // vetor com os pids dos filhos

  // Laço para criar os filhos
  for (j = 0; j < TAM; ++j) {
    pids[j] = fork();
    if (pids[j] > 0) {                    // caso seja pai
      // pausa filho recem criado
      if(kill(pids[j],SIGSTOP) != 0)
        printf("erro ao parar o processo %d\n", pids[j]);

    } else if (pids[j] == 0) {            // caso seja filho

     // Contabiliza tempo de inicio
     struct timeval begin, end;
     gettimeofday(&begin, 0);
     // rotina do filho p/ gastar tempo
     long int i;
     for(i=0; i<20000000000; i++);/* busy waiting com 100% de CPU – demora 25s */
     // Contabiliza tempo de fim e faz a difereça
     gettimeofday(&end, 0);
     long seconds = end.tv_sec - begin.tv_sec;
     long microseconds = end.tv_usec - begin.tv_usec;
     double elapsed = seconds + microseconds*1e-6;
     printf("Filho de PID: %d, tempo de execução: %.5f segundos \n", getpid(),  elapsed);
     // finaliza o processo filho
     exit(0);
   }else{                                   // caso dê errado
    printf("erro no fork\n");
    exit(1);
  }
  }

  /*
    Escalonador dos filho:
      o processo pai roda o vetor de pids, iniciando a execução do filho, esperando 5s e parado-o
      Caso nao consiga enviar o sinal, o filho já morreu, entao apenas dá um continue pra ir p/ proximo
  */
  signal(SIGCHLD, handler);
  j = -1;
  while(1){
    j = j+1;
    j = j%TAM;
    // inicio o proximo filho
    if(kill(pids[j],SIGCONT) != 0)
      continue;
    // dorme por 5s
    sleep(5);
    // Pausa o que está em execução
    if(kill(pids[j],SIGSTOP) != 0)
      continue;
  }
}
