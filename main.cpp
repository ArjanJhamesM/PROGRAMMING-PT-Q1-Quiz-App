//
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include "InputValidators.h"

//
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

char ConvertLetterStringToChar(string input) {
    if (!input.empty()) {
        char convertedLetter = input.front();
        cout << "DEBUG | Successfully converted letter string to char" << endl; // DEBUG
        return convertedLetter;
    }
    else {
        cout << "DEBUG | Cannot convert letter string to char" << endl; // DEBUG
        return '\0';
    }
}

int ConvertNumberStringToInt(string input) {
    if (!input.empty()) {
        try {
            int stringInputToInt = std::stoi(input);

            if (stringInputToInt <= 0)
            {
                return 0;
            }

            return stringInputToInt;
        }
        catch (const std::invalid_argument &e)
        {
            return 0;
        }
        catch (const std::out_of_range &e)
        {
            return 0;
        }
    }
    else {
        return 0;
    }
}

class Questionnaire {
    private:
        struct questionTemplate { // encapsulation
            string question = "", choiceA = "", choiceB = "", choiceC = "", choiceD = "";
            // TODO: hide C and D formatted text if user didn't want to create C and D
            char correctChoice = '\0', selectedAnswer = '\0';
            bool isTimed = false;
            int durationSeconds = 0;
        };

    public:
        using qTemplate = questionTemplate;

        questionTemplate getQuestionTemplate() {
            questionTemplate qTemplate;
            return qTemplate;
        }
};

class CreatedQuestions {
    private:
        vector<Questionnaire::qTemplate> createdQuestions;

    public:
        void addToCreatedQuestions(const string &userQuestion,
                                   const string &userChoiceA,
                                   const string &userChoiceB,
                                   const string &userChoiceC,
                                   const string &userChoiceD,
                                   const char userCorrectChoice,
                                   const bool userIsTimed,
                                   const int userDurationSeconds)
        {
            Questionnaire questionnaire;
            Questionnaire::qTemplate questionnaireCopy = questionnaire.getQuestionTemplate();

            questionnaireCopy.question = userQuestion;
            questionnaireCopy.choiceA = userChoiceA;
            questionnaireCopy.choiceB = userChoiceB;
            questionnaireCopy.choiceC = userChoiceC;
            questionnaireCopy.choiceD = userChoiceD;
            questionnaireCopy.correctChoice = userCorrectChoice;
            questionnaireCopy.isTimed = userIsTimed;
            questionnaireCopy.durationSeconds = userDurationSeconds;

            createdQuestions.push_back(questionnaireCopy);
        }

        const vector<Questionnaire::qTemplate>& outputVectorData() {
            return createdQuestions;
        }
};

class InputQuestionData {
    private:
        Questionnaire questionnaire;
        Questionnaire::qTemplate inputQuestionData = questionnaire.getQuestionTemplate();

    public:
        Questionnaire::qTemplate& getInputQuestionData() {
            return inputQuestionData;
        }
};

class QuizCreation {
    private:
        int numberOfQuestionsToCreate = 0;
        int numberOfCreatedQuestions = 0;

        string inputData = ""; // TODO: make a public setter method for validator class to access

        const char EMPTY_CHAR = '\0';

        const string WARNING_PREFIX = "Warning: ";
        const string EMPTY_QUESTION_WARNING = WARNING_PREFIX + "No Question input entered!";
        const string EMPTY_CHOICE_WARNING = WARNING_PREFIX + "No Choice input entered!";
        const string EMPTY_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "No Correct Answer input entered!";
        const string EMPTY_TIMED_WARNING = WARNING_PREFIX + "Did not specify if timed or not!";
        const string INCORRECT_SECONDS_WARNING = WARNING_PREFIX + "Invalid input for seconds!";
        const string INVALID_BOOL_WARNING = WARNING_PREFIX + "Did not enter \'T\', \'t\', \'F\', or \'f\'!";
        const string INVALID_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "Did not enter \'A\'/\'a\', \'B\'/\'b\', \'C\'/\'c\', or \'D\'/\'d\',!";

        const string QUESTION_SELECTION = "Question";
        const string CHOICE_A_SELECTION = "Choice A";
        const string CHOICE_B_SELECTION = "Choice B";
        const string CHOICE_C_SELECTION = "Choice C";
        const string CHOICE_D_SELECTION = "Choice D";
        const string CORRECT_CHOICE_SELECTION = "Correct Choice";
        const string IS_TIMED_SELECTION = "Is Timed";
        const string DURATION_SECONDS_SELECTION = "Seconds Duration";

        InputQuestionData inputQuestionData;
        Questionnaire::qTemplate& iQuestionData = inputQuestionData.getInputQuestionData();

        int displayCountOfNewQuestion()
        {
            return numberOfCreatedQuestions + 1;
        }

        bool hasMoreQuestionsToCreate() {
            return (numberOfCreatedQuestions < numberOfQuestionsToCreate);
        }

        bool askTotalQuestionsCount()
        {
            cout << "How many questions would you like to create?" << endl;

            string input = "";
            getline(cin, input);

            try
            {
                int stringInputToInt = std::stoi(input);

                if (stringInputToInt <= 0) {
                    cout << WARNING_PREFIX << "Invalid number input!" << endl;
                    return false;
                }

                numberOfQuestionsToCreate = stringInputToInt;
                cout << "DEBUG | Retrieved Number of questions to create: " << numberOfQuestionsToCreate << endl; // DEBUG
                return true;
            }
            catch (const std::invalid_argument &e)
            {
                cout << WARNING_PREFIX << "Non-numerical input entered!" << endl;
                return false;
            }
            catch (const std::out_of_range &e)
            {
                cout << WARNING_PREFIX << "Number input too large!" << endl;
                return false;
            }
        }

        void askTotalNumberUntilSuccess() {
            while (!askTotalQuestionsCount()) {
                cout << WARNING_PREFIX << "Invalid input!" << endl;
            }
        }


        template <typename T>
        bool promptQuestionDetails(const string &inputSelection, const string &promptText, T &questionDataField) {
            inputData = "";

            cout << "For question #" << displayCountOfNewQuestion() << ": " << promptText << endl;
            getline(cin, inputData);

            if (inputSelection == QUESTION_SELECTION) {
                if constexpr (std::is_same_v<T, string>) {
                    return InputValidators::validateStringDetails(inputData, questionDataField, EMPTY_QUESTION_WARNING);
                }
            }
            else if (inputSelection == CHOICE_A_SELECTION ||
                     inputSelection == CHOICE_B_SELECTION ||
                     inputSelection == CHOICE_C_SELECTION ||
                     inputSelection == CHOICE_D_SELECTION)
            {
                if constexpr (std::is_same_v<T, string>) {
                    return InputValidators::validateStringDetails(inputData, questionDataField, EMPTY_CHOICE_WARNING);
                }
            }
            else if (inputSelection == CORRECT_CHOICE_SELECTION) {
                if constexpr (std::is_same_v<T, char>) {
                    return InputValidators::validateCorrectChoice(inputData, questionDataField,
                                                                  EMPTY_CORRECT_CHOICE_WARNING,
                                                                  INVALID_CORRECT_CHOICE_WARNING,
                                                                  iQuestionData);
                }
            }
            else if (inputSelection == IS_TIMED_SELECTION)
            {
                if constexpr (std::is_same_v<T, bool>)
                {
                    return InputValidators::validateBoolDetails(inputData, questionDataField, EMPTY_TIMED_WARNING, INVALID_BOOL_WARNING);
                }
            }
            else if (inputSelection == DURATION_SECONDS_SELECTION)
            {
                if constexpr (std::is_same_v<T, int>)
                {
                    return InputValidators::validateIntDetails(inputData, questionDataField, INCORRECT_SECONDS_WARNING);
                }
            }

            return false;
        }

        template <typename T>
        void askDetailsUntilSuccess(const string &inputSelection, const string promptText, T &questionDataField) {
            while (!promptQuestionDetails(inputSelection, promptText, questionDataField)) {
                cout << "Warning: Invalid input!" << endl;
            }
        }

        template <typename T>
        bool promptOptionalChoices(const string &choiceLetter, const string &inputSelection, const string &promptText,
                                   T &questionDataField)
        {
            cout << "Would you like to enter Choice " << choiceLetter << "? [y/n]" << endl;
            string input = "";
            getline(cin, input);

            if (input == "N" || input == "n") {
                return true;
            }
            else if (input == "Y" || input == "y") {
                askDetailsUntilSuccess(inputSelection, promptText, questionDataField);
                return true;
            }

            return false;
        }

        template <typename T>
        void askChoicesUntilSuccess(const string choiceLetter, const string &inputSelection, const string promptText,
                                    T &questionDataField)
        {
            while (!promptOptionalChoices(choiceLetter, inputSelection, promptText, questionDataField)) {
                cout << "Warning: Invalid input!" << endl;
            }
        }

        void promptChoicesDetails() {
            askDetailsUntilSuccess(CHOICE_A_SELECTION, "Enter choice A", iQuestionData.choiceA);
            askDetailsUntilSuccess(CHOICE_B_SELECTION, "Enter choice B", iQuestionData.choiceB);

            askChoicesUntilSuccess("C", CHOICE_C_SELECTION, "Enter choice C", iQuestionData.choiceC);

            if (iQuestionData.choiceC != "") {
                askChoicesUntilSuccess("D", CHOICE_D_SELECTION, "Enter choice D", iQuestionData.choiceD);
                return;
            }

            return;
        }

        void promptTimeDetails() {
            askDetailsUntilSuccess(IS_TIMED_SELECTION, "Make the question timed?", iQuestionData.isTimed);

            cout << "DEBUG | Is question timed: " << iQuestionData.isTimed << endl; // DEBUG

            if (iQuestionData.isTimed) {
                askDetailsUntilSuccess(DURATION_SECONDS_SELECTION, "Enter duration in seconds", iQuestionData.durationSeconds);
                return;
            }

            return;
        }

        void makeQuestionUsingDetails() { // WIP
            askDetailsUntilSuccess(QUESTION_SELECTION, "Enter question", iQuestionData.question);
            promptChoicesDetails();
            askDetailsUntilSuccess(CORRECT_CHOICE_SELECTION, "Enter the correct choice", iQuestionData.correctChoice);
            promptTimeDetails();

            // DEBUG
            cout << "DEBUG | InputQuestionData values" << endl;
            cout << displayCountOfNewQuestion() << ". " << iQuestionData.question << endl;
            cout << "A. " << iQuestionData.choiceA << endl;
            cout << "B. " << iQuestionData.choiceB << endl;
            cout << "C. " << iQuestionData.choiceC << endl;
            cout << "D. " << iQuestionData.choiceD << endl;
            cout << "Correct choice: " << iQuestionData.correctChoice << endl;
            cout << "Is question Timed: " << iQuestionData.isTimed << endl;
            cout << "Seconds duration: " << iQuestionData.durationSeconds << endl;
            cout << "END OF DEBUG" << endl;

            CreatedQuestions cQuestions;
            cQuestions.addToCreatedQuestions(iQuestionData.question, iQuestionData.choiceA, iQuestionData.choiceB, iQuestionData.choiceC, iQuestionData.choiceD, iQuestionData.correctChoice, iQuestionData.isTimed, iQuestionData.durationSeconds);

            // DEBUG
            cout << "DEBUG | CreatedQuestions values" << endl;
            cout << displayCountOfNewQuestion() << ". " << cQuestions.outputVectorData()[0].question << endl;
            cout << "A. " << cQuestions.outputVectorData()[0].choiceA << endl;
            cout << "B. " << cQuestions.outputVectorData()[0].choiceB << endl;
            cout << "C. " << cQuestions.outputVectorData()[0].choiceC << endl;
            cout << "D. " << cQuestions.outputVectorData()[0].choiceD << endl;
            cout << "Correct choice: " << cQuestions.outputVectorData()[0].correctChoice << endl;
            cout << "Is question Timed: " << cQuestions.outputVectorData()[0].isTimed << endl;
            cout << "Seconds duration: " << cQuestions.outputVectorData()[0].durationSeconds << endl;
            cout << "END OF DEBUG" << endl;

            Questionnaire::qTemplate iQuestionData = inputQuestionData.getInputQuestionData();

            numberOfCreatedQuestions++;
            cout << "DEBUG | Retrieved number of questions created: " << numberOfCreatedQuestions << endl; // DEBUG

            return;
        }

    public:
        void runQuizCreation() {
            askTotalNumberUntilSuccess();

            while (hasMoreQuestionsToCreate()) {
                makeQuestionUsingDetails();
            }
        }
};

int main() {
    QuizCreation qCreation;
    qCreation.runQuizCreation();

    return 0;
}