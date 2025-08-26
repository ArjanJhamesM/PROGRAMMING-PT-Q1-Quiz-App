#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <conio.h>
#include <chrono>
#include <thread>
#include "InputValidators.h"
#include <cctype>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::toupper;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::steady_clock;

int showPositionOfCurrentNumber(const int index) {
    return index + 1;
}

char ConvertLetterStringToChar(string input) {
    if (!input.empty()) {
        char convertedLetter = input.front();
        // cout << "DEBUG | Successfully converted letter string to char" << endl; // DEBUG
        return convertedLetter;
    }
    else {
        // cout << "DEBUG | Cannot convert letter string to char" << endl; // DEBUG
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
        static vector<Questionnaire::qTemplate> createdQuestions;

    public:
        static void addToCreatedQuestions(const string &userQuestion,
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

        static void addUserAnswerToVector(const char userInputLetter, const int currentVectorQuestionIndex) {
            Questionnaire questionnaire;
            Questionnaire::qTemplate questionnaireCopy = questionnaire.getQuestionTemplate();

            questionnaireCopy.selectedAnswer = userInputLetter;
            createdQuestions[currentVectorQuestionIndex].selectedAnswer = questionnaireCopy.selectedAnswer;
        }

        static const vector<Questionnaire::qTemplate>& outputVectorData() {
            return createdQuestions;
        }
};

vector<Questionnaire::qTemplate> CreatedQuestions::createdQuestions;

const auto &getCachedQuestions()
{
    return CreatedQuestions::outputVectorData();
}

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

        const string WARNING_PREFIX = "Warning: ";
        const string EMPTY_QUESTION_WARNING = WARNING_PREFIX + "No Question input entered!";
        const string EMPTY_CHOICE_WARNING = WARNING_PREFIX + "No Choice input entered!";
        const string EMPTY_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "No Correct Answer input entered!";
        const string EMPTY_TIMED_WARNING = WARNING_PREFIX + "Did not specify if timed or not!";
        const string INCORRECT_SECONDS_WARNING = WARNING_PREFIX + "Invalid input for seconds!";
        const string INVALID_BOOL_WARNING = WARNING_PREFIX + "Did not enter \'T\', \'t\', \'F\', or \'f\'!";
        const string INVALID_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "Did not enter \'A\'/\'a\', \'B\'/\'b\', \'C\'/\'c\', or \'D\'/\'d\',!";
        const string INVALID_INPUT_WARNING = WARNING_PREFIX + "Invalid input!";

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

        bool hasMoreQuestionsToCreate() {
            return (numberOfCreatedQuestions < numberOfQuestionsToCreate);
        }

        bool askTotalQuestionsCount()
        {
            cout << "How many questions would you like to create?" << endl;

            string input = "";
            getline(cin, input);
            cout << "\n";

            try
            {
                int stringInputToInt = std::stoi(input);

                if (stringInputToInt <= 0) {
                    return false;
                }

                numberOfQuestionsToCreate = stringInputToInt;
                // cout << "DEBUG | Retrieved Number of questions to create: " << numberOfQuestionsToCreate << endl; // DEBUG
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
                cout << INVALID_INPUT_WARNING << endl;
                cout << "\n";
            }
        }


        template <typename T>
        bool promptQuestionDetails(const string &inputSelection, const string &promptText, T &questionDataField) {
            inputData = "";

            cout << "For question #" << showPositionOfCurrentNumber(numberOfCreatedQuestions) << ": " << promptText << endl;
            getline(cin, inputData);
            cout << "\n";

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
            else if (inputSelection == IS_TIMED_SELECTION) {
                if constexpr (std::is_same_v<T, bool>)
                {
                    return InputValidators::validateBoolDetails(inputData, questionDataField, EMPTY_TIMED_WARNING, INVALID_BOOL_WARNING);
                }
            }
            else if (inputSelection == DURATION_SECONDS_SELECTION) {
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
                cout << INVALID_INPUT_WARNING << endl;
                cout << "\n";
            }
        }

        template <typename T>
        bool promptOptionalChoices(const string &choiceLetter, const string &inputSelection, const string &promptText,
                                   T &questionDataField)
        {
            cout << "Would you like to enter Choice " << choiceLetter << "? [y/n]" << endl;
            string input = "";
            getline(cin, input);
            cout << "\n";

            if (input == "N" || input == "n") {
                input = "";
                return true;
            }
            else if (input == "Y" || input == "y") {
                askDetailsUntilSuccess(inputSelection, promptText, questionDataField);
                input = "";
                return true;
            }

            return false;
        }

        template <typename T>
        void askChoicesUntilSuccess(const string choiceLetter, const string &inputSelection, const string promptText,
                                    T &questionDataField)
        {
            while (!promptOptionalChoices(choiceLetter, inputSelection, promptText, questionDataField)) {
                cout << INVALID_INPUT_WARNING << endl;
                cout << "\n";
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
            askDetailsUntilSuccess(IS_TIMED_SELECTION, "Make the question timed? [t/f]", iQuestionData.isTimed);

            // cout << "DEBUG | Is question timed: " << iQuestionData.isTimed << endl; // DEBUG

            if (iQuestionData.isTimed) {
                askDetailsUntilSuccess(DURATION_SECONDS_SELECTION, "Enter duration in seconds", iQuestionData.durationSeconds);
                return;
            }

            return;
        }

        void makeQuestionUsingDetails() {
            askDetailsUntilSuccess(QUESTION_SELECTION, "Enter question", iQuestionData.question);
            promptChoicesDetails();
            askDetailsUntilSuccess(CORRECT_CHOICE_SELECTION, "Enter the correct choice [a/b (/c/d)]", iQuestionData.correctChoice);
            promptTimeDetails();

            // // DEBUG
            // cout << "DEBUG | InputQuestionData values" << endl;
            // cout << displayCountOfNewQuestion() << ". " << iQuestionData.question << endl;
            // cout << "A. " << iQuestionData.choiceA << endl;
            // cout << "B. " << iQuestionData.choiceB << endl;
            // cout << "C. " << iQuestionData.choiceC << endl;
            // cout << "D. " << iQuestionData.choiceD << endl;
            // cout << "Correct choice: " << iQuestionData.correctChoice << endl;
            // cout << "Is question Timed: " << iQuestionData.isTimed << endl;
            // cout << "Seconds duration: " << iQuestionData.durationSeconds << endl;
            // cout << "END OF DEBUG" << endl;

            CreatedQuestions::addToCreatedQuestions(iQuestionData.question, iQuestionData.choiceA, iQuestionData.choiceB, iQuestionData.choiceC, iQuestionData.choiceD, iQuestionData.correctChoice, iQuestionData.isTimed, iQuestionData.durationSeconds);

            Questionnaire questionnaire;
            iQuestionData = questionnaire.getQuestionTemplate();

            // // DEBUG
            // cout << "DEBUG | CreatedQuestions values" << endl;
            // cout << displayCountOfNewQuestion() << ". " << CreatedQuestions::outputVectorData()[0].question << endl;
            // cout << "A. " << CreatedQuestions::outputVectorData()[0].choiceA << endl;
            // cout << "B. " << CreatedQuestions::outputVectorData()[0].choiceB << endl;
            // cout << "C. " << CreatedQuestions::outputVectorData()[0].choiceC << endl;
            // cout << "D. " << CreatedQuestions::outputVectorData()[0].choiceD << endl;
            // cout << "Correct choice: " << CreatedQuestions::outputVectorData()[0].correctChoice << endl;
            // cout << "Is question Timed: " << CreatedQuestions::outputVectorData()[0].isTimed << endl;
            // cout << "Seconds duration: " << CreatedQuestions::outputVectorData()[0].durationSeconds << endl;
            // cout << "END OF DEBUG" << endl;

            numberOfCreatedQuestions++;
            // cout << "DEBUG | Retrieved number of questions created: " << numberOfCreatedQuestions << endl; // DEBUG

            return;
        }

    public:
        void runQuizCreation() {
            askTotalNumberUntilSuccess();

            while (hasMoreQuestionsToCreate()) {
                makeQuestionUsingDetails();
            }

            return;
        }
};


class QuizTaking {
    private:
        int currentVectorQuestionIndex = 0;

        template <typename T>
        bool hasMoreQuestionsToAnswer(const T &createdQuestionVector) {
            return (currentVectorQuestionIndex < createdQuestionVector.size());
        }

        const string CHOICES_PROMPT = "Enter your answer [a/b/c/d]";

    public:
        template <typename T>
        void displayQuestionAndChoices(const T &createdQuestionVector, const int currentVectorQuestionIndex)
        {
            cout << showPositionOfCurrentNumber(currentVectorQuestionIndex) << ". " << createdQuestionVector[currentVectorQuestionIndex].question << endl;
            cout << "   A. " << createdQuestionVector[currentVectorQuestionIndex].choiceA << endl;
            cout << "   B. " << createdQuestionVector[currentVectorQuestionIndex].choiceB << endl;

            if (createdQuestionVector[currentVectorQuestionIndex].choiceC != "") {
                cout << "   C. " << createdQuestionVector[currentVectorQuestionIndex].choiceC << endl;
            }

            if (createdQuestionVector[currentVectorQuestionIndex].choiceD != "")
            {
                cout << "   D. " << createdQuestionVector[currentVectorQuestionIndex].choiceD << endl;
            }

            if (createdQuestionVector[currentVectorQuestionIndex].isTimed)
            {
                cout << "Duration: " << createdQuestionVector[currentVectorQuestionIndex].durationSeconds << " seconds" << endl;
            }
        }

        template <typename T>
        void retrieveUserAnswerTimed(const T &createdQuestionVector, const int currentVectorQuestionIndex) {
            cout << CHOICES_PROMPT << endl;

            auto startTime = steady_clock::now();

            while (true) {
                if (_kbhit()) {
                    char validatedAnswer = _getch();

                    if (validatedAnswer != 'A' && validatedAnswer != 'a' &&
                        validatedAnswer != 'B' && validatedAnswer != 'b' &&
                        validatedAnswer != 'C' && validatedAnswer != 'c' &&
                        validatedAnswer != 'D' && validatedAnswer != 'd')
                    {
                        cout << "Warning: Invalid letter, answer considered blank!" << endl;
                        break;
                    }

                    if ((validatedAnswer == 'C' || validatedAnswer == 'c') &&
                        (createdQuestionVector[currentVectorQuestionIndex].choiceC == ""))
                    {
                        cout << "Warning: Cannot answer C, answer considered blank!" << endl;
                        break;
                    }

                    if ((validatedAnswer == 'D' || validatedAnswer == 'd') &&
                        (createdQuestionVector[currentVectorQuestionIndex].choiceD == ""))
                    {
                        cout << "Warning: Cannot answer D, answer considered blank!" << endl;
                        break;
                    }

                    CreatedQuestions::addUserAnswerToVector(validatedAnswer, currentVectorQuestionIndex);
                    break;
                }

                auto currentTime = steady_clock::now();

                if (duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= 5) {
                    cout << "Time is up!" << endl;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            cout << "Your answer: " << createdQuestionVector[currentVectorQuestionIndex].selectedAnswer << endl;
            cout << "\n";
        }

        template <typename T>
        void retrieveUserAnswerUntimed(const T &createdQuestionVector, const int currentVectorQuestionIndex) {
            string input = "";
            bool hasNotEnteredValidLetter = true;

            cout << CHOICES_PROMPT << endl;

            while (hasNotEnteredValidLetter) {
                getline(cin, input);
                cout << "\n";

                if ((input == "C" || input == "c") && (createdQuestionVector[currentVectorQuestionIndex].choiceC == "")) {
                    cout << "Warning: Cannot answer C, try again!" << endl;

                    input = "";

                    displayQuestionAndChoices(createdQuestionVector, currentVectorQuestionIndex);
                    cout << CHOICES_PROMPT << endl;

                    continue;
                }

                if ((input == "D" || input == "d") && (createdQuestionVector[currentVectorQuestionIndex].choiceD == "")) {
                    cout << "Warning: Cannot answer D, try again!" << endl;

                    input = "";

                    displayQuestionAndChoices(createdQuestionVector, currentVectorQuestionIndex);
                    cout << CHOICES_PROMPT << endl;

                    continue;
                }

                if (input != "A" && input != "a" &&
                    input != "B" && input != "b" &&
                    input != "C" && input != "c" &&
                    input != "D" && input != "d")
                {
                    cout << "Warning: Invalid letter, try again!" << endl;

                    input = "";

                    displayQuestionAndChoices(createdQuestionVector, currentVectorQuestionIndex);
                    cout << CHOICES_PROMPT << endl;

                    continue;
                }

                CreatedQuestions::addUserAnswerToVector(ConvertLetterStringToChar(input), currentVectorQuestionIndex);

                cout << "Your answer: " << createdQuestionVector[currentVectorQuestionIndex].selectedAnswer << endl;
                cout << "\n";

                hasNotEnteredValidLetter = false;
            }
            return;
        }

        void runQuizTaking()
        {
            while (hasMoreQuestionsToAnswer(getCachedQuestions())) {
                displayQuestionAndChoices(getCachedQuestions(), currentVectorQuestionIndex);

                if (getCachedQuestions()[currentVectorQuestionIndex].isTimed) {
                    retrieveUserAnswerTimed(getCachedQuestions(), currentVectorQuestionIndex);
                    currentVectorQuestionIndex++;
                }
                else {
                    retrieveUserAnswerUntimed(getCachedQuestions(), currentVectorQuestionIndex);
                    currentVectorQuestionIndex++;
                }
            }
        }
};

class Scoring {
    private:
        int score = 0;

    public:
        template <typename T>
        int calculateScore(const T &createdQuestionVector) {
            for (int i = 0; i < createdQuestionVector.size(); i++) {
                char validatedSelectedAnswer = toupper(createdQuestionVector[i].selectedAnswer);
                char validatedCorrectChoice = toupper(createdQuestionVector[i].correctChoice);

                if (validatedSelectedAnswer == validatedCorrectChoice) {
                    score++;
                }
            }

            return score;
        }
};

class QuizResults {
    public:
        string checkIfCorrectAnswer(const char &validatedSelectedAnswer, const char &validatedCorrectChoice) {
            if (validatedSelectedAnswer == validatedCorrectChoice) {
                return "Correctly answered!";
            }
            else {
                return "Answered incorrectly!";
            }
        }

        template <typename T>
        void displayQuizResults(const T &createdQuestionVector)
        {
            Scoring scoring;

            for (const auto &element : createdQuestionVector)
            {
                int currentVectorQuestionIndex = 0;
                char validatedSelectedAnswer = toupper(element.selectedAnswer);
                char validatedCorrectChoice = toupper(element.correctChoice);

                cout << "For question #" << showPositionOfCurrentNumber(currentVectorQuestionIndex) << ": " << element.question << endl;
                cout << "Your answer: " << validatedSelectedAnswer << " | " << "Correct answer: " << validatedCorrectChoice << endl;
                cout << checkIfCorrectAnswer(validatedSelectedAnswer, validatedCorrectChoice) << endl;

                currentVectorQuestionIndex++;

                cout << "\n";
            }

            cout << "Total Score: " << scoring.calculateScore(createdQuestionVector) << "/" << createdQuestionVector.size() << endl;
            cout << "Press any key to exit the quiz app" << endl;
            cout << "\n";
            _getch();    
        }
};

int main() {
    QuizCreation qCreation;
    qCreation.runQuizCreation();

    cout << "Press any key to start taking the quiz" << endl;
    _getch();
    cout << "\n";

    QuizTaking qTaking;
    qTaking.runQuizTaking();

    cout << "Press any key to view your quiz results" << endl;
    _getch();
    cout << "\n";

    QuizResults qResults;
    qResults.displayQuizResults(getCachedQuestions());

    return 0;
}