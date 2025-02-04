#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>

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

// HISTORY command
const std::string HISTORY_COMMAND_HELP = "Given a Credit Card, it will show the transaction history. Optional: date range.";
class HistoryCommand : public LineParserCommand
{
public:
    HistoryCommand() : LineParserCommand(HISTORY_COMMAND_HELP) {};
    void execute() override
    {
        std::cout << "Showing transaction history...\n";
    }
};

// SHOW DUMMY COMMAND
const std::string SHOW_DUMMY_COMMAND_HELP = "It Will show all already added Card numbers. [For this POC version]. ";
class ShowDummyCommand : public LineParserCommand
{
public:
    ShowDummyCommand() : LineParserCommand(SHOW_DUMMY_COMMAND_HELP) {};
    void execute() override
    {
        std::cout << "Showing Dummy Cards Number...\n";
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