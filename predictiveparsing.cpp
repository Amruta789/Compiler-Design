#include <iostream>
#include<vector>
#include<string>
#include<cctype.h>
#include<fstream>
#include<sstream>
using namespace std;

struct NonTerminalDetails {
    char lhs;
    string rhs;
    vector<char> first;
    vector<char> follow;
}nt[30];

void printProductions(int productioncount) {
    for (size_t i = 0; i < productioncount; i++)
    {
        cout << nt[i].lhs << " -> " << nt[i].rhs << endl;
    }

}



auto addTerminals(vector<char> terminals, string rhs) {
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

void firstx(char X) {

}


auto first(string alpha) {
    vector<char> firstterminals;
    firstterminals = addTerminals(firstterminals, alpha);
    for (size_t i = 0; i < alpha.length(); i++)
    {
        if (isupper(alpha[i])) {
            for (size_t i = 0; i < nt.size(); i++)
            {
                if (nt.lhs == alpha[i]) {
                    for (size_t i = 0; i < nt.first.size(); i++)
                    {

                    }
                }
            }
        }
    }
    return firstterminals;
}

auto constructPredictiveParsingTable(string parsingtable[100][100]) {

    return parsingtable;
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
    int productioncount=0;
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
        // Remove the "S->" or "A->" part of the line
        line.erase(0,3);
        //cout << line << endl;
        while(!line.empty()){
            // Find the first "|" in the string
            size_t pipepos=line.find("|");
            // If no "|", then break
            if(pipepos==std::string::npos){
                break;
            }
            string rhs;
            nt[productioncount].lhs = lhs;
            // RHS is from beginning till "|"
            rhs = line.substr(0,pipepos);
            nt[productioncount].rhs = rhs;
            productioncount++;
            terminals = addTerminals(terminals, rhs);
            // Remove the first RHS part including the pipe "|" from the line
            line.erase(0,pipepos+1);
        } 
    } 
    printProductions(productioncount);
    string parsingtable[100][100];
    parsingtable = constructPredictiveParsingTable(parsingtable);
    string input;
    cout << "Enter any input string: ";
    cin >> input;
    
    filebuf.close();
    return 0;
}