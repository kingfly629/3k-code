//--client.cpp
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

using std::cout;
using std::cin;
using std::endl;

//#define SERVER_IP "127.0.0.1"
//#define HOST "King"
#define SERVER_PORT 4444

int getIpbyHostName(char *ip)
{
	char   **pptr;
	struct hostent *hptr;
	char   str[32];

	char hostname[64];
	if (gethostname(hostname,sizeof(hostname)) != 0) {
		printf(" gethostname error for host:%s\n", hostname);
		return -1;
	}

	if((hptr = gethostbyname(hostname)) == NULL)
	{
		printf(" gethostbyname error for host:%s\n", hostname);
		return -2;
	}

	printf("official hostname:%s\n",hptr->h_name);
	for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
		printf(" alias:%s\n",*pptr);

	switch(hptr->h_addrtype)
	{
		case AF_INET:
		case AF_INET6:
			pptr=hptr->h_addr_list;
			for(; *pptr!=NULL; pptr++)
				printf(" address:%s\n",
						inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			ip = inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
			printf(" first address: %s\n",ip);
			break;
		default:
			printf("unknown address type\n");
			break;
	}

	return 0;
}

void *onread(void *args){
	int conn_fd = (int)(*((int*)args));
	struct sockaddr_in cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char msg[10];
	while (true) {
		bool finish = false;
		std::string msg_from_server;
		while (!finish) {
			memset(msg,0x0,sizeof(msg));
			//block default
			//non-block
			//when no data EAGAIN occur
			int recv_len = recv(conn_fd,msg,sizeof(msg),MSG_DONTWAIT);
			cout<<"recv msg from server len="<<recv_len<<endl;
			//-1- 异常
			if (recv_len < 0) {
				cout<<"recv error:"<<strerror(errno)<<endl;
				//EAGAIN for non-block recv
				/*if(errno == EAGAIN) {
				  break;
				  }*/
				close(conn_fd);
				exit(1);
			} else if(recv_len == 0) {
				//-2- 对端关闭
				cout<<"peer dis-connect fd="<<conn_fd<<endl;
				//for CLOSE_WAIT
				close(conn_fd);
				exit(2);
			}

			//-3- 判断是否读完
			if (recv_len != sizeof(msg)) {
				cout<<"recv last msg from server("<<conn_fd<<"):"<<msg<<endl;
				msg_from_server += msg;
				cout<<"recv msg finish from server("<<conn_fd<<"):"<<msg_from_server<<endl;
				finish = true;
			} else {
				cout<<"recv unfinish("<<msg<<"),continue to read!"<<endl;
				msg_from_server += msg;
			}
		}
	}
}

void *onwrite(void *args) {
	int conn_fd = (int)(*((int*)args));
	char head_len[8];
	while(1) {
		char msg[1024];
		cout<<"input the msg to send(1024 bytes max):\n";
		cin>>msg;

		//包头
		/*sprintf(head_len,"%d",strlen(msg));
		  int ret = send(conn_fd,head_len,strlen(head_len),0);	
		  if (ret <= 0) {
		  cout<<"write head error"<<endl;
		  exit(3);
		  }
		  cout<<"write msg head success("<<conn_fd<<") head="<<head_len<<",len="<<ret<<endl;
		  */
		//包体
		int ret = send(conn_fd,msg,strlen(msg),0);	
		if (ret <= 0) {
			cout<<"write write error"<<endl;
			exit(3);
		}
		cout<<"write msg body success("<<conn_fd<<") msg="<<msg<<",len="<<ret<<endl;

	}
}

void onconnect() {
	//--/usr/include/netinet/in.h
	struct sockaddr_in *myaddr = new sockaddr_in;
	myaddr->sin_family=AF_INET;
	myaddr->sin_port=htons(SERVER_PORT);
	myaddr->sin_addr.s_addr = INADDR_ANY;
	//inet_pton(AF_INET,SERVER_IP,(void *)&(myaddr->sin_addr));
	char server_ip[32];
	if (getIpbyHostName(server_ip) != 0) {
		cout<<"getIpbyHostName fail!"<<endl;
		return ;
	}
	inet_pton(AF_INET,server_ip,(void *)&(myaddr->sin_addr));

	//tcp
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		cout<<"create socket error!"<<endl;
		return ;
	}
	socklen_t s_len = sizeof(struct sockaddr_in);
	while (true) {
		int ret = connect(fd, (struct sockaddr *)myaddr, s_len);
		if (ret != 0) {
			cout<<"connect fail:"<<strerror(errno)<<endl;
			sleep(10);
			cout<<"reconnect..."<<endl;
			continue;
		}

		cout <<"connect succ on fd="<<fd<<endl;
		break;
	}

	pthread_t pid_1,pid_2;
	pthread_create(&pid_1, NULL, onwrite, &fd);
	pthread_create(&pid_2, NULL, onread, &fd);

	//--等待线程结束
	void *s1 = NULL;
	void *s2 = NULL;
	pthread_join(pid_1,&s1);
	cout<<"Thread 1 returns:"<<(char *)s1<<endl;
	pthread_cancel(pid_2);
	pthread_join(pid_2,&s2);
	cout<<"Thread 2 returns:"<<(char *)s2<<endl;

	//close
	close(fd);
	delete myaddr;
}


int main(int argc,char *argv[]) 
{
	cout<<"##I am Client##"<<endl;

	pid_t pid;
	if ((pid = fork()) > 0) {
		cout <<"in parent ...\n";
	} else if(pid == 0) {
		cout <<"in child ...\n";
		onconnect();
	} else {
		cout <<"fork error ...\n";
		return -1;
	}

	//signal(SIGCHLD, sig_handle);

	int status = -1;
	while((pid = waitpid(-1,&status,0)) >=0) {       
		if (WIFEXITED(status)) {
			WEXITSTATUS(status);
		}
		cout<<"catch signal SIGCHLD pid="<<pid<<",exit_status="<<status<<endl;
		pid_t a;
		a =fork();
		if (a==0) {
			cout<<"fork a new child..."<<endl;
			onconnect();
			sleep(5);
		}

	}


	return 0;
}
