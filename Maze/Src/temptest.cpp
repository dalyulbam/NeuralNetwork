#include <iostream>
#include <vector>
using namespace std; 

extern "C" vector<int> main_test()
{
	vector<int> tv{ 0,1,10,-1,0,0 }; 
	cout << "Yes!!" << endl; 
	return tv; 
}