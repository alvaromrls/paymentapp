extern "C"
{
#include "sqlite3.h"
}

#include "SQLiteFacade.h"
#include <iostream>
#include <sstream>
#include "AccountORM.h"

int main()
{
    SQLiteFacade database{"payments.db"};
    database.createTables();

    std::cout << "Welcome! Enter commands (PAY <user> <amount> / HISTORY <user> / EXIT):\n";

    std::string command;

    // AccountORM account;
    // account.setLimit(400);

    // database.saveData(account);

    AccountORM read;
    read.setId(2);
    database.loadData(read);

    std::cout << "Data is " << read.getLimit() << std::endl;

    return 0;
}
