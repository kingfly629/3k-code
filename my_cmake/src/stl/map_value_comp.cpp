#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  mymap['x']=4004;
  mymap['z']=2002;
  mymap['y']=3003;
  mymap['h']=1004;

  std::cout << "mymap contains:\n";

  std::pair<char,int> highest = *mymap.rbegin();          // last element

  std::map<char,int>::iterator it = mymap.begin();
  do {
    std::cout << it->first << " => " << it->second << '\n';
  } while ( mymap.value_comp()(*it++, highest) );

  return 0;
}
