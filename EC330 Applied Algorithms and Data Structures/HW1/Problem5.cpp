// Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <string>

using namespace std;

string Analyze(string str, int size) {
    
    if (str.length() < size){
        return str;
    }

    int length = str.length() - (size - 1);
    string all_combo[length];
   
    for (int i = 0; i < length; i++){
        string cur_str = str.substr(i, size); //Get all the combo of str in the size of size
        all_combo[i] = cur_str;
    }
   
    int all_combo_counts[length];

    for (int i = 0; i < length; i++){
        all_combo_counts[i] = 0; //Initialize the array to be zero
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (all_combo[i] == all_combo[j]) {
                all_combo_counts[i] += 1; //Count the frequency of each combo appearing in the array
            }
        }  
    }

    int max_index = 0;
    for(int i = 0; i < length; i++) {
        if(all_combo_counts[i] > all_combo_counts[max_index]) {
            max_index = i; //Find the highest index
        }
    }

    return all_combo[max_index];
}

int main(int argc, char** argv){
    string str;
    int size;
    str = argv[1];
    size = stoi(argv[2]);
    cout<< Analyze (str, size)<<endl;
}

