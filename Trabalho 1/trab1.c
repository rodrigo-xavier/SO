#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>


int main()
{
   int pid, idfila, fd[2], estado;
   struct mensagem
   {
      long pid;
      char msg[30];
   };

   struct mensagem mensagem_env[10], mensagem_rec[10];

   /* cria */
   if ((idfila = msgget(0x1800, IPC_CREAT|0x1B6)) < 0)
   {
     printf("erro na criacao da fila\n");
     exit(1);
   }


   if (( pid = fork()) < 0)
   {
     printf("erro no fork\n");
     exit(1);
   }
   if (pid == 0){                            //Processo filho
   for (int j=0; j<10; j++){
     msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0);
     printf("%dª mensagem recebida = %ld %s\n",j+1, mensagem_rec->pid, mensagem_rec->msg);
     sleep(5);
   }
   exit(0);

  }else                                     //Processo pai
   {
     printf("PID do processo pai: %d\n", getpid());
     for (int j=0; j<10; j++){
       mensagem_env->pid = getpid();
       strcpy(mensagem_env->msg, "Mensagem enviada");
       msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0);
     }
     wait(&estado);
     exit(0);
   }
}
