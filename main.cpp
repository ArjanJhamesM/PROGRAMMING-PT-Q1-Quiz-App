#include <iostream> // import libraries
#include <string>
#include <vector>
#include <type_traits>
#include <conio.h>
#include <chrono>
#include <thread>
#include "InputValidators.h"
#include <cctype>

using std::cin; // namespace aliases
using std::cout;
using std::endl;
using std::string;
using std::toupper;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::steady_clock;

int showPositionOfCurrentNumber(const int index) { // follow off-by-one rule between index and actual count
    return index + 1;
}

char ConvertLetterStringToChar(string input) { 
    if (!input.empty()) { // because user input is retrieved as string and some functions need char as user input
        char convertedLetter = input.front(); // only want to get the first char in case >1 char was entered
        return convertedLetter;
    }
    else {
        return '\0';
    }
}

int ConvertNumberStringToInt(string input) {
    if (!input.empty()) {
        try {
            int stringInputToInt = std::stoi(input); // convert string user input to a proper int

            if (stringInputToInt <= 0) // because 0 and negative numbers cannot be accepted
            {
                return 0;
            }

            return stringInputToInt;
        }
        catch (const std::invalid_argument &e) // 0 signifies input is invalid
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
    private: // encapsulation
        struct questionTemplate { // will be used as basis for what quiz info to show
            string question = "", choiceA = "", choiceB = "", choiceC = "", choiceD = "";
            char correctChoice = '\0', selectedAnswer = '\0';
            bool isTimed = false;
            int durationSeconds = 0;
        };

    public:
        using qTemplate = questionTemplate;

        questionTemplate getQuestionTemplate() { // apply the struct fields to objects that want to do so
            return questionTemplate{};
        }
};

class CreatedQuestions {
    private:
        static vector<Questionnaire::qTemplate> createdQuestions; 
        // has individual indices, good for storing struct fields in each of them
    public: 
        static void addToCreatedQuestions(const string &userQuestion,
                                   const string &userChoiceA,
                                   const string &userChoiceB,
                                   const string &userChoiceC,
                                   const string &userChoiceD,
                                   const char userCorrectChoice,
                                   const bool userIsTimed,
                                   const int userDurationSeconds)
        { // only 1 out of 2 methods that can modify the vector itself
            Questionnaire questionnaire;
            Questionnaire::qTemplate questionnaireCopy = questionnaire.getQuestionTemplate();
            // store user input first into temporary struct, so its fields can be stored to the vector's individual
            // indices after
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
            // provisioned with ability to modify a certain index for iterating for loops
            questionnaireCopy.selectedAnswer = userInputLetter;
            createdQuestions[currentVectorQuestionIndex].selectedAnswer = questionnaireCopy.selectedAnswer;
        }

        static const vector<Questionnaire::qTemplate>& outputVectorData() { // const to make it read-only copy
            return createdQuestions;
        }
};

vector<Questionnaire::qTemplate> CreatedQuestions::createdQuestions; // to be able to use the class' static methods

const auto &getCachedQuestions() { // to avoid unnecessary copies
    return CreatedQuestions::outputVectorData();
}

class InputQuestionData { // so we have a temporary struct to store user input while in quiz creation
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
        int numberOfQuestionsToCreate = 0; // these two track the progress of user while creating questions
        int numberOfCreatedQuestions = 0;

        string inputData = ""; // where the current user input is stored temporarily

        const string WARNING_PREFIX = "Warning: "; // for later code to be self-documenting using shorter names
        const string EMPTY_QUESTION_WARNING = WARNING_PREFIX + "No Question input entered!";
        const string EMPTY_CHOICE_WARNING = WARNING_PREFIX + "No Choice input entered!";
        const string EMPTY_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "No Correct Answer input entered!";
        const string EMPTY_TIMED_WARNING = WARNING_PREFIX + "Did not specify if timed or not!";
        const string INCORRECT_SECONDS_WARNING = WARNING_PREFIX + "Invalid input for seconds!";
        const string INVALID_BOOL_WARNING = WARNING_PREFIX + "Did not enter \'T\', \'t\', \'F\', or \'f\'!";
        const string INVALID_CORRECT_CHOICE_WARNING = WARNING_PREFIX + "Did not enter \'A\'/\'a\', \'B\'/\'b\', \'C\'/\'c\', or \'D\'/\'d\',!";
        const string INVALID_INPUT_WARNING = WARNING_PREFIX + "Invalid input!";

        const string QUESTION_SELECTION = "Question"; // all of these used for validating appropriate user input
        const string CHOICE_A_SELECTION = "Choice A";
        const string CHOICE_B_SELECTION = "Choice B";
        const string CHOICE_C_SELECTION = "Choice C";
        const string CHOICE_D_SELECTION = "Choice D";
        const string CORRECT_CHOICE_SELECTION = "Correct Choice";
        const string IS_TIMED_SELECTION = "Is Timed";
        const string DURATION_SECONDS_SELECTION = "Seconds Duration";

        InputQuestionData inputQuestionData;
        Questionnaire::qTemplate& iQuestionData = inputQuestionData.getInputQuestionData();
        // temporary struct to store needed data for a complete question
        bool hasMoreQuestionsToCreate() { // has to be boolean return function so while loop evaluates it per iteration
            return (numberOfCreatedQuestions < numberOfQuestionsToCreate);
        }

        bool askTotalQuestionsCount()
        {
            cout << "How many questions would you like to create?" << endl;

            string input = "";
            getline(cin, input);
            cout << "\n";

            try {
                int stringInputToInt = std::stoi(input);

                if (stringInputToInt <= 0) {
                    return false;
                }

                numberOfQuestionsToCreate = stringInputToInt; // string to int first so progress tracking works
                return true;
            }
            catch (const std::invalid_argument &e) // for letters/special characters input
            {
                cout << WARNING_PREFIX << "Non-numerical input entered!" << endl;
                return false;
            }
            catch (const std::out_of_range &e) // for numbers larger than int can store
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
        // until success functions are wrapped around while loop so it can evaluate the boolean return functions
        // on whether or not user input was correctly retrieved or not
        template <typename T> // template to deduce the type of whatever argument is passed
        bool promptQuestionDetails(const string &inputSelection, const string &promptText, T &questionDataField) {
            inputData = "";

            cout << "For question #" << showPositionOfCurrentNumber(numberOfCreatedQuestions) << ": " << promptText << endl;
            getline(cin, inputData);
            cout << "\n";
            // calls appropriate input validator based on what question detail from user is being stored
            // InputValidators namespace in separate header file
            // 3 arguments: temporary user input variable, specific struct field, and self-documenting warning message
            if (inputSelection == QUESTION_SELECTION) {
                if constexpr (std::is_same_v<T, string>) {
                    return InputValidators::validateStringDetails(inputData, questionDataField, EMPTY_QUESTION_WARNING);
                } // constexpr avoids compile-time errors from unwanted mismatch of types for particular if branch
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

            if (input == "N" || input == "n") { // keeps Choice C struct field empty for checking later
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

        void promptChoicesDetails() { // combined into one function to house optional choices C and D functionality
            askDetailsUntilSuccess(CHOICE_A_SELECTION, "Enter choice A", iQuestionData.choiceA);
            askDetailsUntilSuccess(CHOICE_B_SELECTION, "Enter choice B", iQuestionData.choiceB);

            askChoicesUntilSuccess("C", CHOICE_C_SELECTION, "Enter choice C", iQuestionData.choiceC);
            if (iQuestionData.choiceC != "") {
                askChoicesUntilSuccess("D", CHOICE_D_SELECTION, "Enter choice D", iQuestionData.choiceD);
                return;
            }

            return;
        }

        void promptTimeDetails() { // combined into one function to house optional duration prompt functionality
            askDetailsUntilSuccess(IS_TIMED_SELECTION, "Make the question timed? [t/f]", iQuestionData.isTimed);

            if (iQuestionData.isTimed) {
                askDetailsUntilSuccess(DURATION_SECONDS_SELECTION, "Enter duration in seconds", iQuestionData.durationSeconds);
                return;
            }

            return;
        }
        // combined into one function so the while loop can easily call all of these functions per iteration
        void makeQuestionUsingDetails() { 
            askDetailsUntilSuccess(QUESTION_SELECTION, "Enter question", iQuestionData.question);
            promptChoicesDetails();
            askDetailsUntilSuccess(CORRECT_CHOICE_SELECTION, "Enter the correct choice [a/b (/c/d)]", iQuestionData.correctChoice);
            promptTimeDetails();

            CreatedQuestions::addToCreatedQuestions(iQuestionData.question, iQuestionData.choiceA, iQuestionData.choiceB, iQuestionData.choiceC, iQuestionData.choiceD, iQuestionData.correctChoice, iQuestionData.isTimed, iQuestionData.durationSeconds);

            Questionnaire questionnaire;
            iQuestionData = questionnaire.getQuestionTemplate(); // reset for next batch of question details

            numberOfCreatedQuestions++; // signal that one question has been successfully created

            return;
        }

    public:
        void runQuizCreation() { // one call to execute all quiz creation implementation
            askTotalNumberUntilSuccess();

            while (hasMoreQuestionsToCreate()) { // while loop evaluates boolean return function that tracks progress
                makeQuestionUsingDetails();
            }

            return;
        }
};


class QuizTaking {
    private:
        int currentVectorQuestionIndex = 0; // so the corresponding struct fields are accessed

        template <typename T>
        bool hasMoreQuestionsToAnswer(const T &createdQuestionVector) {
            return (currentVectorQuestionIndex < static_cast<int>(createdQuestionVector.size())); // follows off-by-one rule
        } // .size() tells how many questions with each of their own struct fields has been created

        const string CHOICES_PROMPT = "Enter your answer [a/b/c/d]";

    public:
        template <typename T> // arguments are iterable by the for loop later, so respective struct fields are shown
        void displayQuestionAndChoices(const T &createdQuestionVector, const int currentVectorQuestionIndex) {
            cout << showPositionOfCurrentNumber(currentVectorQuestionIndex) << ". " << createdQuestionVector[currentVectorQuestionIndex].question << endl;
            cout << "   A. " << createdQuestionVector[currentVectorQuestionIndex].choiceA << endl;
            cout << "   B. " << createdQuestionVector[currentVectorQuestionIndex].choiceB << endl;
            // show only the optional choices C and D if they're successfully filled up and therefore not skipped
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
                    // using the concept of guard clauses here
                    if (validatedAnswer != 'A' && validatedAnswer != 'a' &&
                        validatedAnswer != 'B' && validatedAnswer != 'b' &&
                        validatedAnswer != 'C' && validatedAnswer != 'c' &&
                        validatedAnswer != 'D' && validatedAnswer != 'd')
                    {
                        cout << "Warning: Invalid letter, answer considered blank!" << endl;
                        break; // cannot retry to keep answering timed questions fair
                    }
                    // handle user error for answering non-existent choices C and D
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
                // countdown according to specified seconds alloted for question
                if (duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= createdQuestionVector[currentVectorQuestionIndex].durationSeconds) {
                    cout << "Time is up!" << endl;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // prevents CPU spin (wasteful calculations)
            }

            cout << "Your answer: " << createdQuestionVector[currentVectorQuestionIndex].selectedAnswer << endl;
            cout << "\n";
        }

        template <typename T>
        void retrieveUserAnswerUntimed(const T &createdQuestionVector, const int currentVectorQuestionIndex) {
            string input = "";
            bool hasNotEnteredValidLetter = true; // so user can proceed to next question only if their input is correct

            cout << CHOICES_PROMPT << endl;

            while (hasNotEnteredValidLetter) {
                getline(cin, input);
                cout << "\n";
                // user can retry if they entered a misinput since it would be still fair
                // handle user error for answering non-existent choices C and D
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

                hasNotEnteredValidLetter = false; // proceed to next question since user input is correctly retrieved
            }
            return;
        }

        void runQuizTaking() { // one combined function, one easy call
            while (hasMoreQuestionsToAnswer(getCachedQuestions())) {
                displayQuestionAndChoices(getCachedQuestions(), currentVectorQuestionIndex);
                // retrieve user input in a way appropriate to timed/untimed nature of the question
                if (getCachedQuestions()[currentVectorQuestionIndex].isTimed) {
                    retrieveUserAnswerTimed(getCachedQuestions(), currentVectorQuestionIndex);
                    currentVectorQuestionIndex++; // question has been answered
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
        int score = 0; // private so external code cannot tamper with the result

    public:
        template <typename T>
        int calculateScore(const T &createdQuestionVector) { // for loops allow checking all vector's stored questions
            for (int i = 0; i < static_cast<int>(createdQuestionVector.size()); i++) {
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
        void displayQuizResults(const T &createdQuestionVector) {
            Scoring scoring;

            for (int i = 0; i < static_cast<int>(createdQuestionVector.size()); i++) { // obvious ba???
                char validatedSelectedAnswer = toupper(createdQuestionVector[i].selectedAnswer);
                char validatedCorrectChoice = toupper(createdQuestionVector[i].correctChoice);

                cout << "For question #" << showPositionOfCurrentNumber(i) << ": " << createdQuestionVector[i].question << endl;
                cout << "Your answer: " << validatedSelectedAnswer << " | Correct answer: " << validatedCorrectChoice << endl;
                cout << checkIfCorrectAnswer(validatedSelectedAnswer, validatedCorrectChoice) << endl;

                cout << "\n";
            }

            cout << "Total Score: " << scoring.calculateScore(createdQuestionVector) << "/" << createdQuestionVector.size() << endl;
            cout << "Press any key to exit the quiz app" << endl;
            _getch();
        }
};

int main() { // run each phase of the quiz application
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