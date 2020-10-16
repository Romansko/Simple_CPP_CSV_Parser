// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CCSVParser.h"
#include <fstream>

struct SampleStruct
{
	int32_t     intVal;
	float       floatVal;
	const char* stringVal;
};

enum
{
	INT_VAL,
	FLOAT_VAL,
	STR_VAL,
	ELEMENTS_NUM
};

SampleStruct* makeStructTable(CCSVParser& parser)
{
	SampleStruct* stb = nullptr;
	if (parser.getColumnsSize() == ELEMENTS_NUM)
	{
		const int rowSize = parser.getRowSize();
		stb = new SampleStruct[rowSize];
		for (auto i = 0; i < rowSize; ++i)
		{
			parser.getValue(i, INT_VAL,   stb[i].intVal);
			parser.getValue(i, FLOAT_VAL, stb[i].floatVal);
			parser.getValue(i, STR_VAL,    &(stb[i].stringVal));
		}
	}
	return stb;
}

int main()
{
	CCSVParser parser;
	SampleStruct* stb = nullptr;
	if (parser.parse(".//TestData//input.csv"))  // path to example data.
	{
		table_t tb = parser.getTable();

		row_t row;
		row.push_back("Strings");
		row.push_back("Integer Values");
		row.push_back("Float Values");
		parser.filter(row);
		parser.log();

		stb = makeStructTable(parser);
	}

	if (stb != nullptr)
	{
		try
		{
			std::ofstream output;
			output.open(".//TestData//output.csv");
			const int32_t headerSize = parser.getHeader().size();
			auto hdr = parser.getHeader();
			auto title = hdr.begin();
			for (; title != hdr.end() - 1; ++title)
			{
				output << *title << ",";
			}
			output << *title << std::endl;

			for (uint32_t i = 0; i < parser.getRowSize(); ++i)
			{
				output << stb[i].intVal << "," << stb[i].floatVal << "," << stb[i].stringVal << std::endl;
			}
			output.close();
		}
		catch(...) { /* Do Nothing */ }
		delete[] stb;
	}
 
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
