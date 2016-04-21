#include "Common.hpp"

Game::Game(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount)
        : aField(width, height, evaluator), aUtilityEvaluator(evaluator), aScore(0), aFigureSize(
                figureSize), aColorsCount(colorsCount)
{
}

size_t Game::play()
{
    aScore = 0;
    aField.reset();
    aNextStates.clear();
    aFiguresHistory.clear();

#ifdef TEST_PARAMS
    cout << DELIMITER << endl;
    cout << DELIMITER << endl;
    cout << "NEW GAME STARTED" << endl;
#endif

    while (true)
    {
        aFiguresHistory.emplace_back(aFigureSize, aColorsCount);
#ifdef TEST_PARAMS
        cout << DELIMITER << "New turn!" << endl << "current field is " << aField << endl;
#endif
        aField.getNextStates(aFiguresHistory.back(), aNextStates);

        activityBeforeTurn();
        if (gameOver())
            break;

        aField = *max_element(aNextStates.begin(), aNextStates.end());
        aScore += aField.getRemoved();
    }

    return aScore;

}

void Game::dumpFiguresHistory(string name)
{
    ofstream out;
    out.open(name);
    for (auto& fig : aFiguresHistory)
        out << fig << endl;
    out.close();
}

