//remove from map
#include <iostream>
#include <map>

int main ()
{
	std::map<int,int> mymap ;
        std::map<int,int> mymap2;
        mymap2[100] = 100;
        
        for (int k =0;k< 5;++k) {
            mymap[k] = k;
        }
        
        for (int k =8;k> 3;--k) {
            mymap2[k] = k;
        }
        
        mymap2.insert(std::make_pair(10,10));
	
        mymap2.insert(mymap.begin(),mymap.end());

        mymap2.erase(1);
        mymap2.erase(mymap2.begin()+2);
	std::cout << "mymap contains:";
	for (std::map<int, int>::iterator it=mymap2.begin(); it!=mymap2.end(); ++it)
	{	
		std::cout << ' ' << it->second;
	}
	std::cout << '\n';

	return 0;
}
