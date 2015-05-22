#include"forserv.h"

int count=0;  //record the number of account

void zero_buf(char ch[])
{
	int i;
	for(i=0;i<BUF_SIZE;i++)
		ch[i]=0;
}

void sign_up(int serv_clnt_sock)
{
	int m,n;
	char recd_count[5];      //used for insert 'map' file,save user id
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
		fd=open("passwd",O_WRONLY|O_APPEND|O_CREAT,MODE);
		write(fd,insert,strlen(insert));      //write the new account and password to the 'passwd' file
		close(fd);

		memset((void*)insert,0,sizeof(insert));
		n=strlen(name);
		strcpy(insert,name);
		insert[n]=' ';
		count++;
		memset((void*)recd_count,0,sizeof(recd_count));
		sprintf(recd_count,"%d",count);
		m=strlen(recd_count);
		strcpy(insert+n+1,recd_count);
		insert[n+1+m]=',';
		fd=open("map",O_WRONLY|O_APPEND);
		write(fd,insert,strlen(insert));       //write the informatino to the 'map' file
		close(fd);

		memset((void*)insert,0,sizeof(insert));
		fd=open("score",O_WRONLY|O_APPEND|O_CREAT,MODE);
		if(count>=0 && count<10)         //_## 0## 0## 0##,
		{
			strcpy(insert,recd_count);
			strcpy(insert+m,"##0##0##0##,");
			write(fd,insert,strlen(insert));    //write the information of score to the 'score' file
		}
		else if(count>=10 && count<100)    //__# 0## 0## 0##
		{
			strcpy(insert,recd_count);
			strcpy(insert+m,"#0##0##0##,");
			write(fd,insert,strlen(insert));      //write the information of score to the 'score' file
		}
		else if(count>=100 && count<1000)    //___ 0## 0## 0##
		{
			strcpy(insert,recd_count);
			strcpy(insert+m,"0##0##0##,");
			write(fd,insert,strlen(insert));      //write the information of score to the 'score' file
		}
		else
		{
			printf("This is NO.1000 user,and this user can't be create!\n");
			exit(0);
		}

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


/*char* */void  get_id(char *map_name,char *id)
{
	int count;
	int i;
	int fd;
	char *p;
	char buf[BUF_SIZE]={0,};
	char tmp[BUF_SIZE]={0,};

	fd=open("map",O_RDONLY);
	memset((void*)buf,0,sizeof(buf));	
	read(fd,buf,BUF_SIZE);
	memset((void*)id,0,sizeof(id));

	count=1;
	while(1)
	{
		printf("in while\n");
		if(count%2!=0)
		{
			memset((void*)tmp,0,sizeof(tmp));
			strcpy(tmp,buf);
			p=strtok(buf,",");
			for(i=1;i<count;i++)
			{
				p=strtok(NULL,",");
				if(p==NULL)
					return;
			}
			p=strtok(p," ");
			if(strcmp(p,map_name)==0)  //找到对应项
			{
				p=strtok(NULL," ");    // get user id
				printf("get id success!\n");
				strcpy(id,p);
				//return p;
				return;
			}
			count++;
		}
		else
		{
			memset((void*)buf,0,sizeof(buf));	
			strcpy(buf,tmp);
			p=strtok(tmp,",");
			for(i=1;i<count;i++)
			{
				p=strtok(NULL,",");
				if(p==NULL)
					return;
			}
			p=strtok(p," ");
			if(strcmp(p,map_name)==0)
			{
				p=strtok(NULL," ");    //get user id
				printf("get id success!\n");
				strcpy(id,p);
				//return p;
				return;
			}
			count++;
		}
	}
}

void get_score(char* id,int *score)
{
	int n;
	int fd;
	//int score_tmp[3];
	char *p;
	char *w;   //win
	char *l;   //lose
	char *d;   //draw
	char win[3]={0,};     // the number of win
	char lose[3]={0,};     //the number of lose
	char draw[3]={0,};    //the number of draw
	char buf[14]={0,};

	printf("in get_score print id:%s\n",id);
	fd=open("score",O_RDONLY);
	while(1)
	{
		n=lseek(fd,0,SEEK_CUR);
		memset((void*)buf,0,sizeof(buf));
		read(fd,buf,3);
		p=strtok(buf,"#");
		if(strcmp(id,p)==0)
		{
			memset((void*)buf,0,sizeof(buf));
			read(fd,buf,9);
			close(fd);
			w=strtok(buf,"#");
			l=strtok(NULL,"#");
			d=strtok(NULL,"#");

			score[0]=atoi(w);
			score[1]=atoi(l);
			score[2]=atoi(d);

			printf("score[0]:%d\n",score[0]);
			printf("score[1]:%d\n",score[1]);
			printf("score[2]:%d\n",score[2]);
			
			return;
		}
		else if(p==NULL)
		{
			printf("in else if\n");
			printf("getscore failure!\n");
			close(fd);
			exit(1);
		}
		else
		{
			printf("in else\n");
			lseek(fd,10,SEEK_CUR);
		}
	}
}


int set_score(int flag,char *curname,char *id)   //flag stand for win or lose or draw,0 is win,1 is lose,2 is draw
{
	int fd;
	long n;
	char ch[5]={0,};
	int tmp;
	char buf[14]={0,};
	char *p;
	
	get_id(curname,id);
	printf("in set_score() id:%s\n",id);
	fd=open("score",O_RDWR);
	while(1)
	{
		memset((void*)buf,0,sizeof(buf));
		n=lseek(fd,0,SEEK_CUR);
		printf("before read n:%ld\n",n);
		read(fd,buf,3);
		printf("after read() buf:%s\n",buf);
		p=strtok(buf,"#");
		printf("after strtok p:%s\n",p);
		printf("id:%s\n",id);
		if(strcmp(id,p)==0)    //找到对应项
		{
			if(flag==0)   //user is win
			{
				tmp=0;
				printf("in set_score flag==0\n");
				memset((void*)buf,0,sizeof(buf));
				read(fd,buf,3);
				p=strtok(buf,"#");
				tmp=atoi(p);
				tmp++;
				sprintf(ch,"%d",tmp);
				lseek(fd,-3,SEEK_CUR);
				write(fd,ch,strlen(ch));
				close(fd);
				return 0;
			}
			else if(flag==1)  //user is lose
			{
				tmp=0;
				printf("in set_score flag==1\n");
				n=lseek(fd,0,SEEK_CUR);
				printf("before lseek,n:%ld\n",n);
				n=lseek(fd,3,SEEK_CUR);
				printf("n:%ld\n",n);
				memset((void*)buf,0,sizeof(buf));
				read(fd,buf,3);
				p=strtok(buf,"#");
				tmp=atoi(p);
				tmp++;
				sprintf(ch,"%d",tmp);
				lseek(fd,-3,SEEK_CUR);
				write(fd,ch,strlen(ch));
				close(fd);
				return 0;
			}
			else if(flag==2)   //user is draw
			{
				tmp=0;
				printf("in set_score flag==2\n");
				lseek(fd,6,SEEK_CUR);
				memset((void*)buf,0,sizeof(buf));
				read(fd,buf,3);
				printf("after read\n");
				printf("buf:%s\n",buf);
				p=strtok(buf,"#");
				tmp=atoi(p);
				tmp++;
				sprintf(ch,"%d",tmp);
				printf("ch:%s\n",ch);
				lseek(fd,-3,SEEK_CUR);
				write(fd,ch,strlen(ch));
				close(fd);
				return 0;
			}
			else
			{
				printf("parameter is error!\n");
				exit(1);
			}
		}
		else if(p==NULL)
		{
			printf("getscore failure!\n");
			close(fd);
			exit(1);
		}
		else
			lseek(fd,10,SEEK_CUR);
	}
}

void process_child(int serv_clnt_sock)
{
	int n;
	int j;
	pid_t pid;
	char curname[12]={0,};   //save current user name
	char id[5]={0,};   //used in get_id(),save user id
	int score[3];   //used in get_score(),save win,lose,draw number
	char ch_score[5]={0,};  //used for send score to client 
	int count;     //used for strtok,control the number of loop
	int info_fd;
	char flag[2]={0,0};
	char yorn[5]={0,};
	char tt[BUF_SIZE]={0,};
	char *tmp;
	char *name;
	char map_name[BUF_SIZE];        //used for 'map' file
	//char *id;              //save user id  ('map' file)
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
	zero_buf(buf);
	read(serv_clnt_sock,(void*)buf,BUF_SIZE);                    //receive the message of username from client

	n=strlen(buf);
	buf[n-1]=0;

	memset((void*)map_name,0,sizeof(map_name));
	strcpy(map_name,buf);      //used for get ID of user in 'map' file
	//id=get_id(map_name);       //user id is pointed by id
	get_id(map_name,id);
	printf("ID:%s\n",id);
	if(strcmp(id,""))
	{
		get_score(id,score);
	}

	info_fd=open("passwd",O_RDONLY);
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
			strcpy(curname,buf);
			printf("curname:%s\n",curname);
			flag[0]=1;      //stand for  username is correct
			write(serv_clnt_sock,"ok",strlen("ok"));
			sleep(1);
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

	
	//***************LOGIN SUCCESS**********************
	
	if((pid=fork())<0)
	{
		printf("fork() error\n");
		perror("server fork()");
		exit(1);
	}
	else if(pid==0)    //child process 
	{
		if((pid=fork())<0)
		{
			printf("second fork() error\n");
			perror("second server fork()");
			exit(1);
		}
		else if(pid==0)    //second child process
		{
			if(execl("./chat_serv","chat_serv","6767",(char*)0)<0)
			{
				fprintf(stderr,"execl() error!\n");
				perror("server execl()");
				exit(1);
			}
		}
		else      //first child process,i.e. second child process's parent process
			exit(0);
	}
	wait(NULL);     //parent process,wait first child process

	while(1)
	{
		printf("in server while");
		memset((void*)flag,0,sizeof(flag));
		read(serv_clnt_sock,flag,1);
		if(strcmp(flag,"1")==0)
		{
			//show_score(score);	
			memset((void*)ch_score,0,sizeof(ch_score));
			sprintf(ch_score,"%d",score[0]);
			write(serv_clnt_sock,ch_score,strlen(ch_score));

			read(serv_clnt_sock,ch_score,5);
			memset((void*)ch_score,0,sizeof(ch_score));
			sprintf(ch_score,"%d",score[1]);
			write(serv_clnt_sock,ch_score,strlen(ch_score));

			read(serv_clnt_sock,ch_score,5);
			memset((void*)ch_score,0,sizeof(ch_score));
			sprintf(ch_score,"%d",score[2]);
			write(serv_clnt_sock,ch_score,strlen(ch_score));

			continue;

			//exit(0);
		}
		else if(strcmp(flag,"2")==0);
		//else if(strcmp(flag,"3")==0)
		//{
		//	if(execl("./chat_serv","chat_serv","6767",(char*)0)<0)
		//	{
		//		fprintf(stderr,"execl() error!\n");
		//		perror("server exec()");
		//		exit(1);
		//	}
		//	printf("server after exec!\n");
		//	continue;
		//}
		else if(strcmp(flag,"4"))
			exit(0);	


		memset((void*)buf,0,sizeof(buf));
		read(serv_clnt_sock,buf,BUF_SIZE);   //when client end the game,receive the message of if client win the game
		if(strcmp(buf,"win")==0)
		{
			printf("we are win!\n");
			set_score(0,curname,id);
		}
		else if(strcmp(buf,"lose")==0)
		{
			printf("we are lose!\n");
			set_score(1,curname,id);
		}
		else if(strcmp(buf,"draw")==0)
		{
			printf("we are draw!\n");
			set_score(2,curname,id);
			printf("after set_score\n");
		}
		else
			printf("some error happened!\n");
	}
}


void init()
{
	int fd;
	char *p;
	char ch[BUF_SIZE];
	
	fd=open("map",O_RDONLY|O_CREAT,MODE);
	memset((void*)ch,0,sizeof(ch));
	read(fd,ch,BUF_SIZE);
	close(fd);
	printf("after read() ch:%s\n",ch);
	if(strcmp(ch,"")==0)
	{
		printf("file is empty\n");
		count=0;
	}
	else
	{
		printf("file is not empty\n");
		count=0;
		p=strtok(ch,",");	
		while(p!=NULL)
		{
			count++;
			p=strtok(NULL,",");
		}
	}
}


void show_score(int *score)
{
	printf("Your achievement are as follows:\n");	
	printf("win:%d\n",score[0]);
	printf("lose:%d\n",score[1]);
	printf("draw:%d\n",score[2]);
}



