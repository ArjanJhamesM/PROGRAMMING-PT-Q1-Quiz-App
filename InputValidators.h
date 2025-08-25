#pragma once
#include <iostream>
#include <string>
#include <type_traits>

using std::cin;
using std::cout;
using std::endl;
using std::string;

char ConvertLetterStringToChar(string input);
int ConvertNumberStringToInt(string input);

namespace InputValidators
{
    // TODO: move all validator functions to their own namespace
    template <typename T>
    bool validateStringDetails(string &inputData, T &questionDataField, const string &emptyInputMessage)
    {
        if constexpr (std::is_same_v<T, string>)
        {
            questionDataField = "";
        }

        if (inputData.empty())
        {
            cout << emptyInputMessage << endl;
            return false;
        }

        if constexpr (std::is_same_v<T, string>)
        {
            cout << "DEBUG | String Details (Question, A-D, ...) selected" << endl; // DEBUG
            questionDataField = inputData;
            cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
            return true;
        }

        return false;
    }

    template <typename T, typename U>
    bool validateCorrectChoice(string &inputData, T &questionDataField, const string &emptyInputMessage,
                               const string &incorrectInputMessage, U &questionDataStruct)
    {
        if constexpr (std::is_same_v<T, char>)
        {
            questionDataField = '\0';
        }

        cout << "DEBUG | Char Details (Correct Choice) selected" << endl; // DEBUG
        char charConversionResult = ConvertLetterStringToChar(inputData);

        if (charConversionResult == '\0')
        {
            cout << emptyInputMessage << endl;
            return false;
        }

        if (charConversionResult == 'A' || charConversionResult == 'a' ||
            charConversionResult == 'B' || charConversionResult == 'b' ||
            charConversionResult == 'C' || charConversionResult == 'c' ||
            charConversionResult == 'D' || charConversionResult == 'd')
        {
            if constexpr (std::is_same_v<T, char>)
            {
                if (questionDataStruct.choiceC == "" && (charConversionResult == 'C' || charConversionResult == 'c'))
                {
                    cout << "Warning: Cannot select an empty choice!";
                    return false;
                }
                else if (questionDataStruct.choiceD == "" && (charConversionResult == 'D' || charConversionResult == 'd'))
                {
                    cout << "Warning: Cannot select an empty choice!";
                    return false;
                } // TODO: make conditional expression readable

                questionDataField = charConversionResult;
                cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
                // TODO: reset function for correct choice detail
                return true;
            }
        }

        cout << incorrectInputMessage << endl;
        return false;
    }

    template <typename T>
    bool validateBoolDetails(string &inputData, T &questionDataField, const string &emptyInputMessage,
                             const string &incorrectInputMessage)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            questionDataField = false;
        }

        if (inputData.empty())
        {
            cout << emptyInputMessage << endl;
            return false;
        }

        if (inputData == "T" || inputData == "t")
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                cout << "DEBUG | Bool Details (Is Timed) selected" << endl; // DEBUG
                questionDataField = true;
                cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
                return true;
            }
        }
        else if (inputData == "F" || inputData == "f")
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                cout << "DEBUG | Bool Details (Is Timed) selected" << endl;            // DEBUG
                cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
                return true;
            }
        }

        cout << incorrectInputMessage << endl;
        return false;
    }

    template <typename T>
    bool validateIntDetails(string &inputData, T &questionDataField, const string &incorrectInputMessage)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            questionDataField = 0;
        }

        cout << "DEBUG | Int Details (Seconds Duration) selected" << endl; // DEBUG
        int intConversionResult = ConvertNumberStringToInt(inputData);

        if (intConversionResult == 0)
        {
            cout << incorrectInputMessage << endl;
            return false;
        }

        if constexpr (std::is_same_v<T, int>)
        {
            questionDataField = intConversionResult;
            cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
            return true;
        }

        return false;
    }
}