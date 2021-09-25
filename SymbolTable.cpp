#include "SymbolTable.h"


void SymbolTable::insert(string id, string type) {
	tailTable->addTail(id, type, scope);
	Symbol *current = tailTable->headSymbol;
	while (current != tailTable->tailSymbol) {
		if (current->id == tailTable->tailSymbol->id) {
			string error = "INSERT " + id + " " + type;
			throw Redeclared(error);
		}
		current = current->next;
	}
		cout << "success" << endl;
}

void SymbolTable::assign(string id, string value) {
	regex rNum("[0-9]+");
	regex rString("['][ a-zA-Z0-9]+[']");
	regex rId("[a-z][_a-zA-Z0-9]*");
	string error = "ASSIGN " + id + " " + value;
	Symbol *temp = checkId_STL(id);
	if (regex_match(value, rNum)) {
		if (temp == nullptr)	{
			throw Undeclared(error);
		}
		else {
			if (temp->type == "number") {
				cout << "success" << endl;
			}
			else {
				throw TypeMismatch(error);
			}
		}
	}
	else if (regex_match(value, rString)) {
		if (temp == nullptr) {
			throw Undeclared(error);
		}
		else {
			if (temp->type == "string") {
				cout << "success" << endl;
			}
			else {
				throw TypeMismatch(error);
			}
		}
	}
}
void SymbolTable::begin() {
	this->addTailTable();
}

void SymbolTable::end() { 
	if (scope <= 0)
		throw UnknownBlock();
	else
		this->removeTailTable();
}

void SymbolTable::lookup(string id) {
	Symbol *temp = checkId_DL(id);
	if (temp != nullptr) {
		cout << temp->scopeLevel << endl;
	}
	else {
		string error = "LOOKUP " + id;
		throw Undeclared(error);
	}
}

void SymbolTable::print() {
	Table *result = DeduplicateList();
	Symbol *read = result->headSymbol;
	while (read) {
		cout << read->id << "//" << read->scopeLevel << " ";
		read = read->next;
	}
	cout << endl;
	delete result;
}

void SymbolTable::rprint() {
  Table *result = DeduplicateList(true);
	Symbol *read = result->headSymbol;
	while (read) {
		cout << read->id << "//" << read->scopeLevel << " ";
		read = read->next;
	}
	cout << endl;
	delete result;
}

void SymbolTable::check(string cmd, string para1, string para2) {
	if (cmd == "INSERT")
		insert(para1, para2);
	else if (cmd == "ASSIGN")
		assign(para1, para2);
	else if (cmd == "BEGIN")
		begin();
	else if (cmd == "END")
		end();
	else if (cmd == "LOOKUP")
		lookup(para1);
	else if (cmd == "PRINT")
		print();
	else if (cmd == "RPRINT")
		rprint();
	else {
		string error = cmd + " " + para1 + " " + para2;
		throw InvalidInstruction(error);
	}
}

void SymbolTable::run(string filename) {
	ifstream openFile(filename);
	if (openFile.is_open())
	{
		string buffer;
		while (getline(openFile, buffer)) {
			string inputString[3] = {"", "", ""};
			int beginPos = 0, currPos = 0, numOfChar = 0, paramNum = 0, strLen = buffer.length();
			while (true) {
				if (currPos >= strLen) {
					inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
					break;
				}
				if (buffer[currPos] == ' ') {
					inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
					beginPos = currPos + 1;
					numOfChar = 0;
				}
				else {
					numOfChar++;
				}
				currPos++;
			}
			if (inputString[0] != "")	{
				check(inputString[0], inputString[1], inputString[2]);
			}
		}
		if (scope > 0)
			throw UnclosedBlock(scope);
		openFile.close();
	}
}