#include "slotingMachine.h"

int main(int argc, char* argv[])
{
    int nGamesCount = 0;
    int nStartCredit = 0;
    std::string_view strFileName, strSymbolsParam;
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-gamesCount" && i + 1 < argc)
        {
            nGamesCount = std::stoi(argv[++i]);
            if (nGamesCount <= 0)
            {
                std::cerr << "Invalid game count value" << std::endl;
                return 1;
            }
        }
        else if (std::string(argv[i]) == "-startCredit" && i + 1 < argc)
        {
            nStartCredit = std::stoi(argv[++i]);
            if (nStartCredit <= 0)
            {
                std::cerr << "Invalid start credit value" << std::endl;
                return 1;
            }
        }
        else if (std::string(argv[i]) == "-creditOutFile" && i + 1 < argc)
        {
            strFileName = argv[++i];
            if (strFileName.empty())
            {
                std::cerr << "Missing output file name" << std::endl;
                return 1;
            }
        }
        else if (std::string(argv[i]) == "-symbols" && i + 1 < argc)
        {
            strSymbolsParam = argv[++i];
        }
    }

    if (!strSymbolsParam.empty())
    {
        std::vector<Symbol_t> vecSymbols;
        std::istringstream iss(strSymbolsParam.data());
        std::string token;

        while (std::getline(iss, token, ','))
        {
            try
            {
                int nSymbolValue = std::stoi(token);
                if (nSymbolValue >= SYMBOL_SCATTER && nSymbolValue <= SYMBOL_SEVEN)
                {
                    vecSymbols.push_back(static_cast<Symbol_t>(nSymbolValue));
                }
                else
                {
                    std::cerr << "Invalid symbol value: " << nSymbolValue << std::endl;
                    return 1;
                }
            }
            catch (const std::invalid_argument&)
            {
                std::cerr << "Invalid symbol: " << token << std::endl;
                return 1;
            }
        }

        std::unique_ptr<SlotingMachine> pSlotingMachine = std::make_unique<SlotingMachine>();
        pSlotingMachine->run(vecSymbols);
    }
    else if(!strFileName.empty())
    {
        std::unique_ptr<SlotingMachine> pSlotingMachine = std::make_unique<SlotingMachine>(nGamesCount, nStartCredit, strFileName);
        pSlotingMachine->run();
    }

    return 1;
}
