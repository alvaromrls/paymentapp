#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cctype>

#include <algorithm>

// Base Interface por Validators (Chain of Responsability)
class InputValidator
{
protected:
    std::unique_ptr<InputValidator> next;

public:
    InputValidator *setNext(std::unique_ptr<InputValidator> nextValidator)
    {
        next = std::move(nextValidator);
        return next.get();
    }

    virtual bool validate(std::string &userInput) = 0;
};

// Checks if it's a number
class NumericValidator : public InputValidator
{
public:
    bool validate(std::string &userInput) override
    {
        if (userInput.empty() || !std::all_of(userInput.begin(), userInput.end(), ::isdigit))
        {
            std::cout << "Invalid input: " << userInput << " -- Please enter a number.\n";
            return false;
        }

        if (next)
        {
            return next->validate(userInput);
        }

        return true;
    }
};

// Validator that checks if the input exists is in the correct range
class RangeValidator : public InputValidator
{
    size_t minValue, maxValue;

public:
    RangeValidator(size_t min, size_t max) : minValue(min), maxValue(max) {}

    bool validate(std::string &userInput) override
    {
        size_t num = std::stoul(userInput); // Convierte a número sin modificar el tipo de userInput

        if (num < minValue || num > maxValue)
        {
            std::cout << "Invalid option. Please select a number between " << minValue << " and " << maxValue << ".\n";
            return false;
        }

        if (next)
        {
            return next->validate(userInput);
        }

        return true;
    }
};