#include <iostream>
#include<vector>
#include<string>
#include<cctype>
#include<fstream>
#include<sstream>
#include<stdbool.h>
using namespace std;

struct NonTerminalDetails {
    char lhs;
    vector<string> rhs;
    vector<char> first;
    vector<char> follow;
}nt[30];

int productioncount = 0;

void printProductions() {
    for (size_t i = 0; i < productioncount; i++)
    {
        for (size_t j = 0; j < nt[i].rhs.size(); j++)
        {
            cout << nt[i].lhs << " -> " << nt[i].rhs[j] << endl;
        }
    }

}

int getIndexForLHS(char lhs) {
    for (size_t i = 0; i < productioncount; i++)
    {
        if (nt[i].lhs == lhs) {
            return i;
        }
    }
    return -1;
}

int indexIfEpsilonPresent(vector<char> firstterminals) {
    for (size_t i = 0; i < firstterminals.size(); i++)
    {
        if (firstterminals[i] == '@') {
            return i;
        }
    }
    return -1;
}

vector<char> addTerminals(vector<char> terminals, string rhs) {
    int flag = 0;
    for (size_t i = 0; i < rhs.length(); i++)
    {
        if (!isupper(rhs[i])) {
            for (size_t j = 0; j < terminals.size(); j++) {
                if (rhs[i] == terminals[j]) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                terminals.push_back(rhs[i]);
            }
        }
    }
    return terminals;
}

vector<char> first(string alpha);

void firstx(char X) {
  int productionindex = getIndexForLHS(X);
  for (size_t i = 0; i < nt[productionindex].rhs.size(); i++) {
    vector<char> firstterminals;
    // Calculate first() for each RHS.
    firstterminals = first(nt[productionindex].rhs[i]);
    for (size_t j = 0; j < firstterminals.size(); j++) {
      int flag=0;
      // Check if any of the terminals in firstterminals list are already present in first(X), then don't add.
      for (size_t k = 0; k < nt[productionindex].first.size(); k++) {
        if(nt[productionindex].first[k]==firstterminals[j]){
          flag=1;
        }
      }
      // Add whatever is in firstterminals as long as it is not already in first(X)
      if(flag==0){
        nt[productionindex].first.push_back(firstterminals[j]);
      }

    }
  }

}


vector<char> first(string alpha) {
    vector<char> firstterminals;
    // If first character is a terminal, it can even be '@', it is added to the array, and then returned directly.
    if (!isupper(alpha[0])) {
        firstterminals.push_back(alpha[0]);
    }
    else {
        // When first character is non-terminal
        size_t i;
        for ( i = 0; i < alpha.size(); i++)
        {
            int productionindex = getIndexForLHS(alpha[i]);
            // Find first(X) where X is the first non-terminal in RHS
            if (nt[productionindex].first.empty()) {
                firstx(alpha[i]);
            }
            // If first(X) is known
            int flag=0;
              for (size_t i = 0; i < nt[productionindex].first.size();i++){
                if(nt[productionindex].first[i]!='@'){
                  firstterminals.push_back(nt[productionindex].first[i]);
                }else{
                  // If epsilon is present in first(X)
                  flag=1;
                }
              }
            // If epsilon is not present in first(X), break the loop, return firstterminals
            if(flag==0){
              break;
            }
        }
        // If loop was broken when last character was reached, then that means epsilon shoould be added.
        if (i == alpha.size()) {
            firstterminals.push_back('@');
        }
    }
    return firstterminals;
}

void constructPredictiveParsingTable(string (&parsingtable)[100][100]) {


}



string eliminateSpaces(string line) {
    stringstream ss;
    ss << line;
    string word;
    line = "";
    while (!ss.eof()) {
        ss >> word;
        line = line + word;
    }
    return line;
}

int main(int argc, char* argv[]){
    // Take filename as command-line argument
    if(argc!=2){
        cout << "One filename argument is expected" << endl;
        return 0;
    }
    // Open the file after checking it exists
    string filename=argv[1];
    ifstream filebuf;
    filebuf.open(filename);
    if(!filebuf.is_open()){
        cout << "File does not exist" << endl;
        return 0;
    }
    string line;

    // A vector to create the ordered list of LHS
    vector<char> lhslist;
    // A vector to create the list of all terminals, '@' is included
    vector<char> terminals;

    while(getline(filebuf,line)){
        // Ignore comments in input file, they start with '#'
        if(line[0]=='#'){
            continue;
        }
        // Eliminate spaces in each line
        line = eliminateSpaces(line);
        if(line.empty()){
            continue;
        }
        // cout << line << endl;
        // Get LHS from line and add to list
        char lhs = line[0];
        lhslist.push_back(lhs);
        nt[productioncount].lhs = lhs;
        // Remove the "S->" or "A->" part of the line
        line.erase(0,3);
        cout << line << endl;
        while(!line.empty()){
            // Find the first "|" in the string
            size_t pipepos=line.find("|");
            // If no "|", then break
            if(pipepos==std::string::npos){
                break;
            }
            string rhs;
            cout << "LHS: " << lhs;
            // RHS is from beginning till "|"
            rhs = line.substr(0,pipepos);
            cout << " RHS: " << rhs << endl;
            nt[productioncount].rhs.push_back(rhs);
            terminals = addTerminals(terminals, rhs);
            // Remove the first RHS part including the pipe "|" from the line
            line.erase(0,pipepos+1);
        }
        nt[productioncount].rhs.push_back(line);
        productioncount++;
    }
    printProductions();
    string parsingtable[100][100];
    constructPredictiveParsingTable(parsingtable);
    string input;
    cout << "Enter any input string: ";
    cin >> input;

    filebuf.close();
    return 0;
}
