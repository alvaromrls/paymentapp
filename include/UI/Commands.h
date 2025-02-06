#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include "DataParser.h"

// Base class for Command Patern
class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// Command Patern for Line Parser
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

// Service Interface to calculate the Total Founds
class CalculateTotalFounds
{
public:
    virtual ~CalculateTotalFounds() = default;
    // Get available founds (limit - expended). Negative value means expired card.
    virtual float getFounds(const std::string &cardNumber) = 0;
};

// Service Interface to calculate the Total Founds
class FindCardFee
{
public:
    virtual ~FindCardFee() = default;
    // Get the feed asociated to a card. 0 value means no card found.
    virtual int getFee(const std::string &cardNumber) = 0;
};

// Service Interface to add a new Transaction
class AddNewTransaction
{
public:
    virtual ~AddNewTransaction() = default;
    // Save into DB a new transaction
    virtual void addTransaction(const std::string &cardNumber,
                                const std::string MerchantType,
                                float amount) = 0;
};

// PAY command
const std::string PAY_COMMAND_HELP = "Request a new payment. It will ask for the Amount, the Card Number, and the Commerce Type.";
class PayCommand : public LineParserCommand
{
    // Services
    std::unique_ptr<CalculateTotalFounds> totalFoundsService;
    std::unique_ptr<FindCardFee> findCardFeeService;
    std::unique_ptr<AddNewTransaction> newTransactionService;

    // Merchant map (Name to Fee)
    std::map<std::string, int> merchantTaxMap;

    // Data parsers
    CreditCardParser creditcardparser;
    MoneyParser moneyParser;
    MerchantParser merchantParser;

public:
    PayCommand(std::map<std::string, int> merchantTaxMap,
               std::unique_ptr<CalculateTotalFounds> totalFoundsService,
               std::unique_ptr<FindCardFee> findCardFeeService,
               std::unique_ptr<AddNewTransaction> newTransactionService)
        : LineParserCommand(PAY_COMMAND_HELP),
          merchantTaxMap(merchantTaxMap),
          totalFoundsService(std::move(totalFoundsService)),
          findCardFeeService(std::move(findCardFeeService)),
          newTransactionService(std::move(newTransactionService)),
          creditcardparser(),
          merchantParser(merchantTaxMap) {};

    void execute() override
    {
        std::string cardNumber;
        std::cout << "Executing payment...\n";

        // 1. Ask for credit cartd
        creditcardparser.printCardFormat();
        auto card = creditcardparser.askForCard();
        if (card.has_value())
        {
            cardNumber = card.value();
        }

        // 1.1 Get fee from given card
        int fee = findCardFeeService->getFee(cardNumber);
        if (fee == 0)
        {
            std::cout << std::string(40, '-') << "\n";
            std::cout << "[Error]: Card not found! \n";
            return;
        }

        // 1.2 Check available founds from given card
        float founds = totalFoundsService->getFounds(cardNumber);
        if (founds < 0)
        {
            std::cout << std::string(40, '-') << "\n";
            std::cout << "[Error]: Expired card \n";
            return;
        }

        // 2. Ask for the amount
        std::cout << std::string(40, '-') << "\n";
        moneyParser.printFormat();
        float amount = moneyParser.askForMoney();
        if (amount < 0)
        {
            return;
        }

        // 3. Ask for Merchant amount
        merchantParser.printFormat();
        auto optional_merchant = merchantParser.askForMerchant();
        std::string merchant;
        if (optional_merchant.has_value())
        {
            merchant = optional_merchant.value();
        }
        else
        {
            return;
        }

        // 4. Calculate total feeds and total amount
        float amount_card_fee = amount * float(fee / 100.f);
        float amount_merchant_fee = amount * float(merchantTaxMap[merchant] / 100.f);
        float total_amount = amount + amount_card_fee + amount_merchant_fee;

        // 4.1 Print all the information to user
        std::cout << std::string(40, '-') << "\n"
                  << "Initial Amount:\t\t" << amount << "\n"
                  << "Card fee (%):\t\t" << fee << "\n"
                  << "Card fee (total):\t" << amount_card_fee << "\n"
                  << "Merchant fee (%):\t" << merchantTaxMap[merchant] << "\n"
                  << "Merchant fee (total):\t" << amount_merchant_fee << "\n"
                  << std::string(40, '-') << "\n"
                  << "TOTAL:\t\t\t" << total_amount << " (Euros)\n"
                  << std::string(40, '-') << "\n"
                  << "Available Founds:\t" << founds << " (Euros)\n"
                  << std::string(40, '-') << "\n";

        // 5. Check if it's posible to pay
        if (total_amount > founds)
        {
            std::cout << "[Error]: Not enought founds\n";
            return;
        }

        // 6. Execute the payment (storing it in DB)
        std::cout << "Storing transaction ...\n";
        newTransactionService->addTransaction(cardNumber, merchant, total_amount);
        std::cout << "Transaction stored! \n";
        std::cout << std::string(40, '-') << "\n";
        std::cout << "Balance after transaction:\t" << founds - total_amount << "\n";
        std::cout << std::string(40, '-') << "\n";
    }
};

// Amount - Merchant - DateTime
using TransactionHistory = std::tuple<float, std::string, std::string>;

// Service Interface to get all the transactions from a given card number
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
    // Validators
    std::unique_ptr<CreditCardValidator> CardValidator;

    // Service
    std::unique_ptr<ReadTransactionHistory> service;

    // Parsers
    CreditCardParser creditcardparser;
    DateParser dateParser;

public:
    HistoryCommand(std::unique_ptr<ReadTransactionHistory> service)
        : LineParserCommand(HISTORY_COMMAND_HELP), service(std::move(service)),
          creditcardparser(), dateParser()
    {
        CardValidator = std::make_unique<CreditCardValidator>();
    }

    void execute() override
    {
        std::string startingDate;
        std::string endingDate;
        std::string cardNumber;

        // Asking for Credit Card Number
        creditcardparser.printCardFormat();
        auto card = creditcardparser.askForCard();
        if (card.has_value())
        {
            cardNumber = card.value();
        }
        else
        {
            return;
        }

        // Asking for Date Interval
        dateParser.printDateFormat();
        startingDate = dateParser.askForDates("starting", "1990-01-01");
        endingDate = dateParser.askForDates("ending", "2300-01-01");

        if (startingDate >= endingDate) // Invalid range
        {
            std::cout << std::string(40, '-') << "\n";
            std::cout << "[Error]: starting date should be before endingDate \n";
            std::cout << std::string(40, '-') << "\n";
            return;
        }

        // Get data from given card and date range
        auto searchResult = service->read(cardNumber, startingDate, endingDate);

        std::cout << std::string(40, '-') << "\n";

        // Empty case
        if (searchResult.empty())
        {
            std::cout << "Nothing found with the given data \n";
            return;
        }

        // Print transactions
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

using ReadCards = std::tuple<std::string, std::string, std::string, std::string>;

// Placeholder for reading data
class ReadAvailableCards
{
public:
    // AVAILABLE - CARD NUMBER - USER NAME - ISSUER TYPE
    virtual std::vector<ReadCards> read() = 0;
    virtual ~ReadAvailableCards() = default;
};

// SHOW DUMMY COMMAND
const std::string SHOW_DUMMY_COMMAND_HELP = "Shows all added Card numbers. [For this POC version]. ";
class ShowDummyCommand : public LineParserCommand
{
    // Services
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
            auto [number, user, issuer, isDispo] = card;
            std::cout << "[" << isDispo << "]" << " Card Number: " << number << " ---- Owner: " << user << " ---- Type: " << issuer << "\n";
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