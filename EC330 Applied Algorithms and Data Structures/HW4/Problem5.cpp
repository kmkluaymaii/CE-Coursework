// Rawisara Chairat, rawisara@bu.edu

/* The function uses the findBit method of the InputArray class.
For each bit, count the time one or zero occurs. 
If the total zero is less than half of the element then set that bit to zero.
If the total one is less than half of the element then set that bit to one.
Continue doing that until the last bit. Then we should get the missing number.
The total runtime is O(nlogn) since it goes through first loop of bits of the numbers (k bits), 
and for each bit, it goes through the loop of all numbers (n numbers).
Therefore, the algorithm runs in O(n*k) time. Since k = log2(n+1), the complexity simplifies to O(n log n).*/

#include <iostream>
#include <cmath>
#include <bitset>
#include "Problem5.h"
#include "InputArray.h"
using namespace std;

int find(int k, InputArray &input, int missing){


    if (k < 0) return missing;
    int countone = 0;
    int countzero = 0;
    for(int i = 0; i < input.n; i++){
        if (input.findBit(i, k)==0) {
            countzero++;
        } else {
            countone++;
        }
    }
    if (countzero < countone) {
        cout << "Bit: " << k << " is zero" << endl;
        missing |= (0 << k);
        return find(k-1, input, missing); 
    } else {
        cout << "Bit: " << k << " is one" << endl;
        missing |= (1 << k);
        return find(k-1, input, missing); 
    }
}

int Problem5(InputArray &input){
    int n = input.n;
    int k = log2(n+1);
    int missing = 0;

    int bits[n];

    int countone = 0;
    int countzero = 0;
    for(int i = 0; i < input.n; i++){
        if (input.findBit(i, k)==0) {
            countzero++;
        } else {
            countone++;
        }
    }
    for(int i = 0; i < input.n; i++){
        if (countzero < countone){
            missing |= (0 << k);
            if (input.findBit(i, k)==0){
                bits[i] =;
            }
        }
    }

    if (countzero < countone) {
        cout << "Bit: " << k << " is zero" << endl;
        missing |= (0 << k);
        return find(k-1, input, missing); 
    } else {
        cout << "Bit: " << k << " is one" << endl;
        missing |= (1 << k);
        return find(k-1, input, missing); 
    }

    return find(k-1, input, missing);
}

// int Problem5(InputArray &input){
//     int n = input.n;
//     int k = log2(n+1);
//     int missing = 0;

//     for(int i = 0; i < k; i++){
//         int countone = 0;
//         int countzero = 0;
//         for(int j = 0; j < n ; j++){
//             if(input.findBit(j, i) == 0){
//                 countzero++;
//             } else {
//                 countone++;
//             }
//         }
//         int half = (n+1)/2;
//         if (countzero < half){
//             missing |= (0 << i);
//         } else {
//             missing |= (1 << i);
//         }
//     }

//     return missing;
// }