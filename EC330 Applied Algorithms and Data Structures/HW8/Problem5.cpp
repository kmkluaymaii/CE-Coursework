//Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <string>
#include <vector>


using namespace std;

int findMaxSubSeqLen(string stringA, string stringB){
    int len_A = stringA.length();
    int len_B = stringB.length();

    // If string is empty, the longest subsequence is zero
    if (len_A == 0 || len_B == 0){
        return 0;
    }

    // Create matrix of size len_A+1 * len_B+1
    vector<vector<int>> dp(len_A + 1, vector<int>(len_B + 1, 0));

    for (int i = 1; i <= len_A; i++){
        for (int j = 1; j <= len_B; j++){
            if(stringA[i-1] == stringB[j-1]){
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[len_A][len_B];
}

string findMaxSubSeq(string stringA, string stringB){
    int len_A = stringA.length();
    int len_B = stringB.length();

    string longest_sequence = "";

    // If string is empty, the longest subsequence is blank
    if (len_A == 0 || len_B == 0){
        return longest_sequence;
    }

    // Create matrix of size len_A+1 * len_B+1
    vector<vector<int>> dp(len_A + 1, vector<int>(len_B + 1, 0));

    for (int i = 1; i <= len_A; i++){
        for (int j = 1; j <= len_B; j++){
            if(stringA[i-1] == stringB[j-1]){
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    for (int j = 1; j <= len_B; j++){
        if(dp[len_A][j-1] < dp[len_A][j]){
            longest_sequence = longest_sequence + stringB[j-1];
        }
    }

    return longest_sequence;
}