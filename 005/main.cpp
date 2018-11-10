#include <iostream>
#include "parse_pathname.h"
using namespace std;

int main(void)
{
	string path="/usr/lib";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path="/usr/";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path="usr";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path="/";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path=".";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path="..";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	path="usr/*";
	cout<<"------"<<path<<"------"<<endl;
	cout<<dirname(path)<<endl;
	cout<<basename(path)<<endl;
	return 0;
}
