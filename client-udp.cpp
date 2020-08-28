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
#include <vector>
#include <mutex>
using namespace std;
bool threadGo=true;

void clientUDP(int hisPort, string hisIP,int id,string msg){
  string a;// string para cin
  struct sockaddr_in servaddr={0};//opciones de socket igual que en el server y el from de antes

  int sockfd = socket(AF_INET, SOCK_DGRAM,0);//se crea el socket igual que antes
  if(sockfd<0){// error que tambien debi escribir arriba pero me dio pereza
    //error
  }


  servaddr.sin_family=AF_INET;//ipv4
  servaddr.sin_port=htons(hisPort);//puerto little endian
  servaddr.sin_addr.s_addr=inet_addr(hisIP.c_str());//ip

  while (threadGo){//para mandar varios mensajes
    a=msg;
    a=a+" //Enviado por cliente base: "+to_string(id);
    const char * msg = (a).c_str();//para mandarlo por el socket
    int len=sendto(sockfd,(const char *)msg, strlen(msg),0,(const struct sockaddr *)&servaddr, sizeof(servaddr));// sendto es para enviar msg al socket (no se conecta como en TCP solo manda de una y se desentiende)
    usleep(2*1000000);
   if(len<1){//si falla el send
      //error
    }
  }
  close(sockfd);//se ciera el socket
}
int main(int argc, char** argv){
  int hisPort;
  string hisIP,sId, sPort,sIP;
  int id=0;
  int currentThread=0;
  vector<thread> hilos;
  if (argc != 2) {
    printf("error: no se encontro ID como parametro\n");
    exit(1);
  }
  id=atoi(argv[1]);
  while(1){
    cout<<"ingrese una ip y puerto respectivamente:"<<endl;
    cin>>sIP;
    if(sIP.compare("Terminar")==0){
      threadGo=false;
      break;
    }
    cin>>sPort;
    currentThread++;
    string msg=to_string(currentThread);
    hisIP=sIP;
    hisPort=stoi(sPort);
    thread newT(clientUDP, hisPort,hisIP,id,msg);
    hilos.push_back(move(newT));
  }
  cout<<"Se ha indicado Terminar el programa...."<<endl;
  cout<<"a continuacion se comunicara a los hilos activos, se cerraran los sockets y se hara join()...."<<endl;
  for(auto& it : hilos){
    it.join();
  }

  return 0;
}
