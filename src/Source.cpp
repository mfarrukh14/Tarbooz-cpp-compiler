//Muhammad Farrukh Naveed       01-134211-056
//BS(CS)-5A


#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//GLOBAL VARIABLES

char key;                                   //char to read from file
string toCheck,holder;                      //variable and keyword check string
bool flag = 0;

//ALL THE FILES DECLARED HERE

fstream file("demo.txt", ios::in);                //file to be read
fstream writesymbol("table.txt", ios::out | ios::app);  //symbol table file to be created
fstream readsymbol("table.txt", ios::in);

//#########################################################    TOKEN TYPES    #################################################################

enum TokenType
{
	INT = 1, FLOAT = 2, DOUBLE = 3, LONG = 4, CHAR = 5, STRING = 6, BOOL = 7, VOID = 8, MAIN = 9,
	CLASS = 10, STRUCT = 11, ENUM = 12, PUBLIC = 13, PRIVATE = 14, PROTECTED = 15, UNSIGNED = 16,
	SIGNED = 17, TYPEDEF = 18, CONSTANT = 19, LITERAL = 20, USING = 21, NAMESPACE = 22, STD = 23, SCOPE_RESOLUTION = 24,
	DELETE = 25, VECTOR = 26, STACK = 27, LIST = 29, DEQUE = 30, THIS = 31, STATIC = 32, NEW = 33,
    FRIEND = 34, WHILE = 35, DO = 36, FOR = 37, CONST = 38, CONTINUE = 39, RETURN = 40, IDENTIFIER = 41,
	SEMICOLON = 42, COLON = 43, IF = 44, ELSE = 45, SWITCH = 46, CASE = 47, LESS_THAN = 48, GREATER_THAN = 49,
	LESS_THAN_EQUALS_TO = 50, GREATER_THAN_EQUALS_TO = 51, DEFAULT = 52, EQUAL = 53, EQUAL_EQUAL = 54, NOT = 55, NOT_EQUALS_TO = 56,
	INSERTION = 57, EXERTION = 58, AND = 59, OR = 60, BITWISE_AND = 61, BITWISE_OR = 62, PLUS = 63, MULTIPLY = 64,
	ADDITION = 65, DOT = 66, MINUS = 67, INCREMENT = 68, DECREMENT = 69, DIVIDE = 70, BREAK = 71, LEFT_ROUND_BRACES = 72,
	RIGHT_ROUND_BRACES = 73, LEFT_CURLY_BRACES = 74, RIGHT_CURLY_BRACES = 75, LEFT_SQUARE_BRACES = 76, RIGHT_SQUARE_BRACES = 77,
	COUT = 78, CIN = 79, MODULUS = 80, SPACE=81, COMMENT = 82, COMMA = 83, GARBAGE = 84,BEGIN = 85, END = 86
};

typedef enum { opk, constk, idk } nodekind;




//############################################################   TOKEN STRUCT   #################################################################
struct Token
{
	int value = 0;
	string Token_name = "";
	string idname = "";
	TokenType tok = GARBAGE;
};


Token ret;        //Global Token ret to be used by gettoken and ungettoken functions
Token tokn;       //Global Token tokn to be used for expression tree generation


//############################################################   TREENODE STRUCT   #################################################################

struct treenode
{
	nodekind kind;
	int value;
	string name;
	TokenType opr;
	treenode* leftchild;
	treenode* rightchild;
};


//Functions to generate the tree
treenode* expression();
treenode* term();
treenode* factor();




//Function to check and write if the identifier is present or not in the symboltable file

void IsSymbolPresent(string id)
{

	string temp;
	bool flag = 0;

	//Reset the eof flag and move the pointer to the beginning of the file to search from start
	readsymbol.clear();
	readsymbol.seekg(0, ios::beg);

	while (!readsymbol.eof())
	{
		readsymbol >> temp ;
		//If ID is found present in file, break the loop and do nothing
		if (temp == id)
		{
			flag = 1;
			break;
		}
		//skip
		else continue;
	}

	//Write to file if the id is not already present in it
	if (!flag)
	{
		writesymbol << id << endl;
	}

}




//Function to close all files
//call in main
void CloseAllFiles()
{
	file.close();
	readsymbol.close();
	writesymbol.close();
}





//############################################################   LEXICAL ANALYZER   #################################################################

Token lexical()
{
	//lexeme to be returned
	Token mytoken;

	//First check if the input output file are open
	if (!file.is_open() && !readsymbol.is_open() && !writesymbol.is_open())
	{
		cout << "\nFiles Do Not exist\n";
		exit(0);
	}

	//Get the key from file
	file.get(key);

	//if the file has reached its end then there nothing more to get return garbage
	if (file.eof())
	{
		mytoken.tok = GARBAGE;
		return mytoken;
	}

	//skip all the spaces while the file has characters
	while ( isspace(key) && !file.eof())
	{
		file.get(key);
	}

	//if there remains a space character before end of file then return garbage
	if (isspace(key) && file.peek() == EOF)
	{
		mytoken.tok = GARBAGE;
		return mytoken;
	}

	//cout << "\nKEY: " << key;


	//Checking for symbol characters
		if (key == '(')
		{
			mytoken.Token_name = "Left Round Braces";
			mytoken.tok = LEFT_ROUND_BRACES;
			return mytoken;
		}

		else if (key == ',')
		{
			mytoken.Token_name = "Comma";
			mytoken.tok = COMMA;
			return mytoken;
		}

		else if (key == ')')
		{
			mytoken.Token_name = "Right Round Braces";
			mytoken.tok = RIGHT_ROUND_BRACES;
			return mytoken;
		}

		else if (key == '[')
		{
			mytoken.Token_name = "Left Square Braces";
			mytoken.tok = LEFT_SQUARE_BRACES;
			return mytoken;
		}

		else if (key == ']')
		{
			mytoken.Token_name = "Right Square Braces";
			mytoken.tok = RIGHT_SQUARE_BRACES;
			return mytoken;
		}

		else if (key == '{')
		{
			mytoken.Token_name = "Left Curly Braces";
			mytoken.tok = LEFT_CURLY_BRACES;
			return mytoken;
		}

		else if (key == '}')
		{
			mytoken.Token_name = "Right Curly Braces";
			mytoken.tok = RIGHT_CURLY_BRACES;
			file.get(key);
			return mytoken;
		}

		//multilevel symbol check
		else if (key == '<')
		{

			if (file.peek() == '<')
			{
				file.get(key);
				mytoken.Token_name = "Exertion";
				mytoken.tok = EXERTION;
				return mytoken;
			}
			else if (file.peek() == '=')
			{
				file.get(key);
				mytoken.Token_name = "Less than equals to";
				mytoken.tok = LESS_THAN_EQUALS_TO;
				return mytoken;
			}
			else
			{
				mytoken.tok = LESS_THAN;
				mytoken.Token_name = "Less than";
				return mytoken;
			}

		}

		else if (key == '>')
		{

			if (file.peek() == '>')
			{
				file.get(key);
				mytoken.Token_name = "Insertion";
				mytoken.tok = INSERTION;
				return mytoken;
			}
			else if (file.peek() == '=')
			{
				file.get(key);
				mytoken.Token_name = "Greater than equals to";
				mytoken.tok = GREATER_THAN_EQUALS_TO;
				return mytoken;
			}
			else
			{
				mytoken.tok = GREATER_THAN;
				mytoken.Token_name = "Greater than";
				return mytoken;
			}

		}

		else if (key == '!')
		{
			if (file.peek() == '=')
			{
				file.get(key);
				mytoken.Token_name = "Not Equals to";
				mytoken.tok = NOT_EQUALS_TO;
				return mytoken;
			}
			mytoken.tok = NOT;
			mytoken.Token_name = "NOT";
			return mytoken;
		}

		else if (key == '=')
		{
			if (file.peek() == '=')
			{
				file.get(key);
				mytoken.Token_name = "Equals Equals";
				mytoken.tok = EQUAL_EQUAL;
				return mytoken;
			}
			else
			{
				mytoken.tok = EQUAL;
				mytoken.Token_name = "Equal";
				return mytoken;
			}
			
		}


		else if (key == '&')
		{
			if (file.peek()=='&')
			{
				file.get(key);
				mytoken.Token_name = "AND";
				mytoken.tok = AND;
				return mytoken;
			}
			mytoken.tok = BITWISE_AND;
			mytoken.Token_name = "Bitwise AND";
			return mytoken;
		}

		else if (key == '|')
		{

			if (file.peek() == '|')
			{
				file.get(key);
				mytoken.Token_name = "OR";
				mytoken.tok = OR;
				return mytoken;
			}
			mytoken.tok = BITWISE_OR;
			mytoken.Token_name = "Bitwise OR";
			return mytoken;
		}


		else if (key == '/')
		{
			if (file.peek()=='/')
			{
				file.get(key);
				string skip;	
				getline(file,skip);
				mytoken.tok = COMMENT;
				mytoken.Token_name = "Comment";
				return mytoken;
			}
			else
			{
				mytoken.tok = DIVIDE;
				mytoken.Token_name = "Divide";
				return mytoken;
			}


		}

		else if (key == ';')
		{
			mytoken.tok = SEMICOLON;
			mytoken.Token_name = "Semi-Colon";
			return mytoken;
		}

		else if (key == ':')
		{

			if (file.peek()==':')
			{
				file.get(key);
				mytoken.Token_name = "Scope Resolution Operator";
				mytoken.tok = SCOPE_RESOLUTION;
				return mytoken;
			}
			else
			{
				mytoken.Token_name = "COLON";
				mytoken.tok = COLON;
				return mytoken;
			}

		}

		else if (key == '+')
		{

			if (file.peek() == '+')
			{
				file.get(key);
				mytoken.Token_name = "Increment";
				mytoken.tok = INCREMENT;
				return mytoken;
			}
			else
			{
				mytoken.Token_name = "PLUS";
				mytoken.tok = PLUS;
				return mytoken;
			}

		}


		else if (key == '-')
		{
			if (file.peek() == '-')
			{
				file.get(key);
				mytoken.Token_name = "Decrement";
				mytoken.tok = DECREMENT;
				return mytoken;
			}

			mytoken.Token_name = "MINUS";
			mytoken.tok = MINUS;
			return mytoken;
		}



		else if (key == '*')
		{
			mytoken.Token_name = "MULTIPLY";
			mytoken.tok = MULTIPLY;
			return mytoken;
		}

		else if (key == '%')
		{
		mytoken.Token_name = "MODULUS";
		mytoken.tok = MODULUS;
		return mytoken;
		}

		//Handling digits before variable and identifier so that tokens like "23rar"(int & var) and "rar23"(var) are correctly recognised
		else if (isdigit(key))
		{
		    string val(1,key);
				
			//While continuous digits are present get key and store them in val string	
			while (isdigit(key) && isdigit(file.peek()) && file.peek()!=EOF)
			{
				val += key;
				file.get(key);
			}

			mytoken.tok = CONSTANT;
			mytoken.Token_name = "Constant";
			//type casting string to int
			mytoken.value = stoi(val);
			return mytoken;
			
		}

		//Reading whole literal from end to end
		else if (key == '"')
		{
			file.get(key);
			while (key != '"')
			{
				file.get(key);
			}
			mytoken.tok = LITERAL;
			mytoken.Token_name = "literal";
			return mytoken;
		}

		//IF NOTHING MATCHES THEN WE WILL CHECK FOR KEYWORDS OR IDENTIFIERS

		else
		{

			while ((isalpha(key) || isdigit(key)) && !file.eof())
			{
				toCheck.push_back(key);     //continue for continuous alphanumeric characters
				file.get(key);
			}

			//Since while loop will call get function one extra time we will put it back
			if(!file.eof()) file.unget();

			//First check if the string matches any keyword
			if (toCheck == "float")
			{
				mytoken.Token_name = "float";
				mytoken.tok = FLOAT;
				toCheck.clear();
				return mytoken;
			}
			else if (toCheck == "int")
			{
				mytoken.Token_name = "int";
				mytoken.tok = INT;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "cout")
			{
				mytoken.Token_name = "cout";
				mytoken.tok = COUT;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "cin")
			{
				mytoken.Token_name = "cin";
				mytoken.tok = CIN;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "char")
			{
				mytoken.Token_name = "char";
				mytoken.tok = CHAR;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "double")
			{
				mytoken.Token_name = "double";
				mytoken.tok = DOUBLE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "long")
			{
				mytoken.Token_name = "long";
				mytoken.tok = LONG;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "if")
			{
				mytoken.Token_name = "if";
				mytoken.tok = IF;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "main")
			{
				if (holder == "int")
				{
					mytoken.Token_name = "main";
					mytoken.tok = MAIN;
					holder = mytoken.Token_name;
					toCheck.clear();
					return mytoken;
				}
				else
				{
					mytoken.Token_name = "main";
					mytoken.tok = MAIN;
					toCheck.clear();
					return mytoken;
				}
				
			}

			else if (toCheck == "return")
			{
				mytoken.Token_name = "return";
				mytoken.tok = RETURN;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "begin")
			{
				mytoken.Token_name = "begin";
				mytoken.tok = BEGIN;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "end")
			{
			mytoken.Token_name = "end";
			mytoken.tok = END;
			toCheck.clear();
			return mytoken;
			}

			else if (toCheck == "else")
			{
				mytoken.Token_name = "else";
				mytoken.tok = ELSE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "do")
			{
				mytoken.Token_name = "do";
				mytoken.tok = DO;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "while")
			{
				mytoken.Token_name = "while";
				mytoken.tok = WHILE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "default")
			{
				mytoken.Token_name = "default";
				mytoken.tok = DEFAULT;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "case")
			{
				mytoken.Token_name = "case";
				mytoken.tok = CASE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "bool")
			{
				mytoken.Token_name = "bool";
				mytoken.tok = BOOL;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "string")
			{
				mytoken.Token_name = "string";
				mytoken.tok = STRING;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "using")
			{
				mytoken.Token_name = "using";
				mytoken.tok = USING;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "namespace")
			{
				mytoken.Token_name = "namespace";
				mytoken.tok = NAMESPACE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "std")
			{
				mytoken.Token_name = "std";
				mytoken.tok = STD;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "this")
			{
				mytoken.Token_name = "this";
				mytoken.tok = THIS;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "unsigned")
			{
				mytoken.Token_name = "unsigned";
				mytoken.tok = UNSIGNED;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "signed")
			{
				mytoken.Token_name = "signed";
				mytoken.tok = UNSIGNED;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "switch")
			{
				mytoken.Token_name = "switch";
				mytoken.tok = SWITCH;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "void")
			{
				mytoken.Token_name = "void";
				mytoken.tok = VOID;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "class")
			{
				mytoken.Token_name = "class";
				mytoken.tok = CLASS;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "enum")
			{
				mytoken.Token_name = "enum";
				mytoken.tok = ENUM;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "struct")
			{
				mytoken.Token_name = "struct";
				mytoken.tok = STRUCT;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "public")
			{
				mytoken.Token_name = "public";
				mytoken.tok = PUBLIC;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "protected")
			{
				mytoken.Token_name = "protected";
				mytoken.tok = PROTECTED;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "public")
			{
				mytoken.Token_name = "public";
				mytoken.tok = PUBLIC;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "private")
			{
				mytoken.Token_name = "private";
				mytoken.tok = PRIVATE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "friend")
			{
				mytoken.Token_name = "friend";
				mytoken.tok = FRIEND;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "new")
			{
				mytoken.Token_name = "new";
				mytoken.tok = NEW;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "delete")
			{
				mytoken.Token_name = "delete";
				mytoken.tok = DELETE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "VECTOR")
			{
				mytoken.Token_name = "VECTOR";
				mytoken.tok = VECTOR;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "DEQUE")
			{
				mytoken.Token_name = "DEQUE";
				mytoken.tok = DEQUE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "LIST")
			{
				mytoken.Token_name = "LIST";
				mytoken.tok = LIST;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "const")
			{
				mytoken.Token_name = "const";
				mytoken.tok = CONST;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "typedef")
			{
				mytoken.Token_name = "typedef";
				mytoken.tok = TYPEDEF;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "continue")
			{
				mytoken.Token_name = "continue";
				mytoken.tok = CONTINUE;
				toCheck.clear();
				return mytoken;
			}

			else if (toCheck == "break")
			{
				mytoken.Token_name = "break";
				mytoken.tok = BREAK;
				toCheck.clear();
				return mytoken;
			}

			//If the string does not match for any keyword then it is a identifier
			else
			{
				//Write the identifier to file if not already present in file
				IsSymbolPresent(toCheck);

				mytoken.Token_name = "identifier";
				mytoken.idname = toCheck;
				mytoken.tok = IDENTIFIER;
				toCheck.clear();
				return mytoken;
			}

		}


}


//return token for the parser
Token gettoken() {
	if (flag == 0) {
		ret = lexical();
		return ret;
	}

	//else return the old token and reset flag switch
	flag = 0;
	return ret;
}

//Flag Switch so that only the old token is returned and not a new one from lexical
void ungettoken()
{
	flag = 1;
}




treenode* expression()
{
	treenode* t = new treenode;
	treenode* p = new treenode;
	int flage = 0;
	do
	{
		t = term();
		if (flage)
		{
			p->rightchild = t;
			flage = 0;
			t = p;
		}
		tokn = gettoken();
		if (tokn.tok == PLUS || tokn.tok == MINUS)
		{
			p = new treenode;
			p->kind = opk;
			p->opr = tokn.tok;
			p->name = tokn.Token_name;
			p->leftchild = t;
			flage = 1;
		}
	} while (tokn.tok == PLUS || tokn.tok == MINUS);
	ungettoken();
	return (t);
}

treenode* term()
{
	treenode* t = new treenode;
	treenode* p = new treenode;
	int flage = 0;
	do
	{
		t = factor();
		if (flage)
		{
			p->rightchild = t;
			flage = 0;
			t = p;
		}
		tokn = gettoken();
		if (tokn.tok == MULTIPLY || tokn.tok == DIVIDE)
		{
			p = new treenode;
			p->kind = opk;
			p->opr = tokn.tok;
			p->name = tokn.Token_name;
			p->leftchild = t;
			flage = 1;
		}
	} while (tokn.tok == MULTIPLY || tokn.tok == DIVIDE);
	ungettoken();
	return (t);

}



treenode* factor()
{
	treenode* t = new treenode;
	tokn = gettoken();
	switch (tokn.tok)
	{
	case CONSTANT:
		t = new treenode();
		t->kind = constk;
		t->value = tokn.value;
		t->name = "Constant";
		//t->opr=NUL;
		t->leftchild = NULL;
		t->rightchild = NULL;
		return(t);
		break;
	case IDENTIFIER:
		t = new treenode();
		t->kind = idk;
		//t->value=atoi(tok.name);
		t->name = tokn.idname;
		//t->opr=NUL;
		t->leftchild = NULL;
		t->rightchild = NULL;
		return(t);
		break;
	case  LEFT_ROUND_BRACES:
		return(t);
		break;
	}
}





//Recursive Parsive functions declaration for expression validation
//Implemented according to the precedence of operators
bool farrukhA();
bool farrukhB();
bool farrukhC();
bool farrukhD();
bool farrukhE();
bool farrukhF();
bool farrukhG();
bool farrukhH();


//Recursive parsive functions to check for statements in program
bool farrukhPROGRAM();
bool farrukhDECLARE();
bool farrukhSTATEMENTLIST();
bool farrukhSTATEMENT();
bool farrukhASSIGNMENT();


//Function that checks the complete program from beginning to end

bool farrukhPROGRAM()
{
	Token t;
	t = gettoken();
	if (t.tok == BEGIN)
	{
		if (farrukhDECLARE())
		{
			if (farrukhSTATEMENTLIST())
			{
				t = gettoken();
				return t.tok == END;
			}
			else return false;
		}
		else return false;
		
	}
	return false;
}

//Function that checks if the variable has been declared corectly
bool farrukhDECLARE()
{
	Token t;
	t = gettoken();
	if (t.tok == INT || t.tok == DOUBLE || t.tok == STRING || t.tok == FLOAT || t.tok == BOOL || t.tok == CHAR)
	{
		t = gettoken();
		if (t.tok == IDENTIFIER)
		{
			t = gettoken();
			if (t.tok == SEMICOLON) return true;
			else return false;
		}
		else return false;
	}
	else return false;

}


//Function to validate statement lists
bool farrukhSTATEMENTLIST()
{
	if (farrukhSTATEMENT())
	{
		return true;
	}
	else return false;
}

//Function to check valid statement
bool farrukhSTATEMENT()
{
	if (farrukhASSIGNMENT()) return true;
	else return false;
}

//Function to check the assignment statements such as a = 1  or temp = 1+3
bool farrukhASSIGNMENT()
{
	Token t;
	t = gettoken();
	if (t.tok == IDENTIFIER)
	{
		t = gettoken();
		if (t.tok == EQUAL)
		{
			if (farrukhA())
			{
				t = gettoken();
				return t.tok == SEMICOLON;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}





// Highest Precedence for OR first
bool farrukhA() {
	Token t;
	do
	{
		if (!farrukhB()) {
			return false;
		}
		t = gettoken();
	} while (t.tok==OR);
	ungettoken();
	return true;
}

// Second Highest precedence for AND
bool farrukhB() {
	Token t;
	do
	{
		if (!farrukhC()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == AND);
	ungettoken();
	return true;
}

//Third highest precedence for ==
bool farrukhC() {
	Token t;
	do
	{
		if (!farrukhD()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == EQUAL_EQUAL);
		ungettoken();
	return true;
}

//Fourth highest precedence for >,<,>=,<=
bool farrukhD() {
	Token t;
	do
	{
		if (!farrukhE()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == GREATER_THAN || t.tok == GREATER_THAN_EQUALS_TO || t.tok ==  LESS_THAN || t.tok == LESS_THAN_EQUALS_TO);
	ungettoken();
	return true;
}

//Fifth highest precedence for <<,>>
bool farrukhE() {

	Token t;
	do
	{
		if (!farrukhF()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == INSERTION || t.tok == EXERTION);
	ungettoken();
	return true;
}

//Sixth Highest precedence for +,-
bool farrukhF() {

	Token t;
	do
	{
		if (!farrukhG()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == PLUS || t.tok == MINUS);
	ungettoken();
	return true;
}

//Seventh highest precedence for *,/
bool farrukhG() {
	Token t;
	do
	{
		if (!farrukhH()) {
			return false;
		}
		t = gettoken();
	} while (t.tok == MULTIPLY || t.tok == DIVIDE);
	ungettoken();
	return true;
}

//Check for terminals
bool farrukhH() {
	Token t;
	t = gettoken();
	if (t.tok == CONSTANT || t.tok==IDENTIFIER) {
		return true;
	}
	else return false;
}

void printInorder(struct treenode* node)
{
	if (node == NULL)
		return;

	// First recur on left child
	printInorder(node->leftchild);

	// Then print the data of node
	if (node->kind == constk)
	{
		cout << node->value << " ";
	}

	else cout << node->name << " ";

	// Now recur on right child
	printInorder(node->rightchild);
}



int main()
{

	//Running parser to check for syntax
	/*if (farrukhA())
	{
		cout << "\nSyntax Correct";
	}
	else cout << "\nIncorrect syntax";*/


	//Displaying all the token in the file
	/*Token t;
	bool flager = false;
	while (!flager)
	{
		t = lexical();
		if (t.tok == GARBAGE) flager = 1;
		else cout << "\nToken found: " << t.Token_name;
	}*/


	/*if (farrukhPROGRAM())
	{
		cout << "\nSyntax correct";
	}
	else cout << "Incorrect syntax";*/


	//Generating an expression tree
	printInorder(expression());


	//After the program is complete close all the files
	CloseAllFiles();
	cout << endl << endl;
	system("pause");
	return 0;
}