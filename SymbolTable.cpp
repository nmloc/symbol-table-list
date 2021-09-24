#include "SymbolTable.h"

CheckType *findBetweenTables(Table *table, string id, int currentBlock)
{
    int traversingBlock = currentBlock;
    Table *currentTable = table;
    CheckType *result = new CheckType;
    result->block = -1;
    result->position = 1;
    while (currentTable)
    {
        Symbol *currentSymbol = currentTable->headSymbol;
        while (currentTable)
        {
            if (currentSymbol->id == id)
            {
							result->block = currentBlock - traversingBlock;
							result->type = currentSymbol->type;
            }
            result->position++;
            currentSymbol = currentSymbol->next;
        }
        traversingBlock--;
        currentTable = currentTable->next;
    }
    return result;
}

void SymbolTable::insert(string id, string type)
{
    tailTable->addTail(id, type);
    Symbol *current = tailTable->headSymbol;
    while (current != tailTable->tailSymbol)
    {
        if (current->id == tailTable->tailSymbol->id)
        {
            string error = "INSERT " + id + " " + type;
            throw Redeclared(error);
        }
        current = current->next;
    }
    cout << "success" << endl;
}

void SymbolTable::assign(string id, string value)
{
    regex rNum("[0-9]+");
    regex rString("['][ a-zA-Z0-9]+[']");
    regex rId("[a-z][_a-zA-Z0-9]*");
    string error = "ASSIGN " + id + " " + value;
    if (regex_match(value, rNum))
    {
        CheckType *checkId = findBetweenTables(this->headTable, id, this->tableNum);
        if (checkId->block == -1)
        {
            delete checkId;
            throw Undeclared(error);
        }
        else
        {
            if (checkId->type == "number")
            {
                cout << "success" << endl;
                delete checkId;
            }
            else
            {
                delete checkId;
                throw TypeMismatch(error);
            }
        }
    }
    else if (regex_match(value, rString))
    {
        CheckType *checkId = findBetweenTables(this->headTable, id, this->tableNum);
        if (checkId->block == -1)
        {
            delete checkId;
            throw Undeclared(error);
        }
        else
        {
            if (checkId->type == "string")
            {
                delete checkId;
                cout << "success" << endl;
            }
            else
            {
                delete checkId;
                throw TypeMismatch(error);
            }
        }
    }
    else if (regex_match(value, rId))
    {
        CheckType *checkValue = findBetweenTables(this->headTable, value, this->tableNum);
        CheckType *checkId = findBetweenTables(this->headTable, id, this->tableNum);
        if (checkValue->block == -1)
        {
            delete checkValue;
            delete checkId;
            throw Undeclared(value);
        }
        else
        {
            if (checkId->block == -1)
            {
                delete checkValue;
                delete checkId;
                throw Undeclared(id);
            }
            else
            {
                if (checkValue->type == checkId->type)
                {
                    cout << "success" << endl;
                    delete checkValue;
                    delete checkId;
                }
                else
                {
                    delete checkValue;
                    delete checkId;
                    throw TypeMismatch(error);
                }
            }
        }
    }
    else
    {
        throw TypeMismatch(error);
    }
}
void SymbolTable::begin() { this->addTailTable(); }

bool SymbolTable::end() { return this->removeTailTable(); }

void SymbolTable::lookup(string id)
{
    CheckType *output = findBetweenTables(this->headTable, id, this->tableNum);
    if (output->block != -1)
    {
        cout << output->block << endl;
        delete output;
    }
    else
    {
        delete output;
        string error = "LOOKUP " + id;
        throw Undeclared(error);
    }
}

Table *SymbolTable::fetch()
{
    int tempTableNum = this->tableNum;
    Table *currentTable = headTable;
    Table *scopeTable = new Table();
    while (currentTable)
    {
        Symbol *currentSymbol = currentTable->headSymbol;
        while (currentTable)
        {
            scopeTable->addTail(currentSymbol->id, to_string(this->tableNum - tempTableNum));
            currentSymbol = currentSymbol->next;
        }
        tempTableNum--;
        currentTable = currentTable->next;
    }
    Table *rScopeTable = scopeTable->toReverse();
    Table *result = new Table();
		//remove redeclared symbol
		Table *clonedTable = rScopeTable->toClone();
		int duplicatePos = 2;
		Symbol *duplicateSymbol = rScopeTable->headSymbol->next;
		while (duplicateSymbol)
		{
				if (duplicateSymbol->id == rScopeTable->headSymbol->id)
				{
						clonedTable->removeAtPos(duplicatePos);
						duplicatePos--;
				}
				duplicateSymbol = duplicateSymbol->next;
				duplicatePos++;
		}
		//paste completed table to pointer result
    while (clonedTable->headSymbol)
    { 
        result->addHead(clonedTable->headSymbol->id, clonedTable->headSymbol->type);
        clonedTable->popHead();
    }
		delete currentTable;
    delete rScopeTable;
    delete scopeTable;
		delete clonedTable;
		delete duplicateSymbol;
    return result;
}



void SymbolTable::print()
{
    Table *result = fetch();
    if (result->size)
        result->printTable();
    delete result;
}

void SymbolTable::rprint()
{
    Table *reversedResult = fetch();
    Table *result = reversedResult->toReverse();
    if (result->size)
        result->printTable();
    delete reversedResult;
    delete result;
}

void SymbolTable::check(string cmd, string para1, string para2)
{
    if (cmd == "INSERT")
    {
        insert(para1, para2);
    }
    else if (cmd == "ASSIGN")
    {
        assign(para1, para2);
    }
    else if (cmd == "BEGIN")
    {
        begin();
    }
    else if (cmd == "END")
    {
        if (!end())
        {
            throw UnknownBlock();
        }
    }
    else if (cmd == "LOOKUP")
    {
        lookup(para1);
    }
    else if (cmd == "PRINT")
    {
        print();
    }
    else if (cmd == "RPRINT")
    {
        rprint();
    }
    else
    {
        string error = cmd + " " + para1 + " " + para2;
        throw InvalidInstruction(error);
    }
}

void SymbolTable::run(string filename)
{
    ifstream openFile(filename);
    if (openFile.is_open())
    {
        string buffer;
        while (getline(openFile, buffer))
        {
            string inputString[3] = {"", "", ""};
            int beginPos = 0, currPos = 0, numOfChar = 0, paramNum = 0, strLen = buffer.length();
            while (true)
            {
                if (currPos >= strLen)
                {
                    inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
                    break;
                }
                if (buffer[currPos] == ' ')
                {
                    inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
                    beginPos = currPos + 1;
                    numOfChar = 0;
                }
                else
                {
                    numOfChar++;
                }
                currPos++;
            }
            if (inputString[0] != "")
                check(inputString[0], inputString[1], inputString[2]);
        }
        if (tableNum > 1)
        {
            int errorBlock = tableNum - 1;
            throw UnclosedBlock(errorBlock);
        }
        openFile.close();
    }
}