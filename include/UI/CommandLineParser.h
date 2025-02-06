#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "InputValidator.h"
#include "Commands.h"

// Main class to manage de command line interface
class CommandLineParser
{
    const std::string initialmsg = "SELECT OPERATION: ";                          // Init msg
    std::vector<std::string> options;                                             // For fixed order & index access
    std::unordered_map<std::string, std::unique_ptr<LineParserCommand>> commands; // stores commands
    bool active = false;                                                          // Shows if it's running
    std::unique_ptr<InputValidator> validator;                                    // Validate option is correctly given

    // Format text for user
    std::string formatOption(int option, const std::string &description)
    {
        std::stringstream msg;
        msg << "[" << option << "]:\t" << std::left << std::setw(20) << description << "\n";
        return msg.str();
    }

public:
    // Starts the cli, not possible to add new commands. Ensuring HELP & EXIT are the lasts ones.
    void startRunning()
    {
        // Ensure these are the last
        addCommand("HELP", std::make_unique<HelpCommand>(&commands)); // Help command
        addCommand("EXIT", std::make_unique<ExitCommand>(active));    // Exit command

        validator = std::make_unique<NumericValidator>();
        auto rangeValidator = std::make_unique<RangeValidator>(1, options.size());
        validator->setNext(std::move(rangeValidator));

        active = true;
        printInitialMessage();
    }

    // ASSOCIATES A COMMAND WITH A GIVEN NAME
    void addCommand(std::string name, std::unique_ptr<LineParserCommand> newCommand)
    {
        if (!active)
        {
            options.push_back(name);
            commands[name] = std::move(newCommand);
        }
    }

    // Shows main menu
    void printInitialMessage()
    {
        std::cout << std::string(40, '-') << "\n";
        std::cout << initialmsg << "\n";
        std::cout << std::string(40, '-') << "\n";
        for (size_t i = 0; i < options.size(); i++)
        {
            std::cout << formatOption(i + 1, options[i]);
        }
        std::cout << std::string(40, '-') << "\n";
    }

    // Running getter
    bool running() const
    {
        return active;
    }

    // Ask for a numeric option to the user
    void processUserInput()
    {
        std::cout << "\nChoose an option (1-" << options.size() << "): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (!validator->validate(userInput))
        {
            return;
        }
        std::cout << std::string(40, '-') << "\n";
        int userOption{std::stoi(userInput)};
        std::string selectedOption = options[userOption - 1];
        commands.at(selectedOption)->execute();

        if (active)
        {
            printInitialMessage();
        }
    }
};
