#include <fstream>
#include <iomanip>

#include "slotingMachine.h"
#include "tools.h"

////////////////////////////////////////////////////////////////////
// Name:        SlotingMachine
// Parameters:  -
// Description: 
////////////////////////////////////////////////////////////////////
SlotingMachine::SlotingMachine():
    m_nGameQuantity(1), m_dbStartCredit(0)
{
    
}

////////////////////////////////////////////////////////////////////
// Name:        SlotingMachine
// Parameters:  int nGameQuantity
//              double dbStartCredit
//              const std::string_view& strFileName
// Description: Fills set the sequence of symbols for all cylinders
////////////////////////////////////////////////////////////////////
SlotingMachine::SlotingMachine(int nGameQuantity, double dbStartCredit, const std::string_view& strFileName):
    m_nGameQuantity(nGameQuantity), m_dbStartCredit(dbStartCredit), m_strFileName(strFileName)
{
    for (int nCylinderId = CYLINDER_FIRST; nCylinderId <= CYLINDER_LAST; ++nCylinderId)
    {
        getSymbols(static_cast<Cylinder_t>(nCylinderId));
    }
}

////////////////////////////////////////////////////////////////////
// Name:        ~SlotingMachine
// Parameters:  -
// Description: Default destructor
////////////////////////////////////////////////////////////////////
SlotingMachine::~SlotingMachine()
{

}

////////////////////////////////////////////////////////////////////
// Name:        getSymbols
// Parameters:  const Cylinder_t nCylinderId
// Description: Fills the vector with symbols depends on the cylinder id
////////////////////////////////////////////////////////////////////
void SlotingMachine::getSymbols(const Cylinder_t nCylinderId)
{
    auto& vecSymbols = m_umapPredefinedSymbols[nCylinderId];
    std::unordered_map<Symbol_t, int> symbolChances;
    switch (nCylinderId)
    {
        case CYLINDER_1:
        {
            symbolChances = {
                { SYMBOL_CHERRY,        45 },
                { SYMBOL_LEMON,         10 },
                { SYMBOL_ORANGE,        10 },
                { SYMBOL_PLUM,          10 },
                { SYMBOL_GRAPES,        15 },
                { SYMBOL_WATERMELON,    20 },
                { SYMBOL_SEVEN,         25 },
                { SYMBOL_SCATTER,       2 }
            };
            break;
        }
        case CYLINDER_2:
        {
            symbolChances = {
                { SYMBOL_CHERRY,        15 },
                { SYMBOL_LEMON,         30 },
                { SYMBOL_ORANGE,        25 },
                { SYMBOL_PLUM,          20 },
                { SYMBOL_GRAPES,        15 },
                { SYMBOL_WATERMELON,    10 },
                { SYMBOL_SEVEN,         5 },
                { SYMBOL_SCATTER,       2 }
            };
            break;
        }
        case CYLINDER_3:
        {
            symbolChances = {
                { SYMBOL_CHERRY,        5 },
                { SYMBOL_LEMON,         15 },
                { SYMBOL_ORANGE,        15 },
                { SYMBOL_PLUM,          30 },
                { SYMBOL_GRAPES,        15 },
                { SYMBOL_WATERMELON,    30 },
                { SYMBOL_SEVEN,         15 },
                { SYMBOL_SCATTER,       2 }
            };
            break;
        }
        case CYLINDER_4:
        {
            symbolChances = {
                { SYMBOL_CHERRY,        10 },
                { SYMBOL_LEMON,         15 },
                { SYMBOL_ORANGE,        5 },
                { SYMBOL_PLUM,          5 },
                { SYMBOL_GRAPES,        10 },
                { SYMBOL_WATERMELON,    15 },
                { SYMBOL_SEVEN,         5 },
                { SYMBOL_SCATTER,       2 }
            };
            break;
        }
        case CYLINDER_5:
        {
            symbolChances = {
                { SYMBOL_CHERRY,        40 },
                { SYMBOL_LEMON,         10 },
                { SYMBOL_ORANGE,        15 },
                { SYMBOL_PLUM,          25 },
                { SYMBOL_GRAPES,        10 },
                { SYMBOL_WATERMELON,    5 },
                { SYMBOL_SEVEN,         10 },
                { SYMBOL_SCATTER,       2 }
            };
            break;
        }
        default:
            break;
    }

    for (const auto& itSymbol : symbolChances)
    {
        for (auto i = 0; i < itSymbol.second; ++i)
        {
            vecSymbols.push_back(itSymbol.first);
        }
    }
}

////////////////////////////////////////////////////////////////////
// Name:        checkWin
// Parameters:  ReportDataPtr pData
//              Symbol_t eSymbol
//              int nCounter
// Description: Checks that there is enough symbols to get the win
////////////////////////////////////////////////////////////////////
bool SlotingMachine::checkWin(ReportDataPtr pData, Symbol_t eSymbol, int nCounter)
{
    if (nCounter == 0)
    {
        // No symbols found, it should never happen
        return false;
    }

    const double dbMultiplier = getSymbolMultiplier(eSymbol, nCounter);
    if (dbMultiplier == 0.)
    {
        return false;
    }

    // Update the data
    pData->bWin = true;
    pData->nWinningValue += dbMultiplier;
    pData->addData(eSymbol, nCounter, dbMultiplier);
    return true;
}

void SlotingMachine::checkWinningLines(ReportDataPtr pData, const std::vector<Symbol_t>& vecSymbols)
{
    for (auto& itLine : WINNING_LINES)
    {
        Symbol_t eSymbol = SYMBOL_NONE;
        int nCounter = 0;
        for (auto& itCell : itLine)
        {
            Symbol_t eCurrentSymbol = vecSymbols.at(itCell);
            if (eCurrentSymbol == SYMBOL_SCATTER)
            {
                // Ignore scatter symbols, we will check them later
                break;
            }

            if (eSymbol == SYMBOL_NONE)
            {
                // Get the symbol from the first slot
                nCounter++;
                eSymbol = eCurrentSymbol;
                continue;
            }

            if (eSymbol != eCurrentSymbol)
            {
                if (nCounter < 2)
                {
                    // There is no second symbol matching the scheme
                    eSymbol = SYMBOL_NONE;
                }

                break;
            }

            nCounter++;
        }

        // The eSymbol is set to SYMBOL_NONE if there is no winning
        if (eSymbol != SYMBOL_NONE && checkWin(pData, eSymbol, nCounter))
        {
            pData->vecSymbols = vecSymbols;
        }
    }

    // Check the scatters symbols
    int nScatterQuantity = 0;
    for (int i = CYLINDER_FIRST; i <= CYLINDER_LAST; ++i)
    {
        // Slots numbering
        //  0  1  2  3  4
        //  5  6  7  8  9
        // 10 11 12 13 14
        for (int j = ROW_FIRST; j <= ROW_LAST; ++j)
        {
            const int nIndex = (j * 5) + i;
            Symbol_t eCurrentSymbol = vecSymbols.at(nIndex);
            if (eCurrentSymbol == SYMBOL_SCATTER)
            {
                nScatterQuantity++;
                break;
            }
        }
    }

    if (!checkWin(pData, SYMBOL_SCATTER, nScatterQuantity))
    {
        return;
    }

    pData->vecSymbols = vecSymbols;
}

////////////////////////////////////////////////////////////////////
// Name:        spin
// Parameters:  ReportDataPtr pData
// Description: Spin the virtual cylinders to get arandom sequence of
//              symbols on each cylinder, then compare the results with
//              the winning lines
////////////////////////////////////////////////////////////////////
void SlotingMachine::spin(ReportDataPtr pData)
{
    std::vector<Symbol_t> vecSymbols;
    for (int nRows = ROW_FIRST; nRows <= ROW_LAST; ++nRows)
    {
        for (int nCylinderId = CYLINDER_FIRST; nCylinderId <= CYLINDER_LAST; ++nCylinderId)
        {
            // Get the set of symbols assigned to the specific cylinder and get the random symbol
            const auto& vecPredefinedSymbols = m_umapPredefinedSymbols.at(nCylinderId);
            const auto nIndex = getRandomRangeNumber(0, static_cast<int>(vecPredefinedSymbols.size()) - 1);
            const Symbol_t eSymbol = vecPredefinedSymbols.at(nIndex);

            vecSymbols.push_back(eSymbol);
        }
    }

    checkWinningLines(pData, vecSymbols);
}

////////////////////////////////////////////////////////////////////
// Name:        run
// Parameters:  const std::vector<int>& vecSymbols
// Description: Check the winnings for specific symbols sequence
////////////////////////////////////////////////////////////////////
void SlotingMachine::run(const std::vector<Symbol_t>& vecSymbols)
{
    auto pData = std::make_shared<ReportData>();
    checkWinningLines(pData, vecSymbols);
    std::cout << pData->nWinningValue << std::endl;
}

////////////////////////////////////////////////////////////////////
// Name:        run
// Parameters:  -
// Description: Run m_nGameQuantity times the slot machine and print
//              the results
////////////////////////////////////////////////////////////////////
void SlotingMachine::run()
{
    std::ostringstream strOutput, strReport, strAdditionalReport;
    double dbTotalValue = 0.,
           dbTotalValueInRange = 0.,
           dbHighestWin = 0.;
    int nTotalBet = 0, 
        nTotalWins = 0,
        nTotalWinsInRange = 0,
        nCounter = 0,
        nTotalHits = 0;

    std::vector<ReportDataPtr> vecReportData(m_nGameQuantity);
    std::for_each(std::execution::par_unseq, vecReportData.begin(), vecReportData.end(), [&](auto& pData)
        {
            pData = std::make_shared<ReportData>();
            spin(pData);
        });

    // Count total wins and winning value
    for (auto& pData : vecReportData)
    {
        dbTotalValue += pData->nWinningValue;
        nTotalWins += pData->bWin ? 1 : 0;
    }

    WinningLines umapLineWins;
    for(auto& pData : vecReportData)
    {
        m_dbStartCredit += pData->nWinningValue - DEFAULT_BET;
        dbTotalValueInRange += pData->nWinningValue;
        dbHighestWin = std::max(dbHighestWin, pData->nWinningValue);

        nTotalWinsInRange += pData->bWin ? 1 : 0;

        nTotalBet += DEFAULT_BET;
        strOutput << m_dbStartCredit << std::endl;

        pData->merge(umapLineWins, nTotalHits);

        if (++nCounter % 1000 == 0)
        {
            strReport << "\n" << (nCounter - 1000) << "-" << nCounter;
            strReport << " " << nTotalWinsInRange << " " << (nTotalWinsInRange * 1. / nTotalWins * 100) << "%";
            strReport << " " << dbTotalValueInRange << " " << (dbTotalValueInRange / dbTotalValue * 100) << "%";
            nTotalWinsInRange = 0;
            dbTotalValueInRange = 0;
        }
    }

    strAdditionalReport << "\n\nName Multiplier Hits Hits% TotalValue TotalValue% 1/(w), 1/(g)";
    for (const auto& itWinningLine : umapLineWins)
    {
        const std::string strSymbolName = getSymbolName(itWinningLine.first);
        for (auto& itEntry : itWinningLine.second)
        {
            const double dbSymbolMultiplier = getSymbolMultiplier(itWinningLine.first, itEntry.first);
            const double dbSymbolTotalValue = dbSymbolMultiplier * itEntry.second;

            //                             Name                                              Multiplier
            strAdditionalReport << "\n" << strSymbolName << " x" << itEntry.first << " " << dbSymbolMultiplier;

            //                             Hits                      Hits%
            strAdditionalReport << " " << itEntry.second << " " << (itEntry.second * 1. / nTotalHits * 100) << "%";

            //                             TotalValue                                 TotalValue%
            strAdditionalReport << " " << dbSymbolTotalValue << " " << (dbSymbolTotalValue / dbTotalValue * 100) << "%";

            //                             1/(w)
            strAdditionalReport << " " << (nTotalWins / itEntry.second);

            //                             1/(g)
            strAdditionalReport << " " << (m_nGameQuantity / itEntry.second);
        }
    }

    // Write the string to the file
    std::ofstream fOutputFile(m_strFileName.data());
    fOutputFile << strOutput.str();
    fOutputFile.close();

    // Print the shorten report
    printReport(dbTotalValue, dbHighestWin, nTotalBet, nTotalWins, strReport, strAdditionalReport);
}

////////////////////////////////////////////////////////////////////
// Name:        printReport
// Parameters:  const double& dbTotalValue
//              const double& dbHighestWin
//              const int& nTotalBet
//              const int& nTotalWins
//              const std::ostringstream& strReport
// Description: Print the shorten report to the console
////////////////////////////////////////////////////////////////////
void SlotingMachine::printReport(const double& dbTotalValue, const double& dbHighestWin, 
    const int& nTotalBet, const int& nTotalWins, 
    const std::ostringstream& strReport, const std::ostringstream& strAdditionalReport)
{
    std::cout << "\n*****************************************************************************";
    std::cout << "\n**************************** Shorten Report *********************************";
    std::cout << "\n*****************************************************************************";
    std::cout << "\nModule: slotGameSimulator (version 1.0)";
    std::cout << "\nGames quantity: " << m_nGameQuantity;
    std::cout << "\nOne game bet (BET): " << DEFAULT_BET;
    std::cout << "\nTotal stakes collected: " << m_nGameQuantity << " * " << DEFAULT_BET << " = " << (m_nGameQuantity * DEFAULT_BET);
    std::cout << "\n\nWin distribution (percentage of games won in typical ranges):";
    std::cout << "\nrange count count% value value%";
    std::cout << strReport.str();

    std::cout << "\n\nHighest win achieved during the simulation: " << dbHighestWin;
    std::cout << "\nTotal RTP: " << (dbTotalValue * 100. / nTotalBet) << "% (sum of stakes: " << (m_nGameQuantity * DEFAULT_BET) << ", sum of winnings: " << std::fixed << std::setprecision(2) << dbTotalValue << ")";
    std::cout << "\nTotal HF (Hit Frequency): " << (nTotalWins * 100. / m_nGameQuantity) << "%";
    std::cout << "\nTotal balance remaining: " << m_dbStartCredit;

    std::cout << strAdditionalReport.str() << "\n\n";
}
