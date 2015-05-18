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

int main(int argc,char **argv)
{
	int serv_sock,serv_clnt_sock;
	pid_t childpid;
	socklen_t clnt_addr_len;
	struct sockaddr_in serv_addr,clnt_addr;

	if(argc!=2)
	{
		printf("Usage:%s <port>\n",argv[0]);;
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	{
		fputs("bind() error\n",stderr);
		exit(1);
	}

	if(listen(serv_sock,LISTEN_NUM)==-1)
	{
		fputs("listen() error\n",stderr);
		exit(1);
	}

	for(;;)
	{
		clnt_addr_len=sizeof(clnt_addr);
		if((serv_clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_len))==-1)
		{
			fputs("accept() error\n",stderr);	
			perror("accept() error");
			exit(1);
		}

		//printf("After accept\n");
		if((childpid=fork())==-1)
		{
			fputs("fork() error\n",stderr);
			exit(1);
		}
		else if(childpid==0)      //child process
		{
			close(serv_sock);	
			//process child
			process_child(serv_clnt_sock);
		}
		else       //parent process
			close(serv_clnt_sock);
	}
}

void zero_buf(char ch[])
{
	int i;
	for(i=0;i<BUF_SIZE;i++)
		ch[i]=0;
}

void sign_up(int serv_clnt_sock)
{
	int m,n;
	int fd;
	char insert[BUF_SIZE]={0,};
	char name[BUF_SIZE]={0,};       //store the username
	char first_p[BUF_SIZE]={0,};    //store the password which is first input
	char second_p[BUF_SIZE]={0,};   //store the password which is second input

	//printf("Before print Enter the username\n");
	sleep(2);
	write(serv_clnt_sock,"Enter the username:",strlen("Enter the username:"));
	read(serv_clnt_sock,name,BUF_SIZE);
	n=strlen(name);
	name[n-1]=0;

	write(serv_clnt_sock,"Enter the password:",strlen("Enter the password:"));
	read(serv_clnt_sock,first_p,BUF_SIZE);
	n=strlen(first_p);
	first_p[n-1]=0;

	write(serv_clnt_sock,"Confirm the password:",strlen("Confirm the password:"));
	read(serv_clnt_sock,second_p,BUF_SIZE);
	n=strlen(second_p);
	second_p[n-1]=0;

	if(strcmp(first_p,second_p)==0)
	{
		n=strlen(name);
		strcpy(insert,name);
		insert[n]=' ';
		m=strlen(first_p);
		strcpy((insert+n+1),first_p);
		insert[n+1+m]=',';
		fd=open("info",O_WRONLY|O_APPEND|O_CREAT,MODE);
		write(fd,insert,strlen(insert));      //write the new account and password to the file
		printf("Sign up success!,Now,you already have an account!\n");
		write(serv_clnt_sock,"ok",2);
		//exit(0);
	}
	else
	{
		printf("You entered two different passwords,so you can't create an account!\n");
		write(serv_clnt_sock,"You entered two different passwords,so you can't create an account!\n",strlen("You entered two different passwords,so you can't create an account!\n"));
		exit(0);
	}

}

void process_child(int serv_clnt_sock)
{
	int n;
	int j;
	int count;
	int info_fd;
	char flag[2]={0,0};
	char yorn[5]={0,};
	char tt[BUF_SIZE]={0,};
	char *tmp;
	char *name;
	char info[BUF_SIZE]={0,};
	char buf[BUF_SIZE]={0,};
	char user_name[]="Enter the username:";
	char passwd[]="Enter the password:";

	while(1)
	{
		write(serv_clnt_sock,"Do you have an account?(y/n):",strlen("Do you have an account?(y/n)"));
		read(serv_clnt_sock,yorn,5);
		if(strcmp(yorn,"y\n")==0)
			break;
		else if(strcmp(yorn,"n\n")==0)
		{
			printf("Let's create an account...\n");
			write(serv_clnt_sock,"Let's create an account...\n",strlen("Let's create an account...\n"));
			sign_up(serv_clnt_sock);
			//exit(0);
			sleep(2);
			break;
		}
		else
		{
			printf("Invalid input!\n");
		}
	}




	write(serv_clnt_sock,user_name,strlen(user_name));    //send message of "Enter the usrname:" to client
	//for(j=0;j<BUF_SIZE;j++)
	//	buf[j]=0;
	zero_buf(buf);
	read(serv_clnt_sock,(void*)buf,BUF_SIZE);                    //receive the message of username from client

	n=strlen(buf);
	buf[n-1]=0;

	info_fd=open("info",O_RDONLY);
	zero_buf(info);
	printf("info:%s\n",info);
	read(info_fd,(void*)info,BUF_SIZE);                      //read from the file which save the message of username
	close(info_fd);
	printf("info:%s\n",info);

	n=strlen(info);
	info[n-1]='\0';
	//printf("info:%s\n",info);	
	strcpy(tt,info);	

	
	name=strtok(info,",");

	flag[0]=0;
	count=0;
	while(name!=NULL)
	{
		n=strlen(buf);
		if(buf[n-1]=='\n')
			buf[n-1]=0;
		tmp=name;
		//printf("buf:%s\n",buf);
		//printf("tmp:%s\n",tmp);
		if(strcmp(strtok(tmp," "),buf)==0)                //username is exist
		{
			flag[0]=1;      //stand for  username is correct
			write(serv_clnt_sock,passwd,strlen(passwd));	 //send the message of "Enter the password:" to client
			//for(j=0;j<BUF_SIZE;j++)
			//	buf[j]=0;
			zero_buf(buf);
			read(serv_clnt_sock,buf,BUF_SIZE);               //receive the message of password from client
			
			n=strlen(buf);
			buf[n-1]='\0';

			//printf("Befort strtok tmp:%s\n",tmp);
			tmp=strtok(NULL,",");
			//printf("tmp:%s\n",tmp);
			if(strcmp(tmp,buf)==0)                      //password is correct
			{
				flag[0]=2;   //stand for login success
				//print ttt
				printf("Ok!\n");
				//write(serv_clnt_sock,"Login success!\n",sizeof("Login success!\n"));
				write(serv_clnt_sock,"2",1);

			}
			else
			{
				printf("password is not correct!\n");
				write(serv_clnt_sock,"password is not correct!\n",sizeof("password is not correct!\n"));
				exit(1);
			}
		}
		count++;
		if(count%2==0)
		{
			strcpy(tt,info);
			name=strtok(info,",");
		}
		else
		{
			strcpy(info,tt);
			name=strtok(tt,",");
		}
		for(j=0;j<count;j++)
			name=strtok(NULL,",");
	}
	if(flag[0]==0)
	{
		printf("username is not exist!\n");
		write(serv_clnt_sock,"username is not exist!\n",strlen("username is not exist!\n"));
		exit(1);
	}

}


