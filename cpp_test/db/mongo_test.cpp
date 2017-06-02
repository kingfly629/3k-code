/**
 *  * Mongodb connect for c++
 *  * author kkk
 *  */

#include <iostream>
#include <mongo/client/dbclient.h>

using namespace mongo;
using namespace std;

void run(){
    DBClientConnection c;
    c.connect("localhost");
}

int main(){
    try{
        run();
        cout<<"connect ok"<<endl;
    }catch(DBException &e){
        cout<<"caught"<<e.what()<<endl;
    }
    return 0;
}
