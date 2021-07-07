#include<iostream>
#include<fstream>
#include<string>
#include<cstdbool>
#include<vector>
#include<cctype>
#include<map>
using namespace std;

string keywordslist[33] = {"auto", "bool", "char", "const", "define", "double", "enum",
                    "extern", "float", "int", "FILE", "NULL", "EOF",
                    "long", "register", "short", "static", "struct",
                    "typedef","union", "unsigned","void", "if", "else",
                    "while", "do", "break", "continue", "return", "case", "sizeof",
                    "switch", "goto"};

char delimiters[24] = { ' ', '+', '-', '*', '/', ',', ';', '>', '<', '=', '(', ')',
                    '[', ']', '{', '}', '.', '\t', '|', '&', '\n', '\r', '\\', '!' };

/* Checks if keyword*/
bool checkIfKeyword(string str){
  for (size_t i = 0; i < 33; i++) {
    if(str.compare(keywordslist[i])==0){
      return 1;
    }
  }
  return 0;
}

bool checkIfDelimiter(char c){
  for (size_t i = 0; i < 24; i++) {
    if(c==delimiters[i]){
      return 1;
    }
  }
  return 0;
}

bool checkIfInteger(string s){
  if(s.length()>0){
    for (size_t i = 0; i < s.length(); i++) {
      if(!isdigit(s[i]) || s[i]!='-'){
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

bool checkIfDecimal(string s){
  bool hasDecimal=0;
  if(s.length()>0){
    for (size_t i = 0; i < s.length(); i++) {
      if(!isdigit(s[i]) || s[i]!='-' || s[i]!='.'){
        return 0;
      }
      if(s[i]=='.'){
        hasDecimal=1;
      }
    }
  }
  return hasDecimal;
}

bool checkInQuotes(string str) {
	int len = str.length();
	if (str[0] == '"' && str[len-1] == '"')
		return 1;
  if (str[0] == '\'' && str[len-1] == '\'')
		return 1;
	return 0;
}

bool checkIfIdentifier(string name){
  if(name.length()==0){
    return 0;
  }
  if(isdigit(name[0]) || checkIfDelimiter(name[0])){
    return 0;
  }
  for(size_t i=1; i<name.length(); i++){
    if(!isalnum(name[i]) && name[i]!='_'){
      return 0;
    }
  }

  return 1;
}

map<string, vector<int>> findNames(string str, int linenumber, map<string, vector<int>> nameslist) {
   int left = 0, right = 0;
   int length = str.length();
   while (right < length && left <= right) {
      if (checkIfDelimiter(str[right]) == false)
      right++;
      if (checkIfDelimiter(str[right]) == true && left == right) {
         right++;
         left = right;
      } else if (((checkIfDelimiter(str[right]) == true) && (left != right)) || (right == length && left != right)) {
         string subStr = str.substr(left, right-left);
         if (!checkIfKeyword(subStr) && !checkIfInteger(subStr) && !checkIfDecimal(subStr)  && !checkInQuotes(subStr)){
           if (checkIfIdentifier(subStr) == true && checkIfDelimiter(str[right - 1]) == false &&subStr.length() > 0){
            cout << "Line number: "<< linenumber << " Valid Name: " << subStr << endl;
            nameslist[subStr].push_back(linenumber);
            }
            else if (checkIfIdentifier(subStr) == false && checkIfDelimiter(str[right - 1]) == false &&subStr.length() > 0)
            cout << "Line number: " << linenumber<< " Invalid Name: " << subStr << endl;
            left = right;
         }else{
           left = right;
         }
      }
   }
   return nameslist;
}

//string removeCommentsFromLine

/* DAsh bufallo
 AShskdhkc
 Nonsense */
int main(int argc, char* argv[]){
    if(argc!=2){
        cout << "One argument is expected" << endl;
        return 0;
    }
    /* Try removing commennts from this file
    It doesn't work */
    string filename=argv[1];
    if(filename[filename.length()-1] != 'c' && filename[filename.length()-1] != '.'){
        cout << "File should have .c extension" << endl;
        return 0;
    }
    // hfgdskhfl
    ifstream filebuf;
    filebuf.open(filename);
    if(!filebuf.is_open()){
        cout << "File does not exist" << endl;
        return 0;
    }
    int linecount=0; // hfkdyifyeo idhc
    string line;
    cout << "Lines which contain names: " << endl;
    map<string, vector<int>> nameslist;
    bool comment=0;
    while(getline(filebuf,line)){
        string line2;
        linecount++;
        if(line.empty() || line[0] == '#' || line[0] == '\r' || line[0] == '\n'){
          continue;
        }
        // Removing strings which are comments and within double quotes.
        for(size_t j=0; j < line.size();j++){
          if(!comment && line[j]=='"'){
            j++;
            while(line[j]!='"'){
              j++;
            }
            j++;
          }
          if (!comment && j+1 < line.size() && line[j] == '/' && line[j+1] == '/') {
            break;
          }
          else if (!comment && j+1 < line.size() && line[j] == '/' && line[j+1] == '*'){
            comment = 1;
            j++;
          }
          else if (comment && j+1 < line.size() && line[j] == '*' && line[j+1] == '/') {
            comment = 0;
            j++;
          }
          else if (!comment) {
            line2.push_back(line[j]);
          }
        }
        nameslist = findNames (line2,linecount, nameslist);
    } /* HA */
    for (const auto &variable: nameslist) {
      cout << variable.first << ": ";
      for (size_t i = 0; i < variable.second.size(); i++) {
        cout << variable.second[i] << " ";
      }
      cout << endl;
    }

    filebuf.close();
    return 0;
}
