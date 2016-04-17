#include "Common.hpp"

const size_t OptimizationGame::DEFAULT_GAMES_COUNT = 3;

OptimizationGame::OptimizationGame(UtilityEvaluator* evaluator, size_t width, size_t height, size_t figureSize,
                                   unsigned char colorsCount)
        : Game(evaluator, width, height, figureSize, colorsCount)
{
}

size_t OptimizationGame::play(size_t gamesCount)
{
    vector < size_t > game_scores;

    for (size_t i = 0; i < gamesCount; ++i)
        game_scores.emplace_back(Game::play());

    aScore = accumulate(game_scores.begin(), game_scores.end(), 0ULL, plus<size_t>()) / game_scores.size();
    aUtilityEvaluator->setUtility(aScore);
    return aScore;
}
