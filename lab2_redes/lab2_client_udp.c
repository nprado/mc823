#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <sys/types.h>
#include <sys/socket.h>
//struct sockaddr_in, htons() e outras
#include <netinet/in.h>
//funções inet_*()
#include <arpa/inet.h>

#include <errno.h> /*perror()*/
#include <unistd.h> /*close()*/
 #include <signal.h>
#include <setjmp.h>
// o datagrama padrão possui 65535 bytes, sendo 28 de cabeçalho UDP+IP (restando 65507 para dados)
#define MAX_BUF_SIZE 1024

#define RECV_TIMEOUT 10

static sigjmp_buf recv_timed_out;

    /* timeout handler */
void timeout_handler (int signum)
 {
        signal(SIGALRM, SIG_DFL);
        siglongjmp(recv_timed_out, 1);
 }


int main(int argc, char *argv[]){
	
	int sockfd;
	int bytes;
	int porta = 8099;

	socklen_t length;
	char recv_buffer[MAX_BUF_SIZE], send_buffer[MAX_BUF_SIZE], quit;
	struct sockaddr_in server;

	if(argc != 2)
	
	{
		printf("\n\tusage: %s <ip_do_host>  \n\n", argv[0]);
		close(sockfd);
		exit(1);
	}

	length = sizeof(struct sockaddr);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
   	bzero(&server,sizeof(server));
   	server.sin_family = AF_INET;
   	server.sin_addr.s_addr=inet_addr(argv[1]);
   	server.sin_port=htons(porta);


	length = sizeof(struct sockaddr);
	quit = 'N';
	printf("\nDigita a operacao e argumento(s), caso necessario:\n");
		printf("listagem: lista todas as materias \n");
		printf("informar <sigla> : ver informacoes sobre uma materia \n");
		printf("programa <sigla> : ver programa de uma materia \n");
		printf("comentar <sigla> : escrever um comentario sobre a proxima aula de uma materia \n");
		printf("lercomen <sigla> : ler comentario sobre a proxima aula de uma disciplina\n");
		printf("listtudo         : ver todas as informacoes sobre todas disciplinas\n");
		printf("Q                : sair do programa\n\n");

	
	while(quit != 'Q')
	{
		
		
		printf("<materias-0.1>");

		
		//lendo da entrada padrão
		fgets(send_buffer, 1024, stdin);
		//se no inicio da string estiver o 'Q' então esse é o ultimo loop
		quit = send_buffer[0];
		printf("buffer : %s\n", send_buffer);

		if(quit=='Q') {
			exit(0);
		}
		
		//enviando string ao servidor. strlen(send_buffer) pode ser menor que MAX_BUF_SIZE
		bytes = sendto(sockfd, send_buffer, strlen(send_buffer), 0, (struct sockaddr *)&server, sizeof(struct sockaddr));
		if(bytes < 0) {
			perror("client_send:::");
			close(sockfd);
			exit(1);
		}

		length = sizeof (struct sockaddr);
		/* set timer and handler */
		signal(SIGALRM, timeout_handler);
		alarm(RECV_TIMEOUT);
	
		bytes = recvfrom(sockfd, recv_buffer, MAX_BUF_SIZE, 0, (struct sockaddr *)&server, &length);
		
		/* clear timer and handler */
		alarm(0);
		signal(SIGALRM, SIG_DFL);

	
		//supondo que não ocorreu um erro acima
		recv_buffer[bytes] = 0x00;//ponha o caractere '\0'
		
		//servidor fechou a conexão ou ocorreu um erro
		if(bytes <= 0) {//ocorreu um erro = -1, fechou conexão = 0
			perror("client_recv:::");
			close(sockfd);
			exit(1);
		}
		//só agora podemos ver a mensagem recebida do servidor
		printf("\nRECEBIDO:::%s\n\n", recv_buffer);
		
	}
		close(sockfd);
	
	return 0;
}
	
