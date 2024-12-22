#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Sum(vector<int> nums, int len);

int main(){
    vector<int> nums = {10,100,200,2,330};
    int len = nums.size();
    int result = Sum(nums, len);
    cout<< result << endl;
    return 0;
}
