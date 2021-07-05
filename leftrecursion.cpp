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

map<string,vector<string>> eliminateDirectLeftRecursion(string lhs, map<string, vector<string>> productions){
    for (size_t i = 0; i < productions[lhs].size(); i++){
        string newterminal = lhs+"\'";
        int flag=0;
        // find lhs character in RHS
        int lhspos = productions[lhs][i].find(lhs);
        cout << "Input RHS " << productions[lhs][i] << endl;
        cout << "LHS " << lhs << " position in RHS: " << lhspos << endl;
        printOutput(productions);
        if (lhspos==0){
            flag=1;
            string rhs=productions[lhs][i].substr(1)+newterminal;
            // cout << "RHS is " << rhs << endl;
            productions[newterminal].push_back(rhs);
            for (size_t j = 0; j < productions[lhs].size(); j++){
                if(productions[lhs][j].find(lhs)!=0){
                  cout << "Production is "<< lhs << "->"<< productions[lhs][j] << endl;
                  cout << "Position of newterminal " << productions[lhs][j].find(newterminal) << endl;
                  if(productions[lhs][j].find(newterminal)== std::string::npos){
                    productions[lhs][j].append(newterminal);
                  }
                }
            }
        }
        if(flag==1){
            int flagnewterminal=0;
            for (size_t i = 0; i < outputlhslist.size(); i++) {
              if(newterminal.compare(outputlhslist[i])==0){
                flagnewterminal=1;
              }
            }
            if(flagnewterminal==0){
              outputlhslist.push_back(newterminal);
              productions[newterminal].push_back("#");
            }
        }
    }
    //printOutput(productions);
    for (size_t i = 0; i < productions[lhs].size(); i++){
        // find lhs character in RHS
        int lhspos = productions[lhs][i].find(lhs);
        if (lhspos==0){
          productions[lhs].erase(productions[lhs].begin()+i);
          i--;
        }
    }
    return productions;
}

void eliminateLeftRecursion(vector<string> lhslist, map<string, vector<string>> inputproductions){
    map<string, vector<string>> output;
    output=inputproductions;
    outputlhslist=lhslist;
    for(size_t i=0; i<lhslist.size(); i++){
        for(size_t j=0; j<i; j++){
            for (size_t k = 0; k < inputproductions[lhslist[i]].size() ; k++)
            {
              // If any of the first characters in RHS of production is equal to LHS
                if(lhslist[j][0]==inputproductions[lhslist[i]][k][0]){
                    // gamma is the string after the first character in that RHS
                    string gamma = inputproductions[lhslist[i]][k].substr(1);
                    for(size_t l=0; l < inputproductions[lhslist[j]].size(); l++){
                        // Get productions of Aj
                        string delta=inputproductions[lhslist[j]][l];
                        string rhs = delta + gamma;
                        output[lhslist[i]].push_back(rhs);
                    }
                }
            }
            output=eliminateDirectLeftRecursion(lhslist[i],output);
        }
    }
    cout << "The output productions are: " << endl;
    for(size_t i=0; i<outputlhslist.size(); i++){
        for(size_t j=0; j<output[outputlhslist[i]].size(); j++){
            cout << outputlhslist[i] << " -> " << output[outputlhslist[i]][j] << endl;
        }
    }
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
    eliminateLeftRecursion(lhslist, productions);
    filebuf.close();
    return 0;
}
