#include <string.h>
#include <iostream>
using namespace std;
string dirname(string path)
{
	size_t location = path.find_last_of('/');
	if (path.size() == location + 1){
		return path.substr(0, path.find_last_of('/', location-1)+1);
	}
	if (string::npos == location){
		return ".";
	}
	return path.substr(0, path.find_last_of('/'));
}
string basename(string path)
{
	size_t location = path.find_last_of('/');
	if (0 == location && 1 == path.size()){
		return "/";
	}
	if (path.size() == location + 1){
		return path.substr(path.find_last_of('/', location-1)+1, location-1);
	}
	return path.substr(path.find_last_of('/')+1);
}
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
