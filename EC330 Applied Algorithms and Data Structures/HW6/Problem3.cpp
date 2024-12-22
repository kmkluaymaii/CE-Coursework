//Rawisara Chairat, rawisara@bu.edu
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cstring>
#include <utility>

#include "Problem3.h"
using namespace std;

class TrieNode {
public:
    TrieNode* child[52];
    bool wordEnd;

    TrieNode();
};

class Trie {
public:
    TrieNode* root;

    Trie();
    void insert(const std::string& key);
    pair<bool, bool> search(const std::string& key);
};


TrieNode::TrieNode() {
    wordEnd = false;
    for (int i = 0; i < 52; i++) {
        child[i] = nullptr;
    }
}

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& key) {
    TrieNode* curr = root;
    for (char c : key) {
        int index;
        if (islower(c)) index = c - 'a';
        else index = c - 'A' + 26;
        if (curr->child[index] == nullptr) {
            curr->child[index] = new TrieNode();
        }
        curr = curr->child[index];
    }
    curr->wordEnd = true;
}

pair<bool, bool> Trie::search(const std::string& key) {
        TrieNode* curr = root;
        bool isLeaf = true;

        for (char c : key) {
            int index;
            if (islower(c)) index = c - 'a';
            else index = c - 'A' + 26;
            if (curr->child[index] == nullptr) {
                return {false, false};
            }
            curr = curr->child[index];
        }

        for (int i = 0; i < 52; i++) {
            if (curr->child[i] != nullptr) {
                isLeaf = false;
                break;
            }
        }
    return {curr->wordEnd, isLeaf && curr->wordEnd};
}


int threeA(string fileName){
    ifstream input;
    input.open(fileName);
    string word;

    int max_length = 0;

    if (!input) {
        return -1;
    }

    while (input >> word) {
        max_length = max(max_length, (int)word.length());
    }

    input.close();
    return max_length;
}

int threeB(string fileName) {
    ifstream input;
    input.open(fileName);
    int password = 0;

    if (!input) {
        return -1;
    }

    enum State {
        start, 
        end
    };
    State state = end;

    bool Lower = false;
    bool Upper = false;
    bool Digit = false;

    string current;
    
    char c;
    while (input.get(c)) {
        if (isspace(c)) {
            if (state == start) {
                if (current.length() >= 8 && current.length() <= 20 && Lower && Upper && Digit) {
                    password++;
                }
                current.clear();
                Lower = false;
                Upper = false;
                Digit = false;
                state = end;
            }
        } else {
            current += c;
            if (islower(c)) Lower = true;
            if (isupper(c)) Upper = true;
            if (isdigit(c)) Digit = true;
            state = start;
        }
    }

    if (!current.empty() && current.length() >= 8 && current.length() <= 20 && Lower && Upper && Digit) {
        password++;
    }

    input.close();
    return password;
}



int threeC(string fileName){
    ifstream dict;
    dict.open("dictionary.txt");
    string dict_word;
    char candy[] = {'s', 'p', 'k'};
    Trie trie;
    int count = 0;
    while(getline(dict, dict_word)){
        int find = 0;
        for(char c : candy){
            char upper_c = toupper(c);
            if(dict_word.find(c) < 100000 || dict_word.find(upper_c) < 100000){
                find++;
            }
        }
        if(find < 3){
            trie.insert(dict_word);
        }
    }
    
    ifstream input;
    input.open(fileName);
    string word;
    string current;

    while (input >> word) {
        for (size_t start = 0; start < word.length(); start++) {
            for (size_t length = 1; length <= word.length() - start; length++) {
                current = word.substr(start, length);
                if (all_of(current.begin(), current.end(), ::isalpha)) {
                    if(trie.search(current).first == 1){
                        count++;
                    }
                    if(trie.search(current).second == 1){
                        break;
                    }
                } else {
                    break;
                }
            }
        }   
    }

    input.close();
    dict.close();
    return count;
}

