#pragma once

#include <unordered_map>
#include <sstream>
#include <iostream>

#include "const.h"

class SlotingMachine
{
	public:
		SlotingMachine(int nGameQuantity, double dbStartCredit, const std::string_view& strFileName);
		SlotingMachine();
		~SlotingMachine();

		void run();
		void run(const std::vector<Symbol_t>& vecSymbols);

	private:
		void spin(ReportDataPtr pData);
		void getSymbols(const Cylinder_t nCylinderId);
		void checkWinningLines(ReportDataPtr pData, const std::vector<Symbol_t>& vecSymbols);
		void printReport(const double& nTotalValue, const double& dbHighestWin, 
			const int& nTotalBet, const int& nTotalWins,
			const std::ostringstream& strReport, const std::ostringstream& strAdditionalReport);

		bool checkWin(ReportDataPtr pData, Symbol_t eSymbol, int nCounter);

		int m_nGameQuantity;

		double m_dbStartCredit;

		std::string_view m_strFileName;
		std::unordered_map<int, std::vector<Symbol_t>> m_umapPredefinedSymbols;
};