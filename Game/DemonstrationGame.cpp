#include "Common.hpp"

DemonstrationGame::DemonstrationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize,
                                     unsigned char colorsCount)
        : Game(evaluator, width, height, figureSize, colorsCount), aTurnIdx(0), aStopIdx(0)
{
}

DemonstrationGame::~DemonstrationGame()
{
}

size_t DemonstrationGame::play()
{
    cout << "Demonstration game launched:" << endl;
    cout << *aUtilityEvaluator;
    cout << (aFiguresForGame.empty() ? "Figures will be generated on the fly" : "With " + to_string(aFiguresForGame.size()) + " pre-defined figures") << endl;

    Game::play();
    return aScore;
}

void DemonstrationGame::activityBeforeTurn()
{
    ++aTurnIdx;
    if (aStopIdx != 0 && aTurnIdx < aStopIdx - 1)
        return;     // skip unless reached required turn
    do
    {
        cout << DELIMITER << "Turn " << aTurnIdx << endl << *this << *aUtilityEvaluator;
        cout << "Options: l: lists utilities; s: print states; p: play to..." << endl;
        int ch = cin.get();
        cin.ignore();
        switch (ch)
        {
        case 'l':
            printAllStatesUtilities();
            cin.get();
            break;

        case 's':
            printAllStateOneByOne();
            break;

        case 'p':
            cout << "Enter turn index: ";
            cin >> aStopIdx;
            return;
            break;

        default:
            return;
            break;
        }
    }  while (true);
}

void DemonstrationGame::printAllStatesUtilities()
{
    size_t i = 0;
    cout << "id : utility" << endl;
    for (auto& state : aNextStates)
        cout << to_string(++i) << ": " << state.getUtility() << endl;
    cout << DELIMITER;
}

void DemonstrationGame::printAllStateOneByOne()
{
    size_t i = -1;
    for (auto& state : aNextStates)
    {
        cout << "Printing all states: " << to_string(aNextStates.size() - ++i) << endl;
        cout << state << DELIMITER;
        cin.get();
    }
}
