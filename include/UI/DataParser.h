#pragma once

#include <optional>
#include <memory>
#include <map>
#include <iomanip>

#include "InputValidator.h"

// Class to get a valid Credit card from user interface.
class CreditCardParser
{
    std::unique_ptr<CreditCardValidator> CardValidator;

public:
    CreditCardParser() : CardValidator(std::make_unique<CreditCardValidator>()) {};

    void printCardFormat()
    {
        std::cout << "->Credit card format is 'XXXX-XXXX' \n";
    }

    // Ask in a loop until gets a valid card format or null input.
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
};

// Class to get a valid Credit card from user interface.
class MoneyParser
{
    std::unique_ptr<MoneyValidator> moneyValidator;

public:
    MoneyParser() : moneyValidator(std::make_unique<MoneyValidator>()) {};

    void printFormat()
    {
        std::cout << "->Money format is 100, 100.5, 100.50, but NOT 100.555 \n";
    }

    // Ask in a loop until gets a valid card format or null input.
    float askForMoney()
    {
        while (true)
        {
            std::cout << "Please enter the transaction amount (empty for leaving):";
            std::string userInput;
            std::getline(std::cin, userInput);
            if (userInput == "")
            {
                return -1.f;
            }
            else if (moneyValidator->validate(userInput))
            {
                return std::stof(userInput);
            }
            else
            {
                printFormat();
            }
        }
    }
};

// Class to get a valid Merchant from user interface.
class MerchantParser
{
    std::unique_ptr<InputValidator> validator; // validate option is correctly given
    std::vector<std::string> options;

    // Format text for user
    std::string formatOption(int option, const std::string &description)
    {
        std::stringstream msg;
        msg << "[" << option << "]:\t" << std::left << std::setw(20) << description << "\n";
        return msg.str();
    }

public:
    MerchantParser(const std::map<std::string, int> &merchantMap)
    {
        for (auto const &merchant : merchantMap)
        {
            options.push_back(merchant.first);
        }

        validator = std::make_unique<NumericValidator>();
        auto rangeValidator = std::make_unique<RangeValidator>(1, options.size());
        validator->setNext(std::move(rangeValidator));
    };

    void printFormat()
    {
        std::cout << std::string(40, '-') << "\n";
        std::cout << " Select Merchant \n";
        std::cout << std::string(40, '-') << "\n";
        for (size_t i = 0; i < options.size(); i++)
        {
            std::cout << formatOption(i + 1, options[i]);
        }
        std::cout << std::string(40, '-') << "\n";
    }

    // Ask in a loop until gets a valid card format or null input.
    std::optional<std::string> askForMerchant()
    {
        while (true)
        {
            std::cout << "Please select the correct option:";
            std::string userInput;
            std::getline(std::cin, userInput);
            if (userInput == "")
            {
                return {};
            }
            if (validator->validate(userInput))
            {
                return options[std::stoi(userInput) - 1];
            }
        }
    }
};

// Class to get a valid Date from user interface.
class DateParser
{
    std::unique_ptr<DateTimeSQLValidator> DateValidator;

public:
    DateParser()
    {
        DateValidator = std::make_unique<DateTimeSQLValidator>();
    }

    void printDateFormat()
    {
        std::cout << "->Time format is YYYY-MM-DD (fill with 0's if needed)\n";
    }

    std::string askForDates(const std::string &limit, const std::string &ifEmpty)
    {
        while (true)
        {
            std::cout << "Please select a " << limit << " date for the request (empty = ignore):";
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
};