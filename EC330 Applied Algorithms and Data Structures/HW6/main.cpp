#include <iostream>

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <unordered_set>
#include <vector>

#include "Problem3.h"
#include "Trie.h"

using namespace std;

int main()
{
  cout << "The length of the longest word in the file is: " << threeA("TinyData.txt") << endl;
  cout << "The number of valid passwords in the file is: " << threeB("TinyData.txt") << endl;
  cout << "Number of valid dictionary words that don't contain your Halloween candies abbreviations: " << threeC("TinyData.txt") << endl;
}