//remove from list
#include <iostream>
#include <list>

int main ()
{
	int myints[]= {17,89,7,14};
	std::list<int> mylist (myints,myints+4);

	//mylist.remove(89);

	std::cout << "mylist contains:";
	for (std::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
	{	std::cout << ' ' << *it;
		if (*it == 89)
		{ 
			//int tmp = *it;
			//mylist.remove(tmp);
			mylist.erase(it);
		}
	}
	std::cout << '\n';

	return 0;
}
