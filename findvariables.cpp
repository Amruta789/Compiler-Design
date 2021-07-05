#include<iostream>
#include<fstream>
#include<string>
#include<cstdbool>
#include<vector>
using namespace std;

string declarationslist[19] = {"auto", "bool", "char", "define", "double", "enum",
                    "extern", "float", "int", "FILE",
                    "long", "register", "short", "static", "struct",
                    "typedef","union", "unsigned","void"};

string delimiters[] = { " ", "+", "-", "*", "/", ",", ";", ">", "<", "=", "(", ")",
                    "[", "]", "{", "}", "." };







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
    cout << "Lines which contain declarations: " << endl;
    while(getline(filebuf,line)){
        linecount++;
        
    } /* HA */

    filebuf.close();
    return 0;
}
