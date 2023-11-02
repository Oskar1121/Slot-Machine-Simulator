#pragma once

#include <map>
#include <vector>
#include <execution>

static constexpr auto DEFAULT_BET = 100;

enum Symbol_t : int8_t
{
    SYMBOL_NONE = -1,
    SYMBOL_SCATTER = 0,
    SYMBOL_CHERRY,
    SYMBOL_LEMON,
    SYMBOL_ORANGE,
    SYMBOL_PLUM,
    SYMBOL_GRAPES,
    SYMBOL_WATERMELON,
    SYMBOL_SEVEN
};

enum Cylinder_t : int8_t
{
    CYLINDER_1 = 0,
    CYLINDER_FIRST = CYLINDER_1,
    CYLINDER_2,
    CYLINDER_3,
    CYLINDER_4,
    CYLINDER_5,
    CYLINDER_LAST = CYLINDER_5
};

enum Rows_t : int8_t
{
    ROW_1 = 0,
    ROW_FIRST = ROW_1,
    ROW_2,
    ROW_3,
    ROW_LAST = ROW_3
};

typedef std::map<Symbol_t, std::map<int, int>> WinningLines;

struct ReportData
{
    ReportData() : bWin(false), nWinningValue(0.) {}

    void merge(WinningLines& umapLines, int& nTotalHits)
    {
        for (const auto& itEntry : umapLineWins)
        {
            const Symbol_t eSymbol = itEntry.first;
            auto& umapEntry = umapLines[eSymbol];
            for (const auto& itSubEntry : itEntry.second)
            {
                umapEntry[itSubEntry.first] += itSubEntry.second;
                nTotalHits += itSubEntry.second;
            }
        }
    }

    void addData(Symbol_t eSymbol, const int& nSymbolQuantity, const double& dbMultiplier)
    {
        const auto& itMapQuantity = umapLineWins.find(eSymbol);
        if (itMapQuantity == umapLineWins.end())
        {
            std::map<int, int> umapWins;
            umapWins.emplace(nSymbolQuantity, 1);
            umapLineWins.emplace(eSymbol, umapWins);
        }
        else
        {
            const auto& itQuantity = itMapQuantity->second.find(nSymbolQuantity);
            if (itQuantity == itMapQuantity->second.end())
            {
                itMapQuantity->second.emplace(nSymbolQuantity, 1);
            }
            else
            {
                itQuantity->second++;
            }
        }
    }

    bool bWin;
    double nWinningValue;
    WinningLines umapLineWins;
    std::vector<Symbol_t> vecSymbols;
};

typedef std::shared_ptr<ReportData> ReportDataPtr;

// Slots numbering
//  0  1  2  3  4
//  5  6  7  8  9
// 10 11 12 13 14

static const std::vector<std::vector<int>> WINNING_LINES = {
    // . . . . .
    // 1 1 1 1 1
    // . . . . .
    {5, 6, 7, 8, 9},

    // 1 1 1 1 1
    // . . . . .
    // . . . . .
    {0, 1, 2, 3, 4},

    // . . . . .
    // . . . . .
    // 1 1 1 1 1
    {10, 11, 12, 13, 14},

    // . . 1 . .
    // 1 1 . 1 1
    // . . . . .
    {5, 6, 2, 8, 9},

    // 1 . . . 1
    // . 1 . 1 .
    // . . 1 . .
    {0, 6, 12, 8, 4},

    // . . 1 . .
    // . 1 . 1 .
    // 1 . . . 1
    {10, 6, 2, 8, 14},

    // . 1 . 1 .
    // 1 . 1 . 1
    // . . . . .
    {5, 1, 7, 3, 9},

    // . . . . .
    // 1 . 1 . 1
    // . 1 . 1 .
    {5, 11, 7, 13, 9},

    // 1 1 . . .
    // . . 1 . .
    // . . . 1 1
    {0, 1, 7, 13, 14},

    // . . . 1 1
    // . . 1 . .
    // 1 1 . . .
    {10, 11, 7, 3, 4},

    // . . 1 . .
    // . 1 . 1 1
    // 1 . . . .
    {10, 6, 2, 8, 9},

    // . . 1 1 1
    // . 1 . . .
    // 1 . . . .
    {10, 6, 2, 3, 4},

    // 1 . . . .
    // . 1 . . .
    // . . 1 1 1
    {0, 6, 12, 13, 14},

    // 1 . . . .
    // . 1 . 1 1
    // . . 1 . .
    {0, 6, 12, 8, 9},

    // 1 . 1 . 1
    // . 1 . 1 .
    // . . . . .
    {0, 6, 2, 8, 4},

    // . . . . .
    // . 1 . 1 .
    // 1 . 1 . 1
    {10, 6, 12, 8, 14},

    // 1 1 . . .
    // . . 1 1 1
    // . . . . .
    {0, 1, 7, 8, 9},

    // . . . . .
    // . . 1 1 1
    // 1 1 . . .
    {10, 11, 7, 8, 9},

    // . . . . 1
    // . . . 1 .
    // 1 1 1 . .
    {10, 11, 12, 8, 4},

    // 1 1 1 . .
    // . . . 1 .
    // . . . . 1
    {0, 1, 2, 8, 14}
};

static std::unordered_map<int, std::unordered_map<Symbol_t, double>> SYMBOL_MULTIPLIERS = {
    // Two sumbols in a row (cherrys only)
    {2, {
        {SYMBOL_CHERRY, 0.25}
    }},

    // Three symbols in a row
    {3, {
        {SYMBOL_CHERRY, 1.},
        {SYMBOL_LEMON, 1.},
        {SYMBOL_ORANGE, 1.},
        {SYMBOL_PLUM, 1.},
        {SYMBOL_GRAPES, 2.5},
        {SYMBOL_WATERMELON, 2.5},
        {SYMBOL_SEVEN, 5.},
        {SYMBOL_SCATTER, 2.}
    }},

    // Four symbols in a row
    {4, {
        { SYMBOL_CHERRY, 2.5 },
        { SYMBOL_LEMON, 2.5 },
        { SYMBOL_ORANGE, 2.5 },
        { SYMBOL_PLUM, 2.5 },
        { SYMBOL_GRAPES, 10. },
        { SYMBOL_WATERMELON, 10. },
        { SYMBOL_SEVEN, 50. },
        { SYMBOL_SCATTER, 10. }
    }},

    // Five symbols in a row
    {5, {
        { SYMBOL_CHERRY, 10. },
        { SYMBOL_LEMON, 10. },
        { SYMBOL_ORANGE, 10. },
        { SYMBOL_PLUM, 10. },
        { SYMBOL_GRAPES, 25. },
        { SYMBOL_WATERMELON, 25. },
        { SYMBOL_SEVEN, 250. },
        { SYMBOL_SCATTER, 50. }
    }},
};
