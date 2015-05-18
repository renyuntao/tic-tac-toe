#include"game.h"


void zero_buf(char ch[])
{
	int i;
	for(i=0;i<BUF_SIZE;i++)
		ch[i]=0;
}

int main(int argc,char **argv)
{
	char flag[2]={0,0};
	int clnt_sock;
	char buf[BUF_SIZE]={0,};
	struct sockaddr_in serv_addr;

	if(argc!=3)
	{
		printf("Usage:%s <IP> <Port>\n",argv[0]);
		exit(1);
	}

	flag[0]=0;
	clnt_sock=socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	{
		fputs("connect() error\n",stderr);
		exit(1);
	}


	read(clnt_sock,buf,BUF_SIZE);      //receive "Do you have an account?(y/n)"
	fputs(buf,stdout);
	//zero_buf(buf);
	memset((void*)buf,0,sizeof(buf));
	fgets(buf,BUF_SIZE,stdin);

	while(strcmp(buf,"y\n")!=0 && strcmp(buf,"n\n")!=0)      //invalid input,i.e. input is not 'y' or 'n'
	{
		printf("Invalid input!Please input again:\n");
		memset((void*)buf,0,sizeof(buf));
		printf("Do you have an account?(y/n)");
		fgets(buf,BUF_SIZE,stdin);
	}
	write(clnt_sock,buf,strlen(buf));

	//if don't have an account
	if(strcmp(buf,"n\n")==0)
	{

		read(clnt_sock,buf,BUF_SIZE);
		fputs(buf,stdout);
		
		zero_buf(buf);

		read(clnt_sock,buf,BUF_SIZE);   //receive "Enter the username"
		fputs(buf,stdout);
		fgets(buf,BUF_SIZE,stdin);
		write(clnt_sock,buf,strlen(buf));
		
		read(clnt_sock,buf,BUF_SIZE);    //receive "Enter the password"
		fputs(buf,stdout);
		fgets(buf,BUF_SIZE,stdin);
		write(clnt_sock,buf,strlen(buf));

		read(clnt_sock,buf,BUF_SIZE);    //receive "Confirm the password"
		fputs(buf,stdout);
		fgets(buf,BUF_SIZE,stdin);
		write(clnt_sock,buf,strlen(buf));

		memset((void*)buf,0,sizeof(buf));
		//zero_buf(buf);
		read(clnt_sock,buf,2);
		printf("Before ok buf:%s\n",buf);
		if(strcmp(buf,"ok")==0)
			printf("Now you already have an account,let's to sign in!\n");
		sleep(2);

	}



   //if have an account
   	memset((void*)buf,0,sizeof(buf));
	read(clnt_sock,buf,BUF_SIZE);	     
	fputs(buf,stdout);                    //show  "Enter the username:"
	fgets(buf,BUF_SIZE,stdin);            //input the username
	write(clnt_sock,buf,strlen(buf));

	read(clnt_sock,buf,BUF_SIZE);      
	fputs(buf,stdout);                     //show "Enter the password:"
	fgets(buf,BUF_SIZE,stdin);             //input the password
	write(clnt_sock,buf,strlen(buf));
	zero_buf(buf);

	read(clnt_sock,buf,1);
	//printf("in client buf:%s\n",buf);
	if(!strcmp(buf,"2"))
	{
		puts("Login success!");
		puts("Now,let's begin the game!");     //begin game
		printf("You use the 'X',and computer use the 'O'\n");
		game();
	}
	else
	{
		puts("password is not correct");
	}
	return 0;
}

