#include "Problem5.h"
#include <iostream>

int main(int argc, char* argv[]) {
    InputArray inp = InputArray();
    int result = Problem5(inp);
    cout << "The missing number from the example array is: " << result << ".\n";
    return result;
}