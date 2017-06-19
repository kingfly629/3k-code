//remove from vector
#include <iostream>
#include <vector>
using namespace std;

int main() {
	//1-insert
	/*std::vector<int> myvector;
	std::vector<int> myvector2;
	myvector.push_back(200);
	myvector.insert(myvector.begin(), 2, 2);
	myvector.insert(myvector.begin(), 1);

	myvector2.insert(myvector2.begin(), myvector.begin(), myvector.end());

	std::cout << "myvector contains:";
	for (std::vector<int>::iterator it = myvector2.begin(); it != myvector2.end(); ++it) {
		std::cout << ' ' << *it;
	}
	std::cout << '\n';*/

	//2-clear+resize
	std::vector<string> v_string;
	//v_string.reserve(20);
	//std::cout << "v_string.capacity()=" << v_string.capacity() << "; v_string.size()=" << v_string.size() << std::endl;
	v_string.insert(v_string.begin(), 3, "abc");
	std::cout << "v_string.capacity()=" << v_string.capacity() << "; v_string.size()=" << v_string.size() << std::endl;
	v_string.clear();
	if (!v_string.empty()) {
		std::cout << "v_string.capacity()=" << v_string.capacity() << "; v_string.size()=" << v_string.size() << std::endl;
	}
	v_string.reserve(1);
	std::cout << "v_string.capacity()=" << v_string.capacity() << "; v_string.size()=" << v_string.size() << std::endl;
	
	std::vector<string> v_string_tmp;
	v_string.swap(v_string_tmp);
	std::cout << "v_string.capacity()=" << v_string.capacity() << "; v_string.size()=" << v_string.size() << std::endl;
	std::cout << "v_string_tmp.capacity()=" << v_string_tmp.capacity() << "; v_string_tmp.size()=" << v_string_tmp.size() << std::endl;
	return 0;
}
