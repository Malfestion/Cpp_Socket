#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <thread>

using namespace std;
void serverUDP(int myPort){
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);//define un socket ipv4 UDP con los parametros
  int length, fromlen;//tamanos que se usan en otros parametros
  struct sockaddr_in server;//structs que contienen ip puerto y otras opciones este es el propio
  struct sockaddr_in from;// este es de donde recibe mensajes
  length = sizeof(server);
  bzero(&server, length);
  server.sin_family = AF_INET;//opcion ipv4
  server.sin_addr.s_addr =INADDR_ANY; //inet_addr(myIP.c_str());//esto es para decir cual es la IP
  server.sin_port = htons(myPort);//esto es para decir cual es el puerto htons es para pasar de big a little endian
  if (bind(sockfd, (struct sockaddr *)&server, length) < 0){//control de errores, siempre que falla aca es probablemente que el puerto esta en uso
    //error
  }
  fromlen = sizeof(struct sockaddr_in);
  char buffer[200];//buffer donde recibe el mensaje
  while(1){//while true para recibir varios mensajes
    memset(buffer,0,sizeof(buffer));//se pone el buffer en blanco
    recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);// recibe mensaje de from en buffer
    strcat(buffer,"\n");//se pone un endline al final del mensaje
    printf("Soy el  servidor y recibi:%s",buffer );// se imprime el mensaje
}

close(sockfd);//se cerraria el socket.
}
int main(int argc, char** argv){
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(0);
  }

  int myPort=atoi(argv[1]);
  serverUDP(myPort);
  return 0;
}
