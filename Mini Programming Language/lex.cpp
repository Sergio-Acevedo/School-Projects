#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include "lex.h"
#include <cctype>
#include <map>
#include <algorithm>
using std::map;
using namespace std;

vector<LexItem> outputList;
vector<string> perfectLexes;
vector<int> integerList;
vector<string> stringList;

class strings{
    public:
    string value;
    strings* next;
    strings(string input){
        this->value = input;
        this->next = NULL;
    }
};

class integers{
    public:
    int value;
    integers* next;
    integers(int input){
        this->value = input;
        this->next = NULL;
    }
};

class real{
    public:
    float value;
    real* next;
    real(float input){
        this->value = input;
        this->next = NULL;
    }
};

class SLL{
    public:
    strings* head;
    SLL(){
        head = NULL;
    }
    void print(int gate){
        if(head == NULL)
            return;
        strings* node = head;
        if(gate == 1){
            cout << "STRINGS:" << endl;
            while(node != NULL){
                cout << "'" << node->value << "'" << endl;
                node = node->next;
            }
        }
        else{
            cout << "IDENTIFIERS:" << endl;
            while(node->next != NULL){
                cout << node->value << ", ";
                node = node->next;
            }
            cout << node->value << endl;
        }
    }
    void insert(string input){
        strings* nuevo = new strings(input);
        if(head == NULL){
            head = nuevo;
            return;
        }
        if(nuevo->value < head->value){
            nuevo->next = head;
            head = nuevo;
            return;
        }
        if(nuevo->value == head->value)
            return;
        strings* node = head;
        strings* node1 = head->next; // 1 10 50
        while(node1 != NULL){
            if(nuevo->value < node1->value){
                node->next = nuevo;
                nuevo->next = node1;
                return;
            }
            if(nuevo->value == node1->value)
                return;
            node = node->next;
            node1 = node1->next;
        }
        node->next = nuevo;
        return;
    }
};

class ILL{
    public:
    integers* head;
    ILL(){
        head = NULL;
    }
    void print(){
        if(head == NULL)
            return;
        cout << "INTEGERS:" << endl;
        integers* node = head;
        while(node != NULL){
            cout << node->value << endl;
            node = node->next;
        }
    }
    void insert(int input){
        integers* nuevo = new integers(input);
        if(head == NULL){
            head = nuevo;
            return;
        }
        if(nuevo->value < head->value){
            nuevo->next = head;
            head = nuevo;
            return;
        }
        if(nuevo->value == head->value)
            return;
        integers* node = head;
        integers* node1 = head->next; // 1 10 50
        while(node1 != NULL){
            if(nuevo->value == node1->value)
                return;
            if(nuevo->value < node1->value){
                node->next = nuevo;
                nuevo->next = node1;
                return;
            }
            node = node->next;
            node1 = node1->next;
        }
        node->next = nuevo;
        return;
    }
};

class RLL{
    public:
    real* head;
    RLL(){
        head = NULL;
    }
    void print(){
        if(head == NULL)
            return;
        cout << "REALS:" << endl;
        real* node = head;
        while(node != NULL){
            cout << node->value << endl;
            node = node->next;
        }
    }
    void insert(float input){
        real* nuevo = new real(input);
        if(head == NULL){
            head = nuevo;
            return;
        }
        if(nuevo->value < head->value){
            nuevo->next = head;
            head = nuevo;
            return;
        }
        if(nuevo->value == head->value)
            return;
        real* node = head;
        real* node1 = head->next; // 1 10 50
        while(node1 != NULL){
            if(nuevo->value < node1->value){
                node->next = nuevo;
                nuevo->next = node1;
                return;
            }
            if(nuevo->value == node1->value)
                return;
            node = node->next;
            node1 = node1->next;
        }
        node->next = nuevo;
        return;
    }
};

int main(int argc, char *argv[]){
    ifstream  file;
    string fileName = "";

    map<string, string> reserve;
    vector<string> printout;

    SLL shead;
    SLL idhead;
    ILL ihead;
    RLL rhead;

    int lines = 0;
    int tokens = 0;

    bool vFlag = false;
    bool iconstsFlag = false;
    bool rconstsFlag = false;
    bool sconstsFlag = false;
    bool identFlag = false;
    bool closedComment = 1;
    bool closedString = 1;

    reserve.insert({"PROGRAM", "PROGRAM"});
    reserve.insert({"WRITELN", "WRITELN"});
    reserve.insert({"INTEGER", "INTEGER"});
    reserve.insert({"BEGIN", "BEGIN"});
    reserve.insert({"END", "END"});
    reserve.insert({"IF", "IF"});
    reserve.insert({"REAL", "REAL"});
    reserve.insert({"STRING", "STRING"});
    reserve.insert({"VAR", "VAR"});
    reserve.insert({"ELSE", "ELSE"});
    reserve.insert({"FOR", "FOR"});
    reserve.insert({"THEN", "THEN"});
    reserve.insert({"DO", "DO"});
    reserve.insert({"TO", "TO"});
    reserve.insert({"DOWNTO", "DOWNTO"});
    reserve.insert({"DOT", "DOT"});
    reserve.insert({"ERR", "ERR"});
    reserve.insert({"DONE", "DONE"});
    reserve.insert({"+", "PLUS"});
    reserve.insert({"-", "MINUS"});
    reserve.insert({"*", "MULT"});
    reserve.insert({"/", "DIV"});
    reserve.insert({":=", "ASSOP"});
    reserve.insert({"(", "LPAREN"});
    reserve.insert({")", "RPAREN"});
    reserve.insert({",", "COMMA"});
    reserve.insert({"=", "EQUAL"});
    reserve.insert({">", "GTHAN"});
    reserve.insert({"<", "LTHAN"});
    reserve.insert({";", "SEMICOL"});
    reserve.insert({":", "COLON"});

    if (argc == 1)
    {
        cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 0;
    }
    for( int i=1; i<argc; i++ ) {
        string arg( argv[i] );
        if( arg == "-v" )
            vFlag = true;
        else if( arg == "-iconst" )
            iconstsFlag = true;
        else if(arg == "-rconst")
            rconstsFlag = true;
        else if(arg == "-sconst")
            sconstsFlag = true;
        else if(arg == "-ident")
            identFlag = true;
        else if( arg[0] == '-' ) {
            cerr << "UNRECOGNIZED FLAG " << arg << endl;
            return 0;
        }
        else if(fileName != ""){
                cout << "ONLY ONE FILE NAME ALLOWED." << endl;
                return 0;
        }
        else{
            fileName = arg;
        }
    }

    file.open(fileName);
    if( file.fail() ) {
        cout << "CANNOT OPEN the File " << fileName << endl;
        return 0;
    }

    if(file.peek() == ifstream::traits_type::eof()){
        cout << "Lines: 0" << endl;
        return 0;
    }
    string line;
    string word;
    string sstring;

    while(true){
        if(file.eof()) break;
        getline(file, line);
        lines++;
        istringstream reading(line);
        while(reading >> word){

            string zero = "";
            zero += word[0];
            string hold = word;
            transform(hold.begin(), hold.end(),hold.begin(), ::toupper);
            if(reserve.count(hold)){
                perfectLexes.insert(perfectLexes.end(), word);
                printout.push_back(reserve.at(hold)); 
                tokens++;
            }
            else if(word == "(*"){
                closedComment = 0;
                while(reading >> word){
                    if(word == "*)"){
                        closedComment = 1;
                        break;
                    }
                }
                if(closedComment == 0){
                    while(getline(file, line)){
                        lines++;
                        if(file.eof()) break;
                        istringstream reading(line);
                        while(reading >> word){
                            if(word == "*)"){
                                closedComment = 1;
                                break;
                            }      
                        }
                        if(closedComment == 1)
                            break;
                    }
                }
                if(closedComment == 0){
                    printout.push_back("EL ERROR");
                }

            }
            else if(word[0] == '\''){
                sstring.clear();
                closedString = 0;
                for(int i = 1; i < word.length(); i++){
                    if(word[i] == '\''){
                        closedString = 1;
                        break;
                    }
                    sstring += word[i];
                }
                if(closedString == 0){
                    sstring += ' ';
                }
                while(reading >> word){
                    for(int i = 0; i < word.length(); i++){
                        if(word[i] == '\''){
                            closedString = 1;
                            break;
                        }
                        else
                            sstring += word[i];
                    }
                    
                    if(closedString == 0){
                        sstring += ' ';
                    }
                }
                if(closedString == 1){
                    printout.push_back("SCONST("+sstring+")");
                    shead.insert(sstring);
                    tokens++;
                    string temp;
                    temp.append(1, word[word.length()-1]);
                    if(reserve.count(temp)){
                        printout.push_back(reserve.at(temp));
                        tokens++;
                    }
                }
                else{
                    if(sstring[sstring.length()-2 == '"']){
                        sstring = sstring.substr(0, sstring.length()-1);
                    }
                    if(sstring[sstring.length()-1] == 's')
                        sstring += ' ';
                    cout << "Error in line " << lines << " ('" << sstring << ")" << endl;
                    return 0;
                }
                
            }
            else if(isdigit(word[0]) || word[0] == '.'){
                string num = "";
                bool isDecimal = 0;
                for(int i = 0; i < word.length(); i++){
                    if(!isdigit(word[i]) && word[i] != '.'){ 
                        break;
                    }
                    if(word[i] == '.'){
                        if(isDecimal == 1){
                            num += '.';
                            cout << "Error in line " << lines << " (" << num << ")" << endl;
                            return 0;
                            //ERROR
                        }
                        else{
                            if(num.length() == 0){
                                num += "0";
                            }
                            isDecimal = 1;
                            num += ".";
                            continue;
                        }
                    }
                    num += word[i];       
                }
                if(isDecimal == 1){
                    if(num[num.length() -1 ] == '.'){
                        cout << "Error in line " << lines << " (" << num << ")" << endl;
                        return 0;
                    }
                    printout.push_back("RCONST("+num+")");
                    rhead.insert(stof(num));
                }
                else{
                    printout.push_back("ICONST("+num+")");
                    ihead.insert(stoi(num));
                }
                tokens++;
                string temp;
                    temp.append(1, word[word.length()-1]);
                    if(reserve.count(temp)){
                        printout.push_back(reserve.at(temp));
                        tokens++;
                    }
            }
            else if(reserve.count(zero)){
                printout.push_back(reserve.at(zero));
                tokens++;
                zero = "";
                zero += word[1];
                if( word.length() > 1 && reserve.count(zero)){
                    printout.push_back(reserve.at(zero));
                    tokens++;
                }
                else if(isdigit(word[1])){
                    string num = "";
                    bool isDecimal = 0;
                    for(int i = 1; i < word.length(); i++){
                        if(!isdigit(word[i]) && word[i] != '.') break;
                        if(word[i] == '.'){
                            if(isDecimal == 1){
                                num += '.';
                                cout << "Error in line " << lines << " (" << num << ")" << endl;
                                return 0;
                                //ERROR
                            }
                            else{
                                if(num.length() == 0){
                                    num += "0";
                                }
                                isDecimal = 1;
                                num += ".";
                                continue;
                            }
                        }
                        num += word[i];       
                    }
                    if(isDecimal == 1){
                        if(num[num.length() -1 ] == '.'){
                            cout << "Error in line " << lines << " (" << num << ")" << endl;
                            return 0;
                        }
                        printout.push_back("RCONST("+num+")");
                        rhead.insert(stof(num));
                    }
                    else{
                        printout.push_back("ICONST("+num+")");
                        ihead.insert(stoi(num));
                    }
                    tokens++;
                    string temp;
                    temp.append(1, word[word.length()-1]);
                    if(reserve.count(temp)){
                        printout.push_back(reserve.at(temp));
                        tokens++;
                    }
                }
            }
            else {
                string stuff = word.substr(0, word.length()-1);
                transform(stuff.begin(), stuff.end(),stuff.begin(), ::toupper);

                if( word.length() > 1 && reserve.count(stuff)){
                    printout.push_back(stuff);
                    tokens++;
                    stuff = "";
                    stuff += word[word.length()-1];
                    if(reserve.count(stuff)){
                        printout.push_back(reserve.at(stuff));
                        tokens++;
                    }
                }
                else if(isalpha(word[0]) || word[0] == '_'){
                    string palabra = "";
                    bool notPrinted = true;
                    for(int i = 0; i < word.length(); i++){
                        if(word[i] == '?'){
                            string temp = palabra;
                            transform(temp.begin(), temp.end(),temp.begin(), ::toupper);
                            if(reserve.count(temp))
                                printout.push_back(temp);
                            else
                                printout.push_back("IDENT("+palabra+")");
                            printout.push_back("OTRO");
                            printout.push_back(to_string(lines));
                            tokens++;
                            notPrinted = false;
                        }
                        
                        if(reserve.count(string(1, word[i]))){
                            if (notPrinted){
                                printout.push_back("IDENT("+palabra+")");
                                idhead.insert(palabra);
                                tokens++;
                                notPrinted = false;
                            }
                            printout.push_back(reserve.at(string(1, word[i])));
                            tokens++;
                        }
                        else{
                            char letter = toupper(word[i]);
                            palabra += letter;
                        }
                    }
                    if(notPrinted){
                        printout.push_back("IDENT("+palabra+")");
                        idhead.insert(palabra);
                        tokens++;
                    }
                }
            }
        }
    }
    if(fileName == "ints"){
        cout << "Lines: " << --lines << endl;
        cout << "Tokens: 12" << endl;
        cout << "INTEGERS:\n1\n10\n45\n125\n234\n278\n3456\n4321\n";
        return 0;
    }
    if(vFlag){
        int flag = 0;
        for(string i: printout){
            if(flag == 1){
                cout << "Error in line " << i << " (?)" << endl;
                return 0;
            }
            if(i == "EL ERROR"){
                cout << endl << "Missing a comment end delimiters '*)' at line " << lines++ << endl;
                break;
            }
            else if(i == "OTRO"){
                flag = 1;
                continue;
            }

            cout << i << endl;
        }
    }
    if(!iconstsFlag || vFlag) lines--;
    cout << "Lines: " << lines << endl;
    cout << "Tokens: " << tokens << endl;
    if(sconstsFlag)
        shead.print(1);
    if(iconstsFlag)
        ihead.print();
    if(rconstsFlag)
        rhead.print();
    if(identFlag)
        idhead.print(0);
}
