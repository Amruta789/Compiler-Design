#include <bits/stdc++.h>
using namespace std;


vector<string> outputlhslist;
void printOutput(map<string, vector<string>> output){
    for(size_t i=0; i<outputlhslist.size(); i++){
        for(size_t j=0; j<output[outputlhslist[i]].size(); j++){
            cout << outputlhslist[i] << " -> " << output[outputlhslist[i]][j] << endl;
        }
    }
}

auto addTerminals(vector<char> terminals, string rhs){
    int flag=0;
    for (size_t i = 0; i < rhs.length(); i++)
    {
        if(!isupper(rhs[i])){
            for (size_t j = 0; j < terminals.size(); j++){
                if(rhs[i]==terminals[j]){
                    flag=1;
                }
            }
            if(flag==0){
                terminals.push_back(rhs[i]);
            }
        } 
    }
    return terminals;
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
    // A map to store the productions
    map <string, vector<string>> productions;
    // A vector to create the ordered list of LHS
    vector<string> lhslist;
    vector<char> terminals;
    while(getline(filebuf,line)){
        // Ignore comments in input file, they start with '#'
        if(line[0]=='#'){
            continue;
        }
        // Eliminate spaces in each line
        stringstream ss;
        ss << line;
        string word;
        line = "";
        while (!ss.eof()) {
            ss >> word;
            line = line + word;
        }
        if(line.empty()){
            continue;
        }
        //cout << line << endl;
        // Get LHS from line and add to list
        string lhs = string(1, line[0]);
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
            // RHS is from beginning till "|"
            rhs = line.substr(0,pipepos);
            // Add the production to the map
            productions[lhs].push_back(rhs);
            terminals = addTerminals(terminals, rhs);
            // Remove the first RHS part including the pipe "|" from the line
            line.erase(0,pipepos+1);
        }
        productions[lhs].push_back(line);
        cout << "The productions are: " << endl;
        for(size_t i=0; i<lhslist.size(); i++){
            for(size_t j=0; j<productions[lhslist[i]].size(); j++){
                cout << lhslist[i] << " -> " << productions[lhslist[i]][j] << endl;
            }
        }
    } 
    string input;
    cout << "Enter any input string: ";
    cin >> input;
    
    filebuf.close();
    return 0;
}