#ifndef CCSVParser_H
#define CCSVParser_H

#include <string>
#include <vector>

typedef std::vector<std::string> row_t;
typedef std::vector<row_t> table_t;

class CCSVParser
{
	
public:
	enum { HEADER_INDEX = 0};
	
	CCSVParser();
	virtual ~CCSVParser();

	virtual void reset();
	virtual bool parse(const char* filePath);
	virtual void removeByTitle(const std::string& title);
	virtual void filter(row_t& columnsToKeep);
	
	virtual table_t getTable();
	virtual size_t  getColumnsSize();
	virtual size_t  getRowSize();
	virtual row_t   getHeader();
	
	virtual bool    getValue(uint32_t row, uint32_t col, int32_t&     parsedVal);
	virtual bool    getValue(uint32_t row, uint32_t col, int64_t&     parsedVal);
	virtual bool    getValue(uint32_t row, uint32_t col, float&       parsedVal);
	virtual bool    getValue(uint32_t row, uint32_t col, double&      parsedVal);
	virtual bool    getValue(uint32_t row, uint32_t col, const char** parsedVal);
	
	virtual void log();
	
protected:
	virtual void removeRow(uint32_t index);
	virtual void removeByTitle(uint32_t index);
	virtual void log(const std::string& msg);

	
	table_t m_table;  // table assumed to be MxN sized.
};

#endif
