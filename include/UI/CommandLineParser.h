#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "InputValidator.h"

class CommandLineParser
{
    const std::string initialmsg = "SELECT OPERATION: ";
    const std::vector<std::string> options = {"PAY", "HISTORY", "SHOW DUMMY", "EXIT", "HELP"};
    std::vector<std::string> number_options;
    const std::unordered_map<std::string, std::string> help = {
        {"PAY", "Request a new payment. It will ask for the Amount, the Card Number, and the Commerce Type."},
        {"HISTORY", "Given a Credit Card, it will show the transaction history. Optional: date range."},
        {"SHOW DUMMY", "[For this POC version] Will show all already added Card numbers."},
        {"EXIT", "Will finish the program."},
        {"HELP", "Show available commands and their descriptions."}};

    bool active = true; // Control de ejecución

    std::string formatOption(int option, const std::string &description)
    {
        std::stringstream msg;
        msg << "[" << option << "]: " << std::left << std::setw(20) << description << "\n";
        return msg.str();
    }

    std::unique_ptr<InputValidator> validator;

public:
    CommandLineParser()
    {
        validator = std::make_unique<NumericValidator>();
        auto rangeValidator = std::make_unique<RangeValidator>(1, 5);
        validator->setNext(std::move(rangeValidator));
    }

    void printInitialMessage()
    {
        std::cout << std::string(40, '-') << "\n";
        std::cout << initialmsg << "\n";
        std::cout << std::string(40, '-') << "\n";
        for (size_t i = 0; i < options.size(); i++)
        {
            std::cout << formatOption(i + 1, options[i]);
        }
    }

    bool running() const
    {
        return active;
    }

    void processUserInput()
    {
        std::cout << "Choose an option (1-" << options.size() << "): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (!validator->validate(userInput))
        {
            return;
        }

        int userOption{std::stoi(userInput)};

        std::string selectedOption = options[userOption - 1];

        if (selectedOption == "EXIT")
        {
            std::cout << "Exiting...\n";
            active = false;
        }
        else if (help.find(selectedOption) != help.end())
        {
            std::cout << help.at(selectedOption) << "\n";
        }
        else
        {
            std::cout << "No information available for this option.\n";
        }
    }
};
