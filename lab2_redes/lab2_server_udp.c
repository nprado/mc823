/*lab2 server*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>
#include <unistd.h>
#include "materias.h"

#define MAX_BUF_SIZE 1024

int main(/*int argc, char *argv[]*/) {
	int sockfd, new_sockfd;
	int bytes;
	unsigned int clilen;
	socklen_t length; //para accept() e bind()
	char buffer[1024], oper[8], sigla[5], quit;
	char* buffer_saida;
	char comment[1000];	
	char* lista_materias;
	int i,j;
	int lSize;
	  
	pid_t chpid; // para conexoes concorrentes
	  
	Course* c;
	  
	struct sockaddr_in server;
	struct sockaddr_in client;
	int porta = 8099; // porta que sera usada nesse servidor
	  
	sockfd=socket(AF_INET,SOCK_DGRAM,0);

  	bzero(&client,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr=htonl(INADDR_ANY);
	client.sin_port=htons(porta);
	bind(sockfd,(struct sockaddr *)&client,sizeof(client));

 
	printf("\nAguardando datagramas na porta 8099...\n\n");
	for(;;) {	
		
		while (1) {
		  clilen = sizeof(client);
		  // new_sockfd >>> sock_fd: server iterativo e nao mais concorrente
		  bytes = recvfrom(sockfd, buffer, MAX_BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&client, &clilen);
		  
		  if(bytes <= 0) { // NOTE: ocorreu um erro = -1 ou cliente fechou conexão = 0
			perror("server_recv:::");
			//close(new_sockfd);
			close(sockfd);
			exit(1);
		  }
		  
		  buffer[bytes] = 0x00;//0x00 o mesmo que '\0'
		  quit = buffer[0];
		  printf("%s", buffer);
		  if (quit =='Q') {
			exit(0);
		  }

		  //prepara parametro operacao----------
		  for (i=0 ; i<8 ; i++) {
			oper[i] = buffer[i];
		  }	
		  oper[8] = 0x00;
		  
		  printf("%s\n", buffer);	
		  printf("%s\n", oper);
		  
		  
		  //---------------OPER INFORMACAO <SIGLA >---------------
		  
		  
		  if (strcmp(oper,"informar")==0) {
			//prepara parametro sigla
			for (j=0; j<5;j++){
						sigla[j] = buffer[j+9];
			}
			sigla[5] = 0x00;	
			//chama a funcao 
			
			c = info_course(sigla);
			lSize = strlen(c->title);
			lSize +=strlen(c->program);
			lSize +=strlen(c->date);
			lSize +=strlen(c->comment);
			printf("%d\n", lSize);
			buffer_saida = 	(char*) malloc (sizeof(char)*(lSize+3));
			
				
			strcat(buffer_saida,c->title);
			strcat(buffer_saida,"\n");
			strcat(buffer_saida,c->program);
			strcat(buffer_saida,"\n");
			strcat(buffer_saida,c->date);
			strcat(buffer_saida,"\n");
			strcat(buffer_saida,c->comment);
			printf("%s", buffer_saida);

		  }
		  //-----------------------------------------------------------
		  //----------------Oper Programa <sigla>----------------------
		  if (strcmp(oper, "programa")==0) {
			//prepara parametro sigla
			for (j=0; j<5;j++) {
			  sigla[j] = buffer[j+9];
			}
			sigla[5] = 0x00;	
			//chama a funcao
			c = info_course(sigla);
			lSize = strlen(c->program);
			buffer_saida = 	(char*) malloc (sizeof(char)*(lSize + 10));
			strcpy(buffer_saida,"Programa:\n");
			strcat(buffer_saida,c->program);
			
		  }	
		  //-----------------------------------------------------------
		  //----------------Comentar uma Materia-----------------------
		  if (strcmp(oper, "comentar")==0) {
			//prepara parametro sigla
			for (j=0; j<5;j++) {
			  sigla[j] = buffer[j+9];
			}
			sigla[5] = 0x00;	
			//prepara commentario

				i = 0;
			while(buffer[i]!='\n') {
			 comment[i] = buffer[i+16];
			 i++;
			}
			comment[i-1]=0x00;			   
			insert_comment(sigla, comment);
			buffer_saida = (char*) malloc (sizeof(char)*(16));	
			strcpy(buffer_saida, "Comentario Feito");
		  }
		  //-----------------------------------------------------------
		  //----------------ler comentario de uma materia--------------
		  if (strcmp(oper, "lercomen")==0) {
			//prepara parametro sigla
			for (j=0; j<5;j++) {
			  sigla[j] = buffer[j+9];
			}
			sigla[5] = 0x00;	
			//chama a funcao
			c = info_course(sigla);
			lSize = strlen(c->comment);
			buffer_saida = 	(char*) malloc (sizeof(char)*(lSize + 11));
			strcpy(buffer_saida,"Comentario:\n");
			strcat(buffer_saida,c->comment);
		  }
		  //-----------------------------------------------------------
		  //-------------------Listar Materias-------------------------

		  if(strcmp(oper, "listagem")==0) {
			lista_materias = list();
			lSize = strlen(lista_materias);
			buffer_saida = (char*) malloc (sizeof(char)*(lSize));
			strcpy(buffer_saida,lista_materias);
		  }

		  //-----------------------------------------------------------
		  //-------------------Listar Tudo-----------------------------

		  if(strcmp(oper, "listtudo")==0) {
			lista_materias = listAll();
			lSize = strlen(lista_materias);
			buffer_saida = (char*) malloc (sizeof(char)*(lSize));
			strcpy(buffer_saida,lista_materias);
		  }

		  // ---------------------ENVIA--------------------------------	
		  clilen = sizeof(client);
		
		  bytes = sendto(sockfd, buffer_saida, lSize, 0, (struct sockaddr *)&client, sizeof(client));
		  //ocorreu um erro = -1
		  if(bytes < 0) {
			  perror("server_send:::");
			  //close(new_sockfd);
			  close(sockfd);
			  exit(1);
		  }
		  printf("\nRECEBIDO :%s\n\n", buffer);
		  buffer_saida = NULL;
		}
    }
  free(buffer_saida);
  
  close(sockfd);
  
  return 0;
}
