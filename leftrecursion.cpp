#include <bits/stdc++.h>
using namespace std;


int main(int argc, char* argv[]){
    if(argc!=2){
        cout << "One filename argument is expected" << endl;
        return 0;
    }
    
    string filename=argv[1];
    ifstream filebuf;
    filebuf.open(filename);
    if(!filebuf.is_open()){
        cout << "File does not exist" << endl;
        return 0;
    }
    string line;

    map <string, vector<string>> productions;
    while(getline(filebuf,line)){
        if(line[0]=='#'){
            continue;
        }
        stringstream ss;
        ss << line;
        string word;
        line = "";
        while (!ss.eof()) {
            ss >> word;
            line = line + word;
        }
        cout << line << endl;
        string lhs = string(1, line[0]);
    } 
    filebuf.close();
    return 0;
}