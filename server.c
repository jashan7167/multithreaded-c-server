#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<netinet/in.h>
#include<pthread.h>
#include"myqueue.h"
#define SERVERPORT 9082
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 1
#define THREAD_POOL_SIZE 20
pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER; //Condition
variable initializer
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
void handle_connection(int client_socket);
int check(int exp,const char *msg);
void *thread_function(void *arg);
int main(int argc , char **argv)
{
int server_socket,client_socket,addr_size;
SA_IN server_addr , client_addr;
for(int i = 0;i<THREAD_POOL_SIZE;i++) //creating threads
binded to the thread_function
{
pthread_create(&thread_pool[i],NULL,thread_function,NULL);
}
check((server_socket = socket(AF_INET,SOCK_STREAM,0)),"Failed to
create a socket");
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(SERVERPORT);
check(bind(server_socket,(SA*)&server_addr,sizeof(server_addr)),"Bind
Failed");
check(listen(server_socket,SERVER_BACKLOG),"Listen Failed!");
while(true)
{
printf("Waiting for connections...\n");
addr_size = sizeof(SA_IN);
check(client_socket =
accept(server_socket,(SA*)&client_addr,(socklen_t*)&addr_size),"accept
failed");
printf("Connected!\n");
int *pclient = malloc(sizeof(int));
*pclient = client_socket;
pthread_mutex_lock(&mutex);
enqueue(pclient);
pthread_cond_signal(&condition_var);
pthread_mutex_unlock(&mutex);
}
}
int check(int exp, const char *msg)
{
if(exp== SOCKETERROR)
{
perror(msg);
exit(1);
}
return exp;
}
void *thread_function(void *arg)
{
while(true)
{
int *pclient;
pthread_mutex_lock(&mutex); //racecondition fix
if((dequeue())==NULL)
{
pthread_cond_wait(&condition_var,&mutex); //we passed mutex
here because if a thread suspends it releases the lock and it will alow
some other thread to grab it and we dont want that so we passed in the
mutex too make it locked
pclient = dequeue();
}
pthread_mutex_unlock(&mutex);
if(pclient!=NULL)
{
handle_connection(*pclient);
free(pclient);
}
// else{
// sleep(1); //not a good solution for high cpu usage
// }
}
}
void handle_connection(int client_socket)
{
char buffer[BUFSIZE];
size_t bytes_read;
char actualpath[_PC_PATH_MAX+1];
// reading the client request
bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
check(bytes_read, "recv error");
buffer[bytes_read - 1] = '\0';
printf("REQUEST: %s\n", buffer);
fflush(stdout);
if (realpath(buffer, actualpath) == NULL)
{
printf("ERROR(badpath): %s\n", buffer);
close(client_socket);
return;
}
FILE *fp = fopen(actualpath, "r");
if (fp == NULL)
{
printf("ERROR(open): %s\n", buffer);
close(client_socket);
return;
}
while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
{
printf("sending %zu bytes\n", bytes_read);
write(client_socket, buffer, bytes_read);
}
close(client_socket);
fclose(fp);
printf("closing connection\n");
}