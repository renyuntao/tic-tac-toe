#include"forserv.h"

extern int count;

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
			//process  first child
			if((childpid=fork())<0)
			{
				printf("fork() error!\n");
				exit(1);
			}
			else if(childpid>0)    //first child exit
				exit(0);
			
			//we are second child
			sleep(2);
			printf("second child,parent id=%d\n",getppid());
			init();
			process_child(serv_clnt_sock);
		}
		else       //parent process
		{
			close(serv_clnt_sock);
			printf("Before wait\n");
			wait(NULL);
			printf("After wait\n");
		}
	}
}

