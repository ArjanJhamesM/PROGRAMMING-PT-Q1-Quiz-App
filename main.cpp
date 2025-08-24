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

class CreatedQuestions {
    private:
        struct questionTemplate
        {
            string question, choiceA, choiceB, choiceC, choiceD;
            char correctChoice, userChoice;
            bool isTimed;
        };

        vector<questionTemplate> createdQuestions;

    public:
        void addToCreatedQuestions(const string &userQuestion,
                                   const string &userChoiceA,
                                   const string &userChoiceB,
                                   const string &userChoiceC,
                                   const string &userChoiceD,
                                   const char userCorrectChoice,
                                   const bool userIsTimed)
        {
            questionTemplate qTemplate;

            qTemplate.question = userQuestion;
            qTemplate.choiceA = userChoiceA;
            qTemplate.choiceB = userChoiceB;
            qTemplate.choiceC = userChoiceC;
            qTemplate.choiceD = userChoiceD;
            qTemplate.correctChoice = userCorrectChoice;
            qTemplate.isTimed = userIsTimed;

            createdQuestions.push_back(qTemplate);
        }

        string getVectorQuestion() { // TEMP: getter for debug
            questionTemplate qTemplate;
            return createdQuestions[0].question;
        }
};

void collectUserInput(const string outputText, string &tempVariable) { // WIP
    cout << outputText << ": " << endl;
    getline(cin, tempVariable); // TODO: validate type
}

int main()
{
    // TODO: move to new class for quiz creation
    string question = "", choiceA = "", choiceB = "", choiceC = "", choiceD = "";
    char correctChoice = '\0', userChoice = '\0';
    bool isTimed = false;

    collectUserInput("Enter your question", question); // WIP: implementation

    cout << "Local Question variable data: " << question << endl;

    // TODO: make repeatable for new questions to be created
    CreatedQuestions cQuestions;

    cQuestions.addToCreatedQuestions(question, choiceA, choiceB, choiceC, choiceD, correctChoice, isTimed);
    // TODO: wipe user input variables after for new questions

    // TEMP: debug output of vector data
    cout << "Vector Question data: " << cQuestions.getVectorQuestion() << endl;
    cin;

    return 0;
}