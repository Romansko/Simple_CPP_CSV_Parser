#ifndef CCSVParser_H
#define CCSVParser_H

#include <string>
#include <vector>

typedef std::vector<std::string> row_t;
typedef std::vector<row_t> table_t;

class CCSVParser
{
	
public:
	enum { TITLES_INDEX = 0};
	
	CCSVParser();
	~CCSVParser();
	virtual table_t getTable();
	virtual void reset();
	virtual bool parse(const char* filePath);
	virtual void removeByTitle(const std::string& title);
	virtual void filter(row_t& columnsToKeep);
	virtual void log();
	
protected:
	virtual void removeRow(unsigned int index);
	virtual void removeByTitle(unsigned int index);
	virtual void log(const std::string& msg);

	
	table_t m_table;  // table assumed to be MxN sized.
};

#endif
