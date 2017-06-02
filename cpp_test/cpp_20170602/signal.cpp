#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void ontest() {
	//cout<<"child pid="<<getpid()<<endl;
	//cout<<"---------------------"<<endl;
	sleep(1200);
	exit(0);

}

void sig_proc(int sig_num)
{
	int pid = -1;
	switch(sig_num) {
		case SIGCHLD:
			while ((pid = waitpid(-1,NULL,0)) >=0)
			{
				cout<<"----------------------"<<endl;
				cout<<"catch signal SIGCHLD:"<<pid<<endl;
				pid_t a;
				a =fork();
				if (a==0) {
					cout<<"new child pid="<<getpid()<<endl;
					ontest();
				} else if (a>0){
					cout<<"parent pid="<<getpid()<<endl;
				}

			}
			break;
		case SIGTERM:
			while ((pid = waitpid(-1,NULL,0)) >=0)
			{
				cout<<"----------------------"<<endl;
				cout<<"catch signal SIGTERM:"<<pid<<endl;
				pid_t a;
				a =fork();
				if (a==0) {
					cout<<"new child pid="<<getpid()<<endl;
					ontest();
				} else if (a>0){
					cout<<"parent pid="<<getpid()<<endl;
				}

			}

		default:
			cout<<"other signal..."<<endl;
			break;

	}

}

int main(void)
{
	pid_t pid;
	if ((pid = fork()) == 0) {
		//cout<<"child ...\n";
		ontest();	
	} else if(pid > 0) {
		//cout<<"parent ...\n";
		pid_t b = fork();
		if (b == 0)
			ontest();
	} else {
		cout<<"fork error ...\n";
		return -1;
	}
	signal(SIGCHLD,sig_proc);
	signal(SIGTERM,sig_proc);
	//signal(SIGCHLD,SIG_IGN);

	/*while((pid = waitpid(-1,NULL,0)) >=0)
	  {       
	  cout<<"catch signal SIGCHLD:"<<pid<<endl;
	  pid_t a;
	  a =fork();
	  if (a==0) {
	  cout<<"fork a new child..."<<endl;
	  ontest();
	  sleep(5);
	  }

	  }*/

	while(1)
	{
		//cout<<"waiting...\n";
		sleep(1200);
	}
	return 0;
}
