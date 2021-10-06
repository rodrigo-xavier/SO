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

   // vetores p/ mensagens enviadas e recebidas
   struct mensagem mensagem_env[10], mensagem_rec[10];

   // cria a fila de mensagens
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
     // laço para receber as 10 mesagens, 1 a cada 5 segundos
     for (int j=0; j<10; j++){
       // recebe a mensagem
       msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0);
       // print
       printf("%dª mensagem recebida, pid de quem enviou: %ld, mensagem: \"%s\"\n",j+1, mensagem_rec->pid, mensagem_rec->msg);
       sleep(5);
     }
     exit(0);

  }else                                     //Processo pai
   {
     printf("PID do processo pai: %d\n", getpid());
     // laço para enviar as 10 mensagems para o filho
     for (int j=0; j<10; j++){
       mensagem_env->pid = getpid();
       strcpy(mensagem_env->msg, "Mensagem ...");
       msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0);
     }

     // espera o filho morrer
     wait(&estado);
     // se mata
     exit(0);
   }
}
