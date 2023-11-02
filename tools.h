#pragma once

#include <random>

#include "const.h"

int getRandomRangeNumber(const int nLowestNumber, const int nHighestNumber)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(nLowestNumber, nHighestNumber);
    return dist(gen);
}

const double getSymbolMultiplier(Symbol_t eSymbol, int nCounter)
{
    auto itMultipliers = SYMBOL_MULTIPLIERS.find(nCounter);
    if (itMultipliers == SYMBOL_MULTIPLIERS.end())
    {
        // Not enough symbols in a row to get a win
        return 0.;
    }

    auto itMultiplier = itMultipliers->second.find(eSymbol);
    if (itMultiplier == itMultipliers->second.end())
    {
        // No winning has been found for this symbol - it should never happen
        return 0.;
    }

    return itMultiplier->second * DEFAULT_BET;
}

const std::string getSymbolName(Symbol_t eSymbol)
{
    switch (eSymbol)
    {
        case SYMBOL_SCATTER:
        {
            return "FIG_SCATTER";
        }
        case SYMBOL_CHERRY:
        {
            return "FIG_1";
        }
        case SYMBOL_LEMON:
        {
            return "FIG_2";
        }
        case SYMBOL_ORANGE:
        {
            return "FIG_3";
        }
        case SYMBOL_PLUM:
        {
            return "FIG_4";
        }
        case SYMBOL_GRAPES:
        {
            return "FIG_5";
        }
        case SYMBOL_WATERMELON:
        {
            return "FIG_6";
        }
        case SYMBOL_SEVEN:
        {
            return "FIG_7";
        }
    }

    return "Invalid symbol";
}
