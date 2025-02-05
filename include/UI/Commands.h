#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <optional>

class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class LineParserCommand : public Command
{
    const std::string help;

public:
    LineParserCommand(std::string help) : help{help} {};
    void printHelp()
    {
        std::cout << help << "\n";
    }
};

// PAY command
const std::string PAY_COMMAND_HELP = "Request a new payment. It will ask for the Amount, the Card Number, and the Commerce Type.";
class PayCommand : public LineParserCommand
{
public:
    PayCommand() : LineParserCommand(PAY_COMMAND_HELP) {};
    void execute() override
    {
        std::cout << "Executing payment...\n";
    }
};

// Amount - Merchant - DateTime
using TransactionHistory = std::tuple<float, std::string, std::string>;

class ReadTransactionHistory
{
public:
    virtual ~ReadTransactionHistory() = default;
    virtual std::vector<TransactionHistory> read(const std::string &cardNumber,
                                                 const std::string &startingDate,
                                                 const std::string endingDate) = 0;
};

// HISTORY command
const std::string HISTORY_COMMAND_HELP = "Given a Credit Card, it will show the transaction history. Optional: date range.";
class HistoryCommand : public LineParserCommand
{
    std::unique_ptr<DateTimeSQLValidator> DateValidator;
    std::unique_ptr<CreditCardValidator> CardValidator;
    std::unique_ptr<ReadTransactionHistory> service;
    void printDateFormat()
    {
        std::cout << "->Time format is YYYY-MM-DD (fill with 0's if needed)\n";
    }

    std::string askForDates(const std::string &limit, const std::string &ifEmpty)
    {
        while (true)
        {
            std::cout << "Please select a " << limit << " date for the request (empty if not important):";
            std::string userInput;
            std::getline(std::cin, userInput);
            // std::cout << userInput << std::endl;
            if (userInput == "")
            {
                return ifEmpty;
            }
            else if (DateValidator->validate(userInput))
            {
                return userInput;
            }
            else
            {
                printDateFormat();
            }
        }
    }

    void printCardFormat()
    {
        std::cout << "->Credit card format is 'XXXX-XXXX' \n";
    }

    std::optional<std::string> askForCard()
    {
        while (true)
        {
            std::cout << "Please enter your credit card number (empty for leaving):";
            std::string userInput;
            std::getline(std::cin, userInput);
            if (userInput == "")
            {
                return {};
            }
            else if (CardValidator->validate(userInput))
            {
                return userInput;
            }
            else
            {
                printCardFormat();
            }
        }
    }

public:
    HistoryCommand(std::unique_ptr<ReadTransactionHistory> service)
        : LineParserCommand(HISTORY_COMMAND_HELP), service(std::move(service))
    {
        DateValidator = std::make_unique<DateTimeSQLValidator>();
        CardValidator = std::make_unique<CreditCardValidator>();
    }

    void execute() override
    {
        std::string startingDate;
        std::string endingDate;
        std::string cardNumber;

        // Asking for Credit Card Number
        printCardFormat();
        auto card = askForCard();
        if (card.has_value())
        {
            cardNumber = card.value();
        }
        else
        {
            return;
        }

        // Asking for Date Interval
        printDateFormat();
        startingDate = askForDates("starting", "1990-01-01");
        endingDate = askForDates("ending", "2300-01-01");
        auto searchResult = service->read(cardNumber, startingDate, endingDate);

        std::cout << std::string(40, '-') << "\n";

        if (searchResult.empty())
        {
            std::cout << "Nothing found with the given data \n";
            return;
        }

        std::cout << searchResult.size() << " transaction(s) found \n";
        std::cout << std::string(40, '-') << "\n";
        int i = 1;
        for (const auto &transaction : searchResult)
        {
            auto [amount, merchant, date] = transaction;
            std::cout << i << ". [" << date << "]: "
                      << std::fixed << std::setprecision(2) << amount << " Euros at " << merchant << "\n";
            i++;
        }
        std::cout << std::string(40, '-') << "\n";
    }
};

// Placeholder for reading data
class ReadAvailableCards
{
public:
    // CARD NUMBER - USER NAME - ISSUER TYPE
    virtual std::vector<std::tuple<std::string, std::string, std::string>> read() = 0;
    virtual ~ReadAvailableCards() = default;
};

// SHOW DUMMY COMMAND
const std::string SHOW_DUMMY_COMMAND_HELP = "It Will show all already added Card numbers. [For this POC version]. ";
class ShowDummyCommand : public LineParserCommand
{
    std::unique_ptr<ReadAvailableCards> readCommand;

public:
    ShowDummyCommand(std::unique_ptr<ReadAvailableCards> readCommand) : LineParserCommand(SHOW_DUMMY_COMMAND_HELP), readCommand(std::move(readCommand)) {};
    void execute() override
    {
        std::cout << "Showing Dummy Cards Number [so you can test one]\n";
        std::cout << std::string(40, '-') << "\n";
        auto seachResult = readCommand->read();
        for (const auto &card : seachResult)
        {
            auto [number, user, issuer] = card;
            std::cout << "Card Number: " << number << " ---- Owner: " << user << " ---- Type: " << issuer << "\n";
        }
    }
};

//  HELP Command
const std::string HELP_COMMAND_HELP = "Displays the help for each command.";
class HelpCommand : public LineParserCommand
{
private:
    std::unordered_map<std::string, std::unique_ptr<LineParserCommand>> *commands;

public:
    HelpCommand(std::unordered_map<std::string, std::unique_ptr<LineParserCommand>>
                    *cmds) : LineParserCommand(HELP_COMMAND_HELP), commands{cmds} {}

    void execute() override
    {
        std::cout << "Available Commands:\n";
        std::cout << std::string(40, '-') << "\n";

        const int columnWidth = 20; // Adjust this to align properly

        for (const auto &command : *commands)
        {
            std::cout << "[" << command.first << "]"
                      << std::string(columnWidth - command.first.length(), '-') // Ensures uniform spacing
                      << " ";
            command.second->printHelp();
        }
    }
};

// EXIT Command
const std::string EXIT_COMMAND_HELP = "Finish the program execution";
class ExitCommand : public LineParserCommand
{
private:
    bool &activeFlag;

public:
    ExitCommand(bool &flag) : activeFlag(flag), LineParserCommand(EXIT_COMMAND_HELP) {}

    void execute() override
    {
        std::cout << "Exiting...\n";
        activeFlag = false;
    }
};