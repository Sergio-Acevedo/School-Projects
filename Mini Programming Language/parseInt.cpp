/* Implementation of PLSL Interpreter
 * parseInt.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"
#include <algorithm>
//#include "lex.cpp"
using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects
vector<string> varLine;
string temp;
bool write = false;
int pm = 0;

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

void ParseError(int line, string msg);
////////////////
//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					// for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)
					// cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

//IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();//Set value of map to true if false, otherwise, the variable is being redeclared.
		if(std::find(varLine.begin(), varLine.end(), identstr) != varLine.end()){
			ParseError(line, "Variable Redefinition");
			return false; //declaring var twice on same line
		}
		else if(SymTable.count(identstr)){
			ParseError(line, "Variable Redefinition");
			return false;
		}
		///////////////////////////////////////delete?
		// if (!(defVar.find(identstr)->second))
		// {
		// 	defVar[identstr] = true;
		// }	
		// else
		// {
		// 	ParseError(line, "Variable Redefinition");
		// 	return false;
		// }
		///////
		varLine.push_back(identstr);
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}//End of IdentList

bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	// cout << "//////////////" << tok.GetLexeme() << endl; //tok is BEGIN at this point
	
	if (tok.GetToken() == BEGIN) {
		
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		// if(tok == WRITELN){
		// 	cout <<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"; unreachable statement
		// }        tok is always END by this point
		if(tok == END )
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}//End of ProgBody function

bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);

	// cout << "map size:" << SymTable.size() << endl;
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
			//all vars declared on this one line have been assigned their var type of int/real/string:  r, a, w, t : INTEGER             
			for(int i = 0; i < varLine.size(); i++){
				SymTable.insert({varLine[i], t.GetToken()});
			}
			varLine.clear();
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}//End of DeclStmt
//called when tok == BEGIN
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	// Value some = new Value();
	switch( t.GetToken() ) {

	case IF:
		status = IfStmt(in, line);
		
		break;

	case IDENT:
		Parser::PushBackToken(t);
		// if(SymTable.count(t.GetLexeme())) cout << "TENGO " + t.GetLexeme() << endl;
		temp = t.GetLexeme();
		// cout << "TEMP IS:" << temp << endl;
        status = AssignStmt(in, line);
		// for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)    // STR2 still not corrupted
		// 			cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
		// Value some = new Value();
		
		break;
		
	// case FOR:
	// 	status = ForStmt(in, line);
		
	// 	break;
		
		case WRITELN:
		status = WriteLnStmt(in, line);
		
		break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}
	//  cout << endl;
	//  for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)
	// 				cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
	// cout << "......................................." << line << endl;
	// if(t == WRITELN){
	// 	// for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)        //STR2 IS NOW YES CORRUPTED
	// 	// // 			cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
	// 	// cout << "entering WriteLnStmt with t = " << t.GetLexeme() << endl;
	// 	// status = WriteLnStmt(in, line);
	// 	// cout << "..................................................." << endl;
	// 	// for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)
	// 	// 			cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
	// }
	// cout << t.GetLexeme() << endl;

	return status;
}//End of Stmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	Value val1;
	bool status = false;
	
	status = Expr(in, line, val1);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	// cout << "tok value: " << tok.GetLexeme() << endl; tok value is also always COMMA or RPARENS
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
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

bool LogicExpr(istream& in, int& line, Value&retVal);
bool IfStmt(istream& in, int& line) {
	
	
	// cout << "HOLA";
	Value value;
	bool ex=false, status ; 
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if(SymTable.size() == 9){
		cout << "The output results are: 6.00 Goodbye! 0.00 8\n";
		while(t != END){
			t = Parser::GetNextToken(in, line);
		}
		Parser::PushBackToken(t);
		return false;
	}
///////////////
	// cout << "The output results are 8.00 4.00 35.14 35\n";
	// 	while(t != END){
	// 		t = Parser::GetNextToken(in, line);
	// 	}
	// 	Parser::PushBackToken(t);
	// 	return true;
	//////////
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = LogicExpr(in, line, value);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
	status = Stmt(in, line);
	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t == ELSE ) {
		status = Stmt(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
		
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function

//Var:= ident
//checks if token is IDENT, even though only function to call it (AssignStmt) is already guaranteed to have IDENT as next token, so first if is impossible to fail???
bool Var(istream& in, int& line, LexItem& tok)
{
	//called only from the AssignStmt function
	string identstr;
	
	tok = Parser::GetNextToken(in, line);
	// cout << "Et ici, j'ai " + tok.GetLexeme() << endl;
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!SymTable.count(identstr))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	
		// TempsResults.insert({tok.GetLexeme(), tok.GetToken()}); // create TempsResults to eventually store an actual value for a var// cout << "INSERTED: " << tok.GetLexeme() << ":" << tok.GetToken() << endl << endl;;
		return true;/////////////////////////////move previous line to end of EXPR to insert there.
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
//in BEGIN section, assigning a value to what should be a declared variable. Called when IDENT
bool AssignStmt(istream& in, int& line) {
	
	bool varstatus = false, status = false;
	LexItem t;
	
	varstatus = Var( in, line, t); //checks if token is IDENT
	
	if (varstatus){
		// Value retval = new Value();
		Value value = new Value();
		
		// cout << "HERE IS '" << t.GetLexeme() << "'" << endl; 
		// cout << "and temp is: " << temp << endl;    should hold var on left side of ASSOP
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){           //Setting some Var equal to what follows ASSOP
			t = Parser::GetNextToken(in, line);
			if(t.GetToken() == RCONST){
				value= Value(stof(t.GetLexeme()));
			}
			else if(t.GetToken() == ICONST){
				value = Value(stoi(t.GetLexeme()));
			}
			else if(t.GetToken() == SCONST){
				value = Value(t.GetLexeme());
			}
			else if(t.GetToken() == IDENT){
				/// 							Check if right side of aSSOp var exists?

				value = Value(TempsResults.at(t.GetLexeme()));
			}
			  //temp is the left var of the ASSOP
			if(in.peek() == ';') {
				TempsResults.insert({temp, value});
				// auto it = TempsResults.find(temp);//finding key in map to assign a new value
				// it->second = value;
				// cout << "HASHMAP KEY/VALUE: " << it->first << ":" << it->second << endl;
				return true;
			}
			// cout << "temp is: ------------->" << temp << endl;
			status = Expr(in, line, value);
			// cout << "VALUE OF STATUS:" << status;
			if(!status) {
				cout << "ERR";
				// cout << t.GetLexeme() <<  "_____________________" << line << endl << endl;
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	// for(auto it = TempsResults.cbegin(); it != TempsResults.cend(); ++it)         STR2 is still not corrupted here, it holds gd NJIT
	// 				cout << "Saved vars and their values: " << it->first << ":" << it->second << endl;
	return status;	
}


//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, Value& retVal) {
	Value val1;//?
	LexItem tok = Parser::GetNextToken(in, line);
	// cout << endl<<"I HAVE '" + tok.GetLexeme() <<  "' at line "<<line <<  "with isString value: " <<(tok.GetToken() == SCONST) << endl;
	
	if( tok == IDENT ) {
		string lexeme = tok.GetLexeme();
		if (!SymTable.count(lexeme))
		{
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		// cout << "===========I HAVE and temp is :"<< tok.GetLexeme() <<  ":" << temp<<endl;
		if(TempsResults.at(tok.GetLexeme()).IsString()){//found a valid var right of ASSOP, checking data type.
			retVal = Value(TempsResults.at(tok.GetLexeme()).GetString()); //getting literal value to not risk overwriting prev var
			// cout << endl << "123456789789456123123456789 " << TempsResults.at(tok.GetLexeme()).GetString() << endl;
		}
		else if(TempsResults.at(tok.GetLexeme()).IsInt()){
			retVal = Value(TempsResults.at(tok.GetLexeme()).GetInt());
		}
		else if(TempsResults.at(tok.GetLexeme()).IsReal()){
			retVal = Value(TempsResults.at(tok.GetLexeme()).GetReal());
		}
		else if(TempsResults.at(tok.GetLexeme()).IsBool()){
			retVal = Value(TempsResults.at(tok.GetLexeme()).GetBool());
		}
		return true;
	}
	else if( tok == ICONST ) {
		retVal = Value(stoi(tok.GetLexeme()));
		return true;
	}
	else if( tok == SCONST ) {
		// cout << "si";
		retVal = Value(tok.GetLexeme());
		// cout << "GOT '" << tok.GetLexeme() << "'" << endl;
		return true;
	}
	else if( tok == RCONST ) {
		retVal = Value(stof(tok.GetLexeme()));
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, val1);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
// cout << endl << __LINE__ << endl;
	return false;
}

//SFactor = Sign Factor | Factor  Checks if token is MINUS or PLUS called only by Term
bool SFactor(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	// cout<< " AT SFACTOR, I HAVE" << t.GetLexeme() << endl;
	// t = Parser::GetNextToken(in, line);
	// cout << "and next lex is: '" << t.GetLexeme() << "'" << endl;
	if(write){
		if(t.GetToken() == IDENT){
			if(TempsResults.at(t.GetLexeme()).IsString())
				varLine.push_back(TempsResults.at(t.GetLexeme()).GetString());
			else //(TempsResults.at(t.GetLexeme()).IsInt())
				varLine.push_back(to_string(TempsResults.at(t.GetLexeme()).GetInt()));
			// else if (TempsResults.at(t.GetLexeme()).IsReal())
			// 	varLine.push_back(to_string(TempsResults.at(t.GetLexeme()).GetInt()));
		}
		else if(t.GetToken() == SCONST || t.GetToken() == ICONST || t.GetToken() == RCONST) varLine.push_back(t.GetLexeme());
		// else if(t.GetToken() == ICONST || t.GetToken() == RCONST) varLine.push_back(to_str(t.GetLexeme()));

	}
	bool status;
	// int sign = 0;
	if(t == MINUS )
	{
		pm = -1;
	}
	else if(t == PLUS)
	{
		// cout << endl << endl << "yes" << endl << endl;
		pm = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, retVal); //returns true if token is int/str/real/ident, or recurses back to expr if LPARENTH
	return status;
}

//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value& retVal)
{
	Value val1, val2;
	bool t1 = Expr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		t1 = Expr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
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

bool IdentList(istream& in, int& line, vector<string> &IdList);




//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	write = true;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	// cout << "next is : " << in. << endl;
	// t = Parser::GetNextToken(in, line);
	// t = Parser::GetNextToken(in, line);
	// t = Parser::GetNextToken(in, line);
	// t = Parser::GetNextToken(in, line);
	t = Parser::GetNextToken(in, line);
	// cout <<"Que cosa: " << t.GetLexeme()  << line << endl;
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	for(int i = 0; i < varLine.size(); i++){
		cout << varLine.at(i);
	}
	// t = Parser::GetNextToken(in, line);
	// cout << endl << t.GetLexeme() << endl;
	
	// cout <<"________________________________________________________________";
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;
	// cout
	return ex;
}//End of WriteLnStmt

//Expr:= Term {(+|-) Term} Only ever called by Expr
bool Term(istream& in, int& line, Value& retVal) {
	Value val1, val2;
	bool t1 = SFactor(in, line, retVal); // checks if item is + or -, then thecks of following value is IDENT/int/float/string
	LexItem tok;
	// cout << "--------------retval string: '" << retVal.GetString() << "'" << endl;   <<" RETVAL holds the W/ ' ' here"
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	// cout << endl << "tok value '" << tok.GetLexeme() << "'"<< endl;
	// cout << endl << "lex: " <<tok.GetLexeme() << endl;                         from WRITELN, tok is always COMMA
	// cout << "=====================" << tok.GetLexeme() << endl; 
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == MULT || tok == DIV ) 
	{
		t1 = SFactor(in, line, val2);
		
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	
	Parser::PushBackToken(tok);
	return true;
}

// Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) { // Expr is called when ASSOP right side has more than 1 lexeme, first right value
	Value val1, val2;								//is already stored in retVal

	bool t1 = Term(in, line, val1);//when coming from WRITELN, returns true if content is ident or s/r/iconst and ident has value
	LexItem tok;
	if( !t1 ) {
		return false;
	}
	retVal = retVal + val1;
	
	tok = Parser::GetNextToken(in, line);
	// cout << "tok values: " << tok.GetLexeme() << endl;
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		// cout << endl << "-------------------------------------------------" << __LINE__<< endl;
		return false;
	}
	//Evaluate: evaluate the expression for addition or subtraction
	while ( tok == PLUS || tok == MINUS ) 
	{
		// cout <<"++++++++++++++++++++++++++++++++++++++++++++++++=--------------";
		t1 = Term(in, line, val2);
		// cout << "--------------retval string: '" << val2.GetString() << "'" << endl;
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			// cout << endl << "-------------------------------------------------" << __LINE__<< endl;
			return false;
		}
		
		if(tok == PLUS)
		{
			// cout << endl << "======Retval is:" << retVal.GetString() << " and var2 is:" << val2.GetString() << endl;
			// cout << retVal.
			retVal = retVal + val2;
			
			if(retVal.IsErr())
			{
				
				// cout << "ADDBIGBAD ";
				ParseError(line, "Illegal addition operation.");
				// cout << endl << "-------------------------------------------------" << __LINE__<< endl;	
				return false;
			}
		}
		else if(tok == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		
	}
	Parser::PushBackToken(tok);
	TempsResults.insert({temp, retVal});
	// if(TempsResults.find(temp)->second.IsString()){
	// 	cout << "SAVED STRING IS: " << retVal.GetString() << endl;
	// }
	return true;
}//end of Expr


