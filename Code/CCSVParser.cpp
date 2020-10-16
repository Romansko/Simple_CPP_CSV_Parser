#include "CCSVParser.h"

#include <fstream>
#include <sstream>

#define DEBUG_MODE  // uncomment if not needed.
#ifdef DEBUG_MODE
#include <iomanip>
#include <Windows.h>
#endif

CCSVParser::CCSVParser()
{
}

CCSVParser::~CCSVParser()
{
}


void CCSVParser::reset()
{
	m_table.clear();
}


/**
 * Parse a csv file and load into a table.
 * Parsing will fail if the table isn't NxN dimension.
 */
bool CCSVParser::parse(const char* filePath)
{
	std::ifstream inputFileStream(filePath);
	if (!inputFileStream.is_open())
	{
		log("Open csv file failed!");
		// todo: error code
		return false;
	}
	log("csv file Parse started..");
	reset();

	uint32_t cellsSum = 0;  // for NxN validity.
	std::string stringLine;
	while (std::getline(inputFileStream, stringLine))
	{
		std::stringstream lineStream(stringLine);
		std::string cell;
		row_t parsedLine;
		while (std::getline(lineStream, cell, ','))
		{
			parsedLine.push_back(cell);
		}
		cellsSum += parsedLine.size();
		m_table.push_back(parsedLine);
	}

	// NxN validity test.
	if (!m_table.empty() && (cellsSum != m_table.size()*m_table[0].size()))
	{
		// Rows size unequal. CSV file is not a table!
		log("Rows size unequal. CSV file is not a table!");
		reset();
		return false;
	}
	
	return true;
}

/**
 * Removes a specific row by index, if exists.
 */
void CCSVParser::removeRow(uint32_t index)
{
	if (index < m_table.size())
	{
		m_table.erase(m_table.begin() + index);
	}
}

/**
 * Delete a specific column by index, if exists.
 */
void CCSVParser::removeByTitle(uint32_t index)
{
	if (!m_table.empty() && index < m_table[0].size())  // assume table is NxN sized.
	{
		for (auto row = m_table.rbegin(); row != m_table.rend(); ++row)
		{
			row->erase(row->begin() + index);
		}
	}
}

/**
 * Remove a column by title.
 */
void CCSVParser::removeByTitle(const std::string& title)
{
	if (m_table.empty())
	{
		return;
	}
	// scan title by reverse iterator.
	uint32_t index = m_table[HEADER_INDEX].size() - 1;
	row_t titlesCopy   = m_table[HEADER_INDEX];
	for (auto tbTitle = titlesCopy.rbegin(); tbTitle != titlesCopy.rend(); ++tbTitle)
	{
		if (title == *tbTitle)  // std::string == operator uses compare() function. so it's ok.
		{
			removeByTitle(index);
		}
		--index;
	}
}

/**
 * Filter table by given header vector.
 * The given titles will set which columns will be kept within the parsed table.
 */
void CCSVParser::filter(row_t& columnsToKeep)
{
	if (columnsToKeep.empty())
	{
		reset();
	}

	if (m_table.empty())
	{
		return;
	}

	// Scan titles and mark the columns to keep.
	bool* const toKeepArray = new bool[m_table[HEADER_INDEX].size()]{false};
	for (auto title = columnsToKeep.begin(); title != columnsToKeep.end(); ++title)
	{
		uint32_t index = 0;
		for (auto tbTitle = m_table[HEADER_INDEX].begin(); tbTitle != m_table[HEADER_INDEX].end(); ++tbTitle)
		{
			if (*title == *tbTitle)  // std::string == operator uses compare() function. so it's ok.
			{
				toKeepArray[index] = true;
			}
			++index;
		}
	}
	
	// reverse iteration idiom for unsigned loops.
	for (auto index = m_table[HEADER_INDEX].size(); index --> 0; )
	{
		if (!toKeepArray[index])
		{
			removeByTitle(index);
		}
	}
	delete[] toKeepArray;
}


/**
 * Returns the parsed table.
 * Note: Deep copy is performed on return.
 */
table_t CCSVParser::getTable()
{
	return m_table;
}

/***
 * Returns the number of columns in the table.
 */
size_t CCSVParser::getColumnsSize()
{
	if (m_table.empty())
	{
		return 0;
	}
	return m_table[HEADER_INDEX].size();
}

/**
 * Returns the number of rows in the table WITHOUT the header.
 */
size_t CCSVParser::getRowSize()
{
	return (m_table.size() - 1);  // without header row.
}

row_t CCSVParser::getHeader()
{
	row_t hdr;
	if (!m_table.empty())
	{
		hdr = m_table[HEADER_INDEX];
	}
	return hdr;
}



/***
 * given row & col index, return parsed value.
 * Note: row index is excluding header.
 */
bool CCSVParser::getValue(uint32_t row, uint32_t col, int32_t& parsedVal)
{
	try
	{
		parsedVal = std::stoi(m_table[row + 1][col]);
		return true;
	}
	catch (...) { /* Do Nothing */ }
	return false;
}

/***
 * given row & col index, return parsed value.
 * Note: row index is excluding header.
 */
bool CCSVParser::getValue(uint32_t row, uint32_t col, int64_t& parsedVal)
{
	try
	{
		parsedVal = std::stoll(m_table[row + 1][col]);
		return true;
	}
	catch (...) { /* Do Nothing */ }
	return false;
}

/***
 * given row & col index, return parsed value.
 * Note: row index is excluding header.
 */
bool CCSVParser::getValue(uint32_t row, uint32_t col, float& parsedVal)
{
	try
	{
		parsedVal = std::stof(m_table[row + 1][col]);
		return true;
	}
	catch (...) { /* Do Nothing */ }
	return false;
}

/***
 * given row & col index, return parsed value.
 * Note: row index is excluding header.
 */
bool CCSVParser::getValue(uint32_t row, uint32_t col, double& parsedVal)
{
	try
	{
		parsedVal = std::stod(m_table[row + 1][col]);
		return true;
	}
	catch (...) { /* Do Nothing */ }
	return false;
}


/***
 * given row & col index, return parsed value.
 * Note: row index is excluding header.
 */
bool CCSVParser::getValue(uint32_t row, uint32_t col, const char** parsedVal)
{
	try
	{
		*parsedVal = m_table[row + 1][col].c_str();
		return true;
	}
	catch (...) { /* Do Nothing */ }
	return false;
}

void CCSVParser::log()
{
#ifdef DEBUG_MODE
	const size_t gap = 20;
	std::stringstream toPrint;
	for (auto t1 = m_table.begin(); t1 != m_table.end(); ++t1)
	{
		auto row      = *t1;
		auto cell = row.begin();
		for (; cell != (row.end() - 1); ++cell)
		{
			toPrint.str("");
			toPrint << std::setw(gap) << *cell << ',';
			OutputDebugStringA(toPrint.str().c_str());
		}
		toPrint.str("");
		toPrint << std::setw(gap) << *cell << std::endl;
		OutputDebugStringA(toPrint.str().c_str());
	}
#endif
}

void CCSVParser::log(const std::string& msg)
{
#ifdef DEBUG_MODE
	OutputDebugStringA("@@@@@@@@@@@@@ LOG: ");
	OutputDebugStringA(msg.c_str());
	OutputDebugStringA("\n");
#endif
}
