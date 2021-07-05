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

size_t productioncount = 0;

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

int getIndexForNonTerminallist(char lhs, vector<char> lhslist) {
    for (size_t i = 0; i < lhslist.size(); i++)
    {
        if (lhslist[i] == lhs) {
            return i;
        }
    }
    return -1;
}

int getIndexForTerminal(char terminal, vector<char> terminals) {
    for (size_t i = 0; i < terminals.size(); i++)
    {
        if (terminals[i]== terminal) {
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
        if (!isupper(rhs[i]) && rhs[i]!='@') {
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
            if (!isupper(alpha[i])) {
                firstterminals.push_back(alpha[i]);
                break;
            }
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



void follow(char X){
  // Index of the production X in non-terminals details
  int productionindex = getIndexForLHS(X);
  // cout << "X is: " << X << endl;
  if(!nt[productionindex].follow.empty()){
    cout << "Yes"<< endl;
    return;
  }
  if(productionindex == 0){
    nt[0].follow.push_back('$');
  }
  for (size_t i = 0; i < productioncount; i++) {
    // Find X in RHS of all productions.
    for (size_t j = 0; j < nt[i].rhs.size(); j++) {
      size_t xpos=nt[i].rhs[j].find(X);
      // If X is found
      if(xpos != std::string::npos){
        int kflag=0;
        // If X is not last position
        if(xpos != nt[i].rhs[j].size()-1){
          size_t k;
          for ( k = xpos+1; k < nt[i].rhs[j].size(); k++) {
            cout <<"X is"<< X << "k is: "<< k << endl;
            //cout << "What is after "<< X <<"? " << nt[i].rhs[j][k] << endl;
            if(!isupper(nt[i].rhs[j][k])){
              nt[productionindex].follow.push_back(nt[i].rhs[j][k]);
              break;
            }else{
              int nonterminalindex = getIndexForLHS(nt[i].rhs[j][k]);
              if(nt[nonterminalindex].first.empty()){
                firstx(nt[nonterminalindex].lhs);
              }
              vector<char> firstterminals = nt[nonterminalindex].first;
              int flagepsilon=0;
              cout << "First of " << nt[i].rhs[j][k] << ": ";
              for (size_t l = 0; l < firstterminals.size(); l++) {
                cout << firstterminals[l] << " ";
                int flag=0;
                if(firstterminals[l]=='@'){
                  flagepsilon=1;
                  continue;
                }
                for (size_t m = 0; m < nt[productionindex].follow.size(); m++) {
                  if(firstterminals[l]==nt[productionindex].follow[m]){
                    flag=1;
                  }
                }
                if(flag==0 && firstterminals[l]!='@'){
                  nt[productionindex].follow.push_back(firstterminals[l]);
                }
              }
              cout << endl;
              if(flagepsilon==0){
                break;
              }
            }
          }
          cout <<"X is"<< X << " "<< "k is: "<< k << endl;
          if(k==nt[i].rhs[j].size()){
            cout << "k is " << k << endl;
            kflag=1;
          }
        }
        if( xpos == nt[i].rhs[j].size()-1 || kflag==1 ){
          cout <<"kflag is "<< kflag << endl;
          if(nt[i].follow.empty()){
            follow(nt[i].lhs);
          }
          vector<char> followterminals = nt[i].follow;
          for (size_t n = 0; n < followterminals.size(); n++) {
            int flag =0;
            for (size_t m = 0; m < nt[productionindex].follow.size(); m++) {
              if(followterminals[n]==nt[productionindex].follow[m]){
                flag=1;
              }
            }
            if(flag==0){
              nt[productionindex].follow.push_back(followterminals[n]);
            }
          }
        }
      }
    }
  }
  cout << "The follow for "<< X << ": ";
  for (size_t i = 0; i < nt[productionindex].follow.size(); i++) {
    cout << nt[productionindex].follow[i] << " ";
  }
  cout << endl;
}

void constructPredictiveParsingTable(string (&parsingtable)[100][100], vector<char> lhslist, vector<char> terminals) {
  for (size_t x = 0; x < lhslist.size(); x++) {
    for (size_t y = 0; y < terminals.size(); y++) {
        parsingtable[x][y]=" ";
    }
  }

  for (size_t i = 0; i < productioncount; i++)
  {
      int ntindex = getIndexForNonTerminallist(nt[i].lhs, lhslist);
      for (size_t j = 0; j < nt[i].rhs.size(); j++)
      {
          string production;
          vector<char> firstalpha=first(nt[i].rhs[j]);
          production.push_back(nt[i].lhs);
          production.append("->");
          production.append(nt[i].rhs[j]);
          int flagepsilon=0;
          for (size_t k = 0; k < firstalpha.size() ; k++) {
            if(firstalpha[k]!='@'){
              int terminalindex = getIndexForTerminal(firstalpha[k],terminals);
              parsingtable[ntindex][terminalindex]=production;
            }else{
              flagepsilon=1;
            }
          }
          if(flagepsilon==1){
            if(nt[i].follow.empty()){
              follow(nt[i].lhs);
            }
            vector<char> followterminals = nt[i].follow;
            // for (size_t d = 0; d < followterminals.size(); d++) {
            //   cout<< "Follow list: " << followterminals[d] << " ";
            // }
            // cout << endl;
            for (size_t l = 0; l < followterminals.size(); l++) {
              int terminalindex = getIndexForTerminal(followterminals[l], terminals);
              if(parsingtable[ntindex][terminalindex]!=" "){
                cout << "The grammar is not LL(1)" << endl;
                exit(0);
              }
              parsingtable[ntindex][terminalindex]=production;
            }
          }
      }
  }

  cout << "The parsing table is: " << endl;
  cout << "\t";
  for (size_t i = 0; i < terminals.size(); i++) {
    cout << terminals[i] << "\t";
  }
  cout << endl;
  for (size_t x = 0; x < lhslist.size(); x++) {
    cout << lhslist[x] << "\t";
    for (size_t y = 0; y < terminals.size(); y++) {
        cout << parsingtable[x][y] << "\t";
    }
    cout << endl;
  }
}


// Function that parses the given string input.
int ParseString(string s, string (&parsingtable)[100][100], vector<char> lhslist, std::vector<char> terminals){
    vector<char> stack;
    s.append("$");
    stack.push_back('$');
    stack.push_back(lhslist[0]);
    cout << "Parsing string: " << s << endl;
    printf("Stack top :- %c\n", stack.back());

    int index=0;
    char a = s[index];
    if(isupper(a)){
        return 0;
    }

    while(1){
        if(stack.back() == a && a == '$'){
            return 1;
        }
        if(stack.back() == a){
            stack.pop_back();
            index++;
            a = s[index];
            if(isupper(a)){
                return 0;
            }
        }
        else{
            if(isupper(stack.back())){
                int i = getIndexForNonTerminallist(stack.back(), lhslist);
                int j = getIndexForTerminal(a, terminals);
                if(parsingtable[i][j] == " "){
                    return 0;
                }else{
                    stack.pop_back();
                    string prod = parsingtable[i][j];
                    prod.erase(0,3);
                    for(int k=prod.size()-1 ; k>=0; k--){
                        if(prod[k] != '@'){
                            stack.push_back(prod[k]);
                        }
                    }

                }
            }
            else if(stack.back() == '$' && a!='$'){
                return 0;
            }
        }
        // Printing the stack to know the progress of the parser.
        cout << "Stack contains: " << endl;
        for (size_t i = 0; i < stack.size(); i++) {
          cout << stack[i] << '\n';
        }
	}
    return 1;
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
        // cout << line << endl;
        while(!line.empty()){
            // Find the first "|" in the string
            size_t pipepos=line.find("|");
            // If no "|", then break
            if(pipepos==std::string::npos){
                break;
            }
            string rhs;
            // cout << "LHS: " << lhs;
            // RHS is from beginning till "|"
            rhs = line.substr(0,pipepos);
            // cout << " RHS: " << rhs << endl;
            nt[productioncount].rhs.push_back(rhs);
            terminals = addTerminals(terminals, rhs);
            // Remove the first RHS part including the pipe "|" from the line
            line.erase(0,pipepos+1);
        }
        nt[productioncount].rhs.push_back(line);
        terminals = addTerminals(terminals, line);
        productioncount++;
    }
    terminals.push_back('$');
    printProductions();
    string parsingtable[100][100];
    constructPredictiveParsingTable(parsingtable, lhslist, terminals);
    string input;
    cout << "Enter any input string: ";
    cin >> input;
    if(ParseString(input, parsingtable, lhslist, terminals)){
      cout << "String has been accepted" << endl;
    }else{
      cout << "String could not be accepted" << endl;
    }

    filebuf.close();
    return 0;
}
