// Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>

using namespace std;

void MergeFiles(string file1, string file2){
    ifstream input1;
    ifstream input2;
    ofstream output;

    string word1;
    string word2;

    int count_word = 0;
    int j = 1;

    list<string> wordlist;

    input1.open(file1);
    input2.open(file2);
    output.open("output.txt", ios::out);

    if (!input1){
        cout<< "Error opening input file: " << file1 << endl;
        return ;
    } else if (!input2){
        cout<< "Error opening input file: " << file2 << endl;
        return ;
    }

    while(input1>>word1){
        wordlist.push_front(word1);
        count_word++;
    }

    while(input2>>word2){
        wordlist.push_front(word2);
        count_word++;
    }

    wordlist.sort();

    for(list<string>::iterator iter = wordlist.begin(); iter != wordlist.end(); iter++){
        if (j == count_word){
            output<<*iter; //avoid printing the last blank sentence
            break;
        }
        output<<*iter<<endl; //write the sorted word back into the output.txt file
        j++;
    }

    output.close();
    input1.close();
    input2.close();

    return;
}

int main(int argc, char** argv){
    string file1;
    string file2;
    file1 = argv[1];
    file2 = argv[2];
    MergeFiles(file1, file2);
    return 0;
}
