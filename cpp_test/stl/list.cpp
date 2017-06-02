//remove from vector
#include <iostream>
#include <vector>

int main ()
{
	std::vector<int> myvector ;
        std::vector<int> myvector2 ;
        myvector[] = 100;
        myvector.push_back(200);
        myvector.insert(myvector.begin(),2,2);
        myvector.insert(myvector.begin(),1);
	
        myvector2.insert(myvector2.begin(),myvector.begin(),myvector.end());

	std::cout << "myvector contains:";
	for (std::vector<int>::iterator it=myvector2.begin(); it!=myvector2.end(); ++it)
	{	std::cout << ' ' << *it;
	}
	std::cout << '\n';

	return 0;
}
