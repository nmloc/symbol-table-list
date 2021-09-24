#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
struct CheckType
{
    int position;
    int block;
    string type;
};

class Symbol
{
public:
    string id;
    string type;
    Symbol *next;
    Symbol(string para1, string para2)
		{
				this->id = para1;
				this->type = para2;
				this->next = nullptr;
		}
};

class Table
{
public:
    Symbol *headSymbol;
    Symbol *tailSymbol;
    int size;
    Table *next;
    Table() {
        headSymbol = nullptr;
        tailSymbol = nullptr;
        next = nullptr;
        size = 0;
    }
    ~Table() {
        while (headSymbol)
            popHead();
    }
    void duplicate(Table *table) {
        Symbol *read = table->headSymbol;
        while (read)
        {
					addTail(read->id, read->type);
					read = read->next;
        }
    }
    void free() {
			while (headSymbol)
					popHead();
    }
    void printTable() {
        Symbol *read = headSymbol;
        while (read)
        {
            cout << read->id << "//" << read->type << " ";
            read = read->next;
        }
        cout << endl;
    }
    void addHead(string id, string type)
    {
        Symbol *entry = new Symbol(id, type);
        if (!headSymbol)
        {
            headSymbol = entry;
            tailSymbol = headSymbol;
        }
        else
        {
            entry->next = headSymbol;
            headSymbol = entry;
        }
        size++;
    }
    void addTail(string id, string type)
    {
        Symbol *entry = new Symbol(id, type);
        if (!headSymbol)
        {
            headSymbol = entry;
            tailSymbol = headSymbol;
        }
        else
        {
            tailSymbol->next = entry;
            tailSymbol = tailSymbol->next;
        }
        size++;
    }
    void removeAtPos(int pos)
    {
        if (pos == 1)
        {
            popHead();
        }
        else if (pos == size)
        {
            popTail();
        }
        else if (pos < size && pos > 1)
        {
            int count = 1;
            Symbol *current = headSymbol;
            Symbol *temp = headSymbol;
            while (current && count < pos)
            {
                temp = current;
                current = current->next;
                count++;
            }
            if (temp->next)
            {
                temp->next = temp->next->next;
            }
            delete current;
            size--;
        }
    }

    void popHead() {
        if (headSymbol) {
            Symbol *temp = headSymbol;
            headSymbol = headSymbol->next;
            delete temp;
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
				delete current;
				tailSymbol = beforeTail;
				size--;
			}
    }
    Table *toClone() {
			Table *result = new Table;
			Symbol *read = this->headSymbol;
			while (read) {
				result->addTail(read->id, read->type);
				read = read->next;
			}
			return result;
    }
		Table *toReverse() {
			Table *result = new Table;
			Symbol *read = this->headSymbol;
			while (read) {
				result->addHead(read->id, read->type);
				read = read->next;
			}
			return result;
    }
};

class SymbolTable
{
public:
    Table *headTable;
    Table *tailTable;
    int tableNum;

    SymbolTable()
    {
        headTable = nullptr;
        tailTable = nullptr;
        tableNum = 0;
        addTailTable();
    };
    ~SymbolTable()
    {
        while (headTable || tailTable)
            this->removeTailTable();
    }
    void addTailTable()
    {
        Table *entry = new Table();
        if (!headTable)
        {
            headTable = entry;
            tailTable = headTable;
        }
        else
        {
            tailTable->next = entry;
            tailTable = tailTable->next;
            ;
        }
        tableNum++;
    }
    bool removeTailTable()
    {
        if (headTable)
        {
            Table *beforeTail = nullptr;
            Table *current = headTable;
            while (current != tailTable)
            {
                beforeTail = current;
                current = current->next;
            }
            if (!beforeTail)
            {
                headTable = nullptr;
            }
            else
            {
                beforeTail->next = nullptr;
            }
            delete tailTable;
            tailTable = beforeTail;
            return (tableNum-- > 1 ? true : false);
        }
        else
            return false;
    }
    inline void check(string cmd, string param1, string param2);
    inline void insert(string id, string type);
    inline void assign(string id, string value);
    inline void begin();
    inline bool end();
    inline void lookup(string id);
    inline Table *fetch();
    inline void print();
    inline void rprint();
    inline void run(string filename);
};

inline CheckType *findBetweenTables(Table *symbolTable, string id, int currentBlock);

#endif