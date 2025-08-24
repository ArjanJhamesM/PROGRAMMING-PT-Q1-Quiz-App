//
#include <iostream>
#include <string>
#include <vector>

//
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Questionnaire {
    private:
        struct questionTemplate { // encapsulation
            string question = "", choiceA = "", choiceB = "", choiceC = "", choiceD = "";
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
                                   const char userSelectedAnswer,
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
            questionnaireCopy.selectedAnswer = userSelectedAnswer;
            questionnaireCopy.isTimed = userIsTimed;
            questionnaireCopy.durationSeconds = userDurationSeconds;

            createdQuestions.push_back(questionnaireCopy);
        }

        #pragma region VectorDataDebug
            string getVectorQuestion() { // TEMP: getter for debug
                return createdQuestions[0].question;
            }
        #pragma endregion
};

char ConvertLetterStringToChar(string input) {
    if (!input.empty()) {
        char convertedLetter = input.front();
        return convertedLetter;
    }
    else {
        return '\0';
    }
}

class QuizCreation {
    private:
        // TODO: separate into another class
        Questionnaire questionnaire;
        Questionnaire::qTemplate inputQuestionData = questionnaire.getQuestionTemplate();

        int numberOfQuestionsToCreate = 0;
        int numberOfCreatedQuestions = 0;

        string inputData;

        int displayNumberForNewQuestion() {
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

                if (stringInputToInt > 0) {
                    numberOfQuestionsToCreate = stringInputToInt;
                }
                else {
                    cout << "Warning: Number input cannot be zero!" << endl;
                    return false;
                }

                cout << "DEBUG | Retrieved user input for numberOfQuestionsToCreate: " << numberOfQuestionsToCreate << endl; // DEBUG
                return true;
            }
            catch (const std::invalid_argument &e)
            {
                cout << "Warning: Non-numerical input entered!" << endl;
                return false;
            }
            catch (const std::out_of_range &e)
            {
                cout << "Warning: Number input too large!" << endl;
                return false;
            }
        }

        void promptTotalNumberUntilSuccess() {
            while (!askTotalQuestionsCount()) {
                cout << "Warning: Invalid input!" << endl;
            }
        }

        template <typename T>
        bool promptQuestionDetails(const string &inputSelection, const string &promptText, T &questionDataField) { // TODO: track current question number
            cout << promptText << endl;
            getline(cin, inputData);

            if (inputSelection == "Question")
            {
                if constexpr (std::is_same_v<T, string>)
                {
                    if (inputData.empty())
                    {
                        cout << "Warning: No Question specified!" << endl; // TODO: abstract input selection in this message
                        return false;
                    }

                    cout << "DEBUG | Question selected" << endl; // DEBUG
                    questionDataField = inputData;
                    cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
                    return true;
                }
            }

            if (inputSelection == "Correct Choice") {
                if constexpr (std::is_same_v<T, char>)
                {
                    cout << "DEBUG | Correct Choice selected" << endl; // DEBUG
                    char charConversionResult = ConvertLetterStringToChar(inputData);

                    if (charConversionResult == '\0')
                    {
                        cout << "Warning: No Correct Choice specified!" << endl;
                        return false;
                    }

                    questionDataField = charConversionResult;
                    cout << "DEBUG | Retrieved user input: " << questionDataField << endl; // DEBUG
                    return true;
                }
            }
            // TODO: if-check for all int input selections

            return false;
        }

        template <typename T>
        void promptDetailsUntilSuccess(const string inputSelection, const string promptText, T &questionDataField) {
            while (!promptQuestionDetails(inputSelection, promptText, questionDataField)) {
                cout << "Warning: Invalid input!" << endl;
            }
        }

        void buildQuestionUsingDetails() {
            string questionPrompt = "Enter question " + std::to_string(displayNumberForNewQuestion()) + ":";
            promptDetailsUntilSuccess("Question", questionPrompt, inputQuestionData.question);
            promptDetailsUntilSuccess("Correct Choice", "Enter the correct choice: ", inputQuestionData.correctChoice);
            numberOfCreatedQuestions++;
            cout << "DEBUG | Retrieved number of questions created: " << numberOfCreatedQuestions << endl; // DEBUG
            return;
        }

    public:
        void runQuizCreation() {
            promptTotalNumberUntilSuccess();

            while (hasMoreQuestionsToCreate()) {
                buildQuestionUsingDetails();
            }
        }
        // TODO: Questionnaire::qTemplate inputQuestionData{};
};

#pragma region InputImplementationPrototype
    void collectUserInput(const string outputText, string &tempVariable) { // WIP
        cout << outputText << ": " << endl;
        getline(cin, tempVariable); // TODO: validate type
    }
#pragma endregion

int main() {
    QuizCreation qCreation;
    qCreation.runQuizCreation();

    return 0;
}