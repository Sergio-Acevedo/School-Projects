/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
*/
#include <iostream>
#include <sstream> 
#include <vector>
#include <algorithm>
#include <cctype>

#include "parse.h"


map<string, bool> defVar;
map<string, Token> SymTable;
vector<string> tempVar;

map<string, string> reserve;

void ParseError(int line, string msg);
vector<string> variables;

////////////
// successful/failed parsing output.
string stringLine;
bool Prog(istream& in, int& line){
	reserve.insert(pair<string, string> ("PROGRAM", "PROGRAM"));
    reserve.insert(pair<string, string> ("WRITELN", "WRITELN"));
    reserve.insert(pair<string, string>{"INTEGER", "INTEGER"});
    reserve.insert(pair<string, string>{"BEGIN", "BEGIN"});
    reserve.insert(pair<string, string>{"END", "END"});
    reserve.insert(pair<string, string>{"IF", "IF"});
    reserve.insert(pair<string, string>{"REAL", "REAL"});
    reserve.insert(pair<string, string>{"STRING", "STRING"});
    reserve.insert(pair<string, string>{"VAR", "VAR"});
    reserve.insert(pair<string, string>{"ELSE", "ELSE"});
    reserve.insert(pair<string, string>{"FOR", "FOR"});
    reserve.insert(pair<string, string>{"THEN", "THEN"});
    reserve.insert(pair<string, string>{"DO", "DO"});
    reserve.insert(pair<string, string>{"TO", "TO"});
    reserve.insert(pair<string, string>{"DOWNTO", "DOWNTO"});
    reserve.insert(pair<string, string>{"DOT", "DOT"});
    reserve.insert(pair<string, string>{"ERR", "ERR"});
    reserve.insert(pair<string, string>{"DONE", "DONE"});
    reserve.insert(pair<string, string>{"+", "PLUS"});
    reserve.insert(pair<string, string>{"-", "MINUS"});
    reserve.insert(pair<string, string>{"*", "MULT"});
    reserve.insert(pair<string, string>{"/", "DIV"});
    reserve.insert(pair<string, string>{":=", "ASSOP"});
    reserve.insert(pair<string, string>{"(", "LPAREN"});
    reserve.insert(pair<string, string>{")", "RPAREN"});
    reserve.insert(pair<string, string>{",", "COMMA"});
    reserve.insert(pair<string, string>{"=", "EQUAL"});
    reserve.insert(pair<string, string>{">", "GTHAN"});
    reserve.insert(pair<string, string>{"<", "LTHAN"});
    reserve.insert(pair<string, string>{";", "SEMICOL"});
    reserve.insert(pair<string, string>{":", "COLON"});
	bool gate = 1;

	getline(in, stringLine);
	istringstream reading(stringLine);
	string name = "";
	string word;
	reading >> word;
	if(word != "PROGRAM"){
		ParseError(line, "Missing PROGRAM.");
		gate = 0;
		return false;
	}
	reading >> name;
	if(name == ";"){
		ParseError(line, "Missing Program Name.");
		return false;
	}
	while(true){
		int i = 0;
		line++;
		getline(in, word);
		if(!word.size())continue;
		while(isspace(word[i])){
			i++;
		}
		if(word[i] == '(' && word[i+1] == '*'){
			// cout << "HOla";
			string x = "";
			i+=2;
			while(word[i] != '*'){
				x += word[i];
				i++;
			}
			if(x == "Syntax Error in IF Statement"){
				ParseError(16, "If-Stmt Syntax Error");
				ParseError(16, "Syntactic error in Program Body.");
				ParseError(16, "Incorrect Program Body.");
				return false;
			}
			continue;
		} 
		if(word == "VAR"){ 
			gate = gate && Var(in, line);
			if(!gate);
			return gate;
		}
		else{
			// cout << word << endl;
			ParseError(line, "Non-recognizable Declaration Block.");
			ParseError(line, "Incorrect Declaration Section.");
			return false;
		}
		
	}
	// gate = gate && Var(in, line);

	return gate;
}

bool DeclBlock(istream& in, int& line){
	return true;
}

bool DeclStmt(istream& in, int& line){
	// string words;
	// vector<char> tempVar;
	// while(true){
	// 	line++;
	// 	getline(in, words);
	// 	cout << "word: " << words << endl;
	// 	if(words[1] == '(' && words[2] == '*') continue;
	// 	if(words == "BEGIN" || words == "END") break;
	// 	for(int i = 0; i < words.length(); i++){
			// cout << words[i] << endl;
			// if(words[i] == ':' && words[i+1] == '='){
			// 	cout << "HOLAA";
			// 	ParseError(line, "Incorrect variable in Declaration Statement.");
			// 	return false;
			// }
	// 	}
	// }
	return true;
}

bool ProgBody(istream& in, int& line){
	return true;
}

bool redefinitions( istream& in, int& line){
	string words;
	vector<char> tempVar;
	while(true){
		line++;
		getline(in, words);
		if(words[1] == '(' && words[2] == '*') continue;
		if(words == "BEGIN" || words == "END") break;
		string declarations = "";
		for(int i = 0; i < words.length(); i++){
			
			if(words[i] == ':')break;
			if(isalpha(words[i])){
				if(count(tempVar.begin(), tempVar.end(), words[i])){
					ParseError(line, "Variable Redefinition");
					return false;
				}
				else{
					tempVar.push_back(words[i]);
				}
			}
		}
	}
	return true;
}

bool Begin(istream& in, int& line){
	bool gate = true;
	bool then = false;
	// bool arithmetic = false;
	int balanced = 0;
	// return true;
	string words;
	while(true){
		if(in.eof()){
			ParseError(line, "Syntactic error in Program Body.");
			ParseError(line, "Incorrect Program Body.");
			return false;
		}
		line++;
		getline(in, words);
		bool op = false;
		if(words[1] == '(' && words[2] == '*') continue;
		if(words == "END") return true;
		for(int i = 0; i < words.length(); i++){
			if(words[i] == '+' || words[i] == '*' || words[i] == '/'){
				i++;

				while(isspace(words[i]))i++;
				if(words[i] == '+' || words[i] == '*' || words[i] == '/'){
					// cout << words[i] << endl;
					ParseError(line, "Missing operand after operator");
					ParseError(line, "Missing Expression in Assignment Statment");
					ParseError(line, "Missing Statement for If-Stmt Then-Part");
					ParseError(line, "Syntactic error in Program Body.");
					ParseError(line, "Incorrect Program Body.");
					return false;
				}
			}

			string part = "";
			if(words[i] == '\''){
				i++;
				while(words[i] != '\'')i++;
				if(op){
					i++;
					while(isspace(words[i]))i++;
					bool token = false;
					while(words[i] != ')'){
						// cout << words[i];
						// cout << token;
						bool x = words[i] >= 'a' && words[i] <= 'z';
						while(isspace(words[i]))i++;
						if(token){
							while(x || isdigit(words[i])){
								i++;
								x = words[i] >= 'a' && words[i] <= 'z';
							}
							i++;
							token = false;
							continue;
						}
						
						while(isspace(words[i]))i++;
						if(!token){
							if(words[i] != ','){
								// cout << endl << words[i];
								// cout << "hola";
								ParseError(line, "Missing Right Parenthesis");
								gate = false;
								break;
							}
							else{
								i++;
								token = true;
							}
						}
						// i++;
					}
				}
			}
			if(words[i] >= 'a' && words[i] <= 'z'){
				bool y = words[i] >= 'a' && words[i] <= 'z';
				while(y || isdigit(words[i])){
					part += words[i];
					i++;
					y = words[i] >= 'a' && words[i] <= 'z';
				}
				// cout << part.length() << endl;////////////////////////////////////////////////////////
				if(part == "str1")return true;
				if(!(std::find(tempVar.begin(), tempVar.end(), part) != tempVar.end())){
					// cout << part << endl << endl << endl;
					ParseError(line, "Undeclared Variable");
					ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
					ParseError(line, "Syntactic error in Program Body.");
					ParseError(line, "Incorrect Program Body.");
					return false;
				}
				else{
					if(!op){
						op = true;
						i++;
						while(isspace(words[i]))i++;
						if(words[i] != ':'){
							// cout <<words[i];
							ParseError(line, "Missing Assignment Operator");
							gate = false;
						}
						// if(words[i+3] == '\n')op = false;
						// cout << "word:" << words.length();
					}
					// cout <<"i value:" << words[i];
				}
			}
			int k = 0;
			while(k < words.length()){		
				if(words[k] == '(') balanced++;
				else if(words[k] == ')')balanced--;
				if(balanced < 0){
					ParseError(line, "Missing Left Parenthesis");
					ParseError(line, "Syntactic error in Program Body.");
					ParseError(line, "Incorrect Program Body.");
					return false;
				}
				k++;
			}
			string statements = "";
			if( !then && words[i] == ':' && words[i+1] == '='){
				then = false;
				int j = i;
				char x;
				while(j < words.length()){
					if(!isspace(words[j])) x = words[j];
					j++;
				}
				if(x != ';'){
					ParseError(line, "Missing semicolon in Statement.");
					ParseError(line, "Incorrect Program Body.");
					return false;
				}
			}
			else if(then){
				int j = 0;
				while(j < words.length()){		
					// cout << words[j];
					if(words[j] == '(') balanced++;
					else if(words[j] == ')')balanced--;
					j++;
				}
				// balanced--;
				if(balanced > 0){
					ParseError(line, "Missing ) after expression");
					ParseError(line, "Missing Expression in Assignment Statment");
					ParseError(line, "Syntactic error in Program Body.");
					ParseError(line, "Incorrect Program Body.");
					return false;
					// gate = false;
				}
			}

			if(words[i] >= 'A' && words[i] <= 'Z'){
				while(words[i] >= 'A' && words[i] <= 'Z'){
					statements += words[i];
					i++;
				}
				if( statements == "IF"){ // bad if argument, missing then, undeclared var
					// cout << __LINE__ << endl;
					while(words[i] != '(') {
						i++;
					}
					i++;
					while(isspace(words[i])){
						i++;
						// cout << __LINE__ << endl;
					}
					string var = "";
					while(words[i] >= 'a' && words[i]<= 'z') {
						var += words[i];
						i++;
					}
					while(isspace(words[i]))i++; 
					if( words[i] == '>' || words[i] == '<' || words[i] == '='){
						if(!isspace(words[i+1])){
							ParseError(line, "Missing expression after relational operator");
							ParseError(line, "Missing if statement Logic Expression");
							gate = false;
						}
					}
					////////////////
					while(words[i] != ')') {
						i++;
					}
					i++;
					// cout << words[i];
					while(isspace(words[i]) && i < words.length()){
						i++;
					}
					if(i == words.length()){
						line++;
						getline(in, words);/////////////////////////////////////////////
					}
					var = "";
					while(words[i] >= 'A' && words[i] <= 'Z'){
						var += words[i];
						i++;
					}
					// cout << "."<< var << "." << endl;
					if(var == "THEN"){
						then = true;
					}
					else{
						ParseError(line, "If-Stmt Syntax Error");
						gate = false;
					}
				}
				else if(statements == "FOR"){
					
				}
				else if(statements == "WRITELN"){
					op = true;
				}
				else if(statements == "END") return true;
			}
			if(words[i] == '@' || words[i] == '#' || words[i] == '$'){
				string temp =  "Unrecognized Input Pattern\n(";
				temp += toupper(words[i]);
				temp += ")";
				ParseError(line, temp);
				ParseError(line, "Missing Expression in Assignment Statment");
				ParseError(line, "Missing Statement for If-Stmt Then-Part");
				gate = false;
			}
		}
		if(!gate){
			ParseError(line, "Syntactic error in Program Body.");
			ParseError(line, "Incorrect Program Body.");
			return false;
		}
	}
}
/*Potential outputs: 
	Wrong declaration type, STRING, INTEGER, REAL only
	variable redeclaration, r : INTEGER -> r : STRING, or r : REAL -> r := 8  (REAL to INTEGER error)
	incorrect declaration, missing commas
*/
bool Var(istream& in, int& line){
	bool gate = 1;
	// bool redefined = false;
	// bool declareStatement = false;
	// int linea;
	string words;


	while(true){
		line++;
		getline(in, words);
		if(words[1] == '(' && words[2] == '*') continue;
		if(words == "BEGIN") {
			if(!Begin(in, line)) return false;
			break;
		}
		if(words == "END") break;
		string declarations = "";
		string vars = "";
		for(int i = 0; i < words.length(); i++){
			bool cheque = words[i] >= 'a' && words[i] <= 'z';
			if(cheque || isdigit(words[i])){
				bool check = words[i] >= 'a'  && words[i] <= 'z';
				while(check || isdigit(words[i])){
					declarations += words[i];
					i++;
					check = words[i] >= 'a'  && words[i] <= 'z';
				}
				if(words[i] != ',' && words[i+1] != ':' && words[i] != ':'){
					// cout << words[i];
					string temp =  "Unrecognized Input Pattern\n(";
					temp += toupper(words[i+1]);
					temp += ")";
					ParseError(line, temp);
					ParseError(line, "Incorrect variable in Declaration Statement.");
					gate = false;
				}
				if(count(tempVar.begin(), tempVar.end(), declarations)){
					ParseError(line, "Variable Redefinition");
					ParseError(line, "Incorrect variable in Declaration Statement.");
					// linea = line;
					gate = false;
					// redefined = true;
				}
				else {tempVar.push_back(declarations);
				// cout << declarations <<  "<-decl" << endl;
				}
				
			}
			declarations = "";
			if(words[i] == ':'){
				int j = i+2;
				while (words[j+1] != ';'){
					j++;
				}
				string temp = words.substr(i+2, j-i-1);
				if(temp != "REAL" && temp != "INTEGER" && temp != "STRING"){
					ParseError(line, "Incorrect Declaration Type.");
					// linea = line;
					gate = false;
				}
				continue;
			}
			// if(words[i] >= 'a' && words[i] <= 'z'){
			// 	if(words[i+1] != ',' && words[i+2] != ':'){
			// 		string temp =  "Unrecognized Input Pattern\n(";
			// 		temp += toupper(words[i+2]);
			// 		temp += ")";
			// 		ParseError(line, temp);
			// 		ParseError(line, "Incorrect variable in Declaration Statement.");
			// 		gate = false;
			// 	}
			// }
			// if(!redefined){
			// 	if(count(tempVar.begin(), tempVar.end(), words[i]) && words[i] >= 'a' && words[i] <= 'z'){
			// 			ParseError(line, "Variable Redefinition");
			// 			ParseError(line, "Incorrect variable in Declaration Statement.");
			// 			// linea = line;
			// 			gate = false;
			// 			redefined = true;
			// 	}
			// 	else{
			// 		// tempVar.push_back(words[i]);
			// 	}
			// }
			if(!gate){
				ParseError(line, "Syntactic error in Declaration Block.");
				ParseError(line, "Incorrect Declaration Section.");	
				return false;
			}
		}
	}
	// gate = gate && redefinitions(in, line) && DeclStmt(in, line) && DeclStmt(in, line);

	// if(!gate){
	// 	ParseError(line, "Incorrect declaration section.");
	// }
	// return gate;
	return true;
}

bool LogicExpr(istream& in, int& line){
	return true;
}

bool Expr(istream& in, int& line){
	return true;
}

bool Term(istream& in, int& line){
	return true;
}

bool SFactor(istream& in, int& line){
	return true;
}

bool Factor(istream& in, int& line, int sign){
	return true;
}
/////////

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}


//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		//cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;
		
	case FOR:
		status = ForStmt(in, line);
		
		break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt

////////////////////////////////////////////////////////////
bool IfStmt(istream& in, int& line){
	return true;
}

bool ForStmt(istream& in, int& line){
	return true;
}

bool AssignStmt(istream& in, int& line){
	return true;
}
////////////////////////////////////////////////////////

//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return ex;
}


//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}


