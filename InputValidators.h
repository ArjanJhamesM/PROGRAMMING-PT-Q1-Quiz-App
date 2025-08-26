#pragma once
#include <iostream>
#include <string>
#include <type_traits>

using std::cin;
using std::cout;
using std::endl;
using std::string;

char ConvertLetterStringToChar(string input); // external functions' declarations
int ConvertNumberStringToInt(string input);
// validators catch errors in user input before storing the input itself to the specified struct field
namespace InputValidators { // to separate from the otherwise lengthy QuizCreation class
    template <typename T> // so the type of the argument is deduced automatically
    bool validateStringDetails(string &inputData, T &questionDataField, const string &emptyInputMessage)
    { // these validators are boolean return functions because they signal whether user input was correctly stored or not
        // using the concept of guard clauses here
        if constexpr (std::is_same_v<T, string>) { // reset struct field before manipulating it
            questionDataField = "";
        }

        if (inputData.empty()) {
            cout << emptyInputMessage << endl;
            return false;
        }

        if constexpr (std::is_same_v<T, string>)
        { // constexpr only runs this statement body if deduced type is same to the required type
            questionDataField = inputData;
            return true;
        }

        cout << "Warning: No if branches matched" << endl;
        return false;
    }

    template <typename T, typename U>
    bool validateCorrectChoice(string &inputData, T &questionDataField, const string &emptyInputMessage,
                               const string &incorrectInputMessage, U &questionDataStruct)
    {
        const string MISSING_CHOICE_WARNING = "Warning: Cannot select an empty choice!"; // self-documenting warning

        if constexpr (std::is_same_v<T, char>) {
            questionDataField = '\0';
        }

        char charConversionResult = ConvertLetterStringToChar(inputData);

        if (charConversionResult == '\0') {
            cout << emptyInputMessage << endl;
            return false;
        }

        if (charConversionResult != 'A' && charConversionResult != 'a' &&
            charConversionResult != 'B' && charConversionResult != 'b' &&
            charConversionResult != 'C' && charConversionResult != 'c' &&
            charConversionResult != 'D' && charConversionResult != 'd')
        { // user mistyped ABCD/abcd
            cout << incorrectInputMessage << endl;
            return false;
        }
        // handle user error for answering non-existent choices C and D
        if (questionDataStruct.choiceC == "" && (charConversionResult == 'C' || charConversionResult == 'c')) {
            cout << MISSING_CHOICE_WARNING << endl;
            return false;
        }
        else if (questionDataStruct.choiceD == "" && (charConversionResult == 'D' || charConversionResult == 'd')) {
            cout << MISSING_CHOICE_WARNING << endl;
            return false;
        }

        if constexpr (std::is_same_v<T, char>) {
            questionDataField = charConversionResult;
            return true;
        }

        cout << "Warning: No if branches matched" << endl;
        return false;
    }

    template <typename T>
    bool validateBoolDetails(string &inputData, T &questionDataField, const string &emptyInputMessage,
                             const string &incorrectInputMessage)
    {
        if constexpr (std::is_same_v<T, bool>) {
            questionDataField = false;
        }

        if (inputData.empty()) {
            cout << emptyInputMessage << endl;
            return false;
        }

        if (inputData != "T" && inputData != "t" &&
            inputData != "F" && inputData != "f")
        { // user mistyped T/F/t/f
            cout << incorrectInputMessage << endl;
            return false;
        }

        if (inputData == "F" || inputData == "f") {
            if constexpr (std::is_same_v<T, bool>)
            { // no changes needed since the boolean field of questionnaire struct is false already
                return true;
            }
        }

        if constexpr (std::is_same_v<T, bool>) {
            questionDataField = true;
            return true;
        }

        cout << "Warning: No if branches matched" << endl;
        return false;
    }

    template <typename T>
    bool validateIntDetails(string &inputData, T &questionDataField, const string &incorrectInputMessage) {
        if constexpr (std::is_same_v<T, int>) {
            questionDataField = 0;
        }

        int intConversionResult = ConvertNumberStringToInt(inputData);

        if (intConversionResult == 0) {
            cout << incorrectInputMessage << endl;
            return false;
        }

        if constexpr (std::is_same_v<T, int>) {
            questionDataField = intConversionResult;
            return true;
        }

        cout << "Warning: No if branches matched" << endl;
        return false;
    }
}