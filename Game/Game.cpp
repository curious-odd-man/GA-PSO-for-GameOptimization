#include "Common.hpp"
#include <iterator>

Game::Game(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize, unsigned char colorsCount)
        : aField(width, height, evaluator), aUtilityEvaluator(evaluator), aScore(0), aFigureSize(figureSize), aColorsCount(
                colorsCount)
{
}

size_t Game::play()
{
    auto addFigure = [&]()
    {
        if (aFiguresForGame.empty())
        aFiguresHistory.emplace_back(aFigureSize, aColorsCount);
        else
        {
            aFiguresHistory.emplace_back(aFiguresForGame.back());
            aFiguresForGame.pop_back();
        }
    };

    aScore = 0;
    aField.reset();
    aNextStates.clear();
    aFiguresHistory.clear();

    // reverse order of figures for game, - taken figure will be removed from vector
    if (aFiguresForGame.size())
        reverse(aFiguresForGame.begin(), aFiguresForGame.end());

#ifdef TEST_PARAMS
    cout << DELIMITER << endl;
    cout << DELIMITER << endl;
    cout << "NEW GAME STARTED" << endl;
#endif

    addFigure();

    while (true)
    {
#ifdef TEST_PARAMS
        cout << DELIMITER << "New turn!" << endl << "current field is " << aField << endl;
#endif
        aField.getNextStates(aFiguresHistory.back(), aNextStates);

        addFigure();
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
    out << *aUtilityEvaluator << endl;
    for (auto& fig : aFiguresHistory)
        out << fig << endl;
    out.close();
}

void Game::setFiguresForGame(istream& in)
{
    while (in)
    {
        unsigned char figureBytes[256] =
            { 0 };
        for (size_t i = 0; i < aFigureSize; ++i)
        {
            in >> figureBytes[i];
            figureBytes[i] -= '0';
        }

        aFiguresForGame.emplace_back(aFigureSize, aColorsCount, figureBytes);
    }

    aFiguresForGame.pop_back();     // last is invalid
}

ostream& Game::operator<<(ostream& os)
{
    os << "Game: Score " << aScore << endl;
    if (aFiguresHistory.size())
        os << aFiguresHistory.back() << endl << aField;
    return os;
}