#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>
#include <arpa/inet.h>

#define PORT 8080
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// function to handle messaging with threading .
void *socket_thread_send(void *arg){
  int newSocket = *((int *)arg);
  char input_msg[1000];
  do{
    // printf("Client : ");
    fgets(input_msg, 1000, stdin);
    send(newSocket, input_msg, strlen(input_msg), 0);
  }while(strcmp(input_msg,"exit\n"));
  exit(0);
}

void *socket_thread_recv(void *arg){
  int valread;
  do{
    char buffer[1000];
    int newSocket1 = *((int *)arg);
    valread = recv(newSocket1, buffer, 1000, 0);
    if(valread){
      printf("\b\b\b\b\b\b\b\b\b\nServer : ");
      printf("%.*s", valread, buffer);
    }
  }while(valread);
  exit(0);
}

int main(int argc, char const *argv[]) {

  int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "192.168.0.104", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
  else{
    printf("Connected to Server\n");
  }

  pthread_t thread_id1;
  pthread_t thread_id2;

  if(pthread_create(&thread_id1, NULL, socket_thread_send, &sock) != 0){
    printf("failed to create send thread\n");
  }

  if(pthread_create(&thread_id2, NULL, socket_thread_recv, &sock) != 0){
    printf("failed to create recv thread\n");
  }
  pthread_join(thread_id1, NULL);
  pthread_join(thread_id2, NULL);
  return 0;
}
