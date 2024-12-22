//
//  main.cpp
//  find maximum subsequence
//

#include "Problem5.h"

using namespace std;

// main to test findMaxSubSeq
int main()
{
  // example sequences A, B
  string a = "elo";
  string b = "bello";

  cout << "The length of the maximum subsequence is: " << findMaxSubSeqLen(a, b) << endl;
  cout << "The maximum subsequene is: " << findMaxSubSeq(a, b) << endl;

  return 0;
}
