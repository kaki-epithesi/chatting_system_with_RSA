#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>

#define PORT 8080
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// function to handle messaging with threading .
void *socket_thread_send(void *arg){
  int newSocket = *((int *)arg);
  char input_msg[1000];
  do{
    // printf("Server : ");
    fgets(input_msg, 1000, stdin);
    send(newSocket, input_msg, strlen(input_msg), 0);
  }while(strcmp(input_msg,"exit\n"));
  exit(0);
}

void *socket_thread_recv(void *arg){
  int valread;
  int newSocket1 = *((int *)arg);
  do{
    char buffer[1000];
    valread = recv(newSocket1, buffer, 1000, 0);
    if(valread){
      // char tp[1000];
      // strncpy(tp, buffer, valread);
      // strcat(tp, "Server : ");
      printf("\b\b\b\b\b\b\b\b\b\nClient : ");
      printf("%.*s", valread, buffer);
    }

  }while(valread);
  exit(0);
}

int main(int argc, char const *argv[]){

  int server_fd, valread, newSocket;

  struct sockaddr_in address;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  int opt = 1;
  int addrlen = sizeof(address);

  // settings for socket connection
  // settings start
  if(1){
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    	perror("socket failed");
    	exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))){
    	perror("setsockopt");
    	exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
    	perror("listen");
    	exit(EXIT_FAILURE);
    }
    // if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
    // 	perror("accept");
    // 	exit(EXIT_FAILURE);
    // }
}
  // settings end

  pthread_t thread_id1;
  pthread_t thread_id2;

  addr_size = sizeof serverStorage;
  newSocket = accept(server_fd, (struct sockaddr *) &serverStorage, &addr_size);

  if(newSocket){
    printf("Connected to Client\n");
  }

  if(pthread_create(&thread_id1, NULL, socket_thread_send, &newSocket) != 0){
    printf("failed to create send thread\n");
  }

  if(pthread_create(&thread_id2, NULL, socket_thread_recv, &newSocket) != 0){
    printf("failed to create recv thread\n");
  }

  pthread_join(thread_id1, NULL);
  pthread_join(thread_id2, NULL);
  return 0;
}
