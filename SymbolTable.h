#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Symbol {
public:
	string id;
	string type;
	int scopeLevel;
	Symbol *next;
	Symbol(string para1, string para2, int para3) {
		this->id = para1;
		this->type = para2;
		this->scopeLevel = para3;
		this->next = nullptr;
	}
};

class Table {
public:
	Symbol *headSymbol;
	Symbol *tailSymbol;
	Table *next;
	int size;
	Table() {
		this->headSymbol = nullptr;
		this->tailSymbol = nullptr;
		this->next = nullptr;
		this->size = 0;
	}
	~Table() {
		while (headSymbol)
			popHead();
	}
	void printTable() {
		Symbol *read = headSymbol;
		while (read) {
			cout << read->id << "//" << read->scopeLevel << " ";
			read = read->next;
		}
		cout << endl;
	}
	void addHead(string id, string type, int scopeLevel) {
		Symbol *entry = new Symbol(id, type, scopeLevel);
		if (!headSymbol) {
			headSymbol = entry;
			tailSymbol = headSymbol;
		}
		else {
			entry->next = headSymbol;
			headSymbol = entry;
		}
		size++;
	}
	void addTail(string id, string type, int scopeLevel) {
		Symbol *entry = new Symbol(id, type, scopeLevel);
		if (!headSymbol) {
			headSymbol = entry;
			tailSymbol = headSymbol;
		}
		else {
			tailSymbol->next = entry;
			tailSymbol = tailSymbol->next;
		}
		size++;
	}

	void popHead() {
			if (headSymbol) {
					headSymbol = headSymbol->next;
					size--;
			}
	}
	void popTail() {
		if (headSymbol) {
			Symbol *beforeTail = nullptr;
			Symbol *current = headSymbol;
			while (current != tailSymbol) {
				beforeTail = current;
				current = current->next;
			}
			if (!beforeTail) {
				headSymbol = beforeTail;
			}
			else {
				beforeTail->next = nullptr;
			}
			tailSymbol = beforeTail;
			size--;
		}
	}
};

class SymbolTable {
public:
	Table *headTable;
	Table *tailTable;
	int scope;

	SymbolTable() {
		headTable = nullptr;
		tailTable = nullptr;
		scope = -1;
		addTailTable();
	};

	~SymbolTable() {
		while (headTable || tailTable)
			this->removeTailTable();
	};

	void addTailTable() {
		Table *entry = new Table();
		if (!headTable) {
			headTable = entry;
			tailTable = headTable;
		}
		else {
			tailTable->next = entry;
			tailTable = tailTable->next;
		}
		scope++;
	};

	void removeTailTable() {
		if (headTable) {
			Table *beforeTail = nullptr;
			Table *current = headTable;
			while (current != tailTable) {
				beforeTail = current;
				current = current->next;
			}
			if (!beforeTail) {
				headTable = nullptr;
			}
			else {
				beforeTail->next = nullptr;
			}
			delete tailTable;
			tailTable = beforeTail;
			scope--;
		}
	};

	Symbol *checkId_STL(string id) {
		Table *temp = SymbolTableList();
		Symbol *currentSymbol = temp->headSymbol;
		while (currentSymbol) {
			if (currentSymbol->id == id) {
				delete temp;
				return currentSymbol;
			}
			currentSymbol = currentSymbol->next;
		}
		return nullptr;
	};

	Table *SymbolTableList(bool reversed = false) {
		Table *result = new Table();
		Table *currentTable = headTable;
		while (currentTable) {
			Symbol *currentSymbol = currentTable->headSymbol;
			while (currentSymbol) {
				if (reversed) {
					result->addHead(currentSymbol->id, currentSymbol->type, currentSymbol->scopeLevel);
				}
				else {
					result->addTail(currentSymbol->id, currentSymbol->type, currentSymbol->scopeLevel);
				}
				currentSymbol = currentSymbol->next;
			}
			currentTable = currentTable->next;
		}
		return result;
	};

	Symbol *checkId_DL(string id) {
		Table *temp = DeduplicateList();
		Symbol *currentSymbol = temp->headSymbol;
		while (currentSymbol) {
			if (currentSymbol->id == id) {
				delete temp;
				return currentSymbol;
			}
			currentSymbol = currentSymbol->next;
		}
		return nullptr;
	};

	bool existInDeduplicateList(string id, Table *table) {
		Symbol *currentSymbol = table->headSymbol;
		while (currentSymbol) {
			if (currentSymbol->id == id) {
				return true;
			}
			currentSymbol = currentSymbol->next;
		}
		return false;
	};

	Table *DeduplicateList(bool reversed = false) {
		Table *record = SymbolTableList(true);
		Table *result = new Table();
		Symbol *currentSymbol = record->headSymbol;
		while (currentSymbol)	{
			bool isDuplicate = existInDeduplicateList(currentSymbol->id, result);
			if (isDuplicate)
				currentSymbol = currentSymbol->next;
			else {
				if (!reversed) {
				result->addHead(currentSymbol->id, currentSymbol->type, currentSymbol->scopeLevel);
				}
				else {
					result->addTail(currentSymbol->id, currentSymbol->type, currentSymbol->scopeLevel);
				}
				currentSymbol = currentSymbol->next;
			}
		}
		delete record;
		return result;
	};

	inline void check(string cmd, string param1, string param2);
	inline void insert(string id, string type);
	inline void assign(string id, string value);
	inline void begin();
	inline void end();
	inline void lookup(string id);
	inline void print();
	inline void rprint();
	inline void run(string filename);
};
#endif