// Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <string>

using namespace std;


int main(){
    string hello = "Test HELLO";
    cout<<hello.find("HL")<<endl;
    string text = "Hello, World!";
    string sub = text.substr(7, 5); // Extract "World"
    cout<<sub<<endl;
    string str = "hi";
    cout<<str.length()<<endl;
}

