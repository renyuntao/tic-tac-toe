#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<netdb.h>
#include<signal.h>
#include<unistd.h>
#include<error.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<errno.h>
#include <pthread.h>
#define MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define LISTEN_NUM 8
#define BUF_SIZE 500

void zero_buf(char ch[]);
void sign_up(int serv_clnt_sock);
void process_child(int serv_clnt_sock);

