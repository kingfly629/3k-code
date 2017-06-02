#include <iostream>
#include <string.h>
using namespace std;

int main(void)
{
	char msg[16];
	//getline(&msg,sizeof(msg),cin);
	cin.get(msg,sizeof(msg));
	strcat(msg,"\r\n\r\n");
	cout<<"sizeof(rnrn)"<<strlen("\r\n\r\n")<<endl;
	cout<<"msg="<<msg<<",strlen(msg)="<<strlen(msg)<<endl;
	return 0;
}
