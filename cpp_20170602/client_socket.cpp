#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

//#define SERVER_IP "192.168.9.45"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4444

void *onread(void *args){
	int conn_fd = (int)(*((int*)args));
	struct sockaddr_in cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char msg[1024];
	while (true) {
		bool finish = false;
		while (!finish) {
			int recv_len = recv(conn_fd,msg,sizeof(msg),0);
			//-1- 异常
			if (recv_len < 0) {
				cout<<"recv error!"<<endl;
				if(errno == EAGAIN) {
					break;
				}
				pthread_exit(NULL);
			} else if(recv_len == 0) {
				//-2- 对端关闭
				cout<<"peer dis-connnect fd="<<conn_fd<<endl;
				close(conn_fd);
			}

			//-3- 判断是否读完
			if (recv_len != sizeof(msg)) {
				finish = true;
				cout<<"recv msg from server="<<conn_fd<<":"<<msg<<endl;
			} else {
				cout<<"recv unfinish,continue to read!"<<endl;
			}
		}
	}
}

void *onwrite(void *args) {
	int conn_fd = (int)(*((int*)args));
	while(1) {
		char msg_to_write[1024];
		cout<<"input your msg_to_send:\n";
		cin >> msg_to_write;
		int ret = send(conn_fd,msg_to_write,strlen(msg_to_write),0);	
		if (ret <= 0) {
			cout<<"write error"<<endl;
			pthread_exit(NULL);
		}
		cout<<"write msg success("<<conn_fd<<") len="<<ret<<endl;
	}
}

int main(int argc,char *argv[]) 
{
	cout<<"##I am Client##"<<endl;
	pthread_attr_t attr;
	pthread_t p_id;
	//--/usr/include/netinet/in.h
	struct sockaddr_in *myaddr = new sockaddr_in;
	myaddr = new sockaddr_in;
	myaddr->sin_family=AF_INET;
	myaddr->sin_port=htons(SERVER_PORT);
	myaddr->sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,SERVER_IP,(void *)&(myaddr->sin_addr));

	socklen_t s_len = sizeof(struct sockaddr_in);
	//tcp
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		cout<<"create socket error!"<<endl;
		return -1;
	}

	int ret = connect(fd, (struct sockaddr *)myaddr, s_len);
	if (ret != 0) {
		cout<<"connect error:"<<strerror(errno)<<endl;
		cout<<"connect error:"<<errno<<endl;
		return -1;
	}

	pthread_t pid_1,pid_2;
	pthread_create(&pid_1, NULL, onwrite, &fd);
	pthread_create(&pid_2, NULL, onread, &fd);


	//--等待线程结束
	void *s1 = NULL;
	void *s2 = NULL;
	pthread_join(pid_1,&s1);
	cout<<"Thread 1 returns:"<<(char *)s1<<endl;
	pthread_join(pid_2,&s2);
	cout<<"Thread 2 returns:"<<(char *)s2<<endl;

	//destory
	close(fd);
	delete myaddr;

	return 0;
}
