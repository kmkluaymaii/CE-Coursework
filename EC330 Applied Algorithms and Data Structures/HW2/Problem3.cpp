// Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int Sum(vector<int> nums, int len) {
    vector<int> sums;
    int best_sum = 0;
    int target = 330;
    int best_diff = 330;

    for(int i = 0; i < len; i++){
        if(nums[i] == target){
            return target;
        }
        sums.push_back(nums[i]);
        for(int j = i+1; j < len; j++){
            if(nums[i]+nums[j] == target){
                return target;
            }
            sums.push_back(nums[i]+nums[j]);
            for(int k = j+1; k < len; k++){
                if(nums[i]+nums[j]+nums[k] == target){
                    return target;
                }
                sums.push_back(nums[i]+nums[j]+nums[k]);
            }  
        }
    }

    for(int i = 0; i < sums.size(); i++){
        int diff = abs(330 - sums[i]);
        if(best_diff > diff){
            best_diff = diff;
            best_sum = sums[i];
        }
    }

    return best_sum;
}


