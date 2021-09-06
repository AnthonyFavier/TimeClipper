#ifndef DEF_LOGMANAGER
#define DEF_LOGMANAGER

#include <iostream>
#include <fstream>

using namespace std;

class LogManager
{
	friend LogManager& operator <<(LogManager&, const string);
	friend LogManager& operator <<(LogManager& obj, const char* str);
	friend LogManager& operator <<(LogManager&, int);
	friend LogManager& operator <<(LogManager&, long unsigned int);
	friend LogManager& operator <<(LogManager&, float);
	friend LogManager& operator <<(LogManager&, double);
	friend LogManager& operator <<(LogManager&, bool);
	friend LogManager& operator <<(LogManager&, ostream&(*f)(ostream&));
public:
	LogManager();
	~LogManager();
	void activate();
	void disable();
	bool isActive();
private:
	ofstream m_file;
	bool m_active;
};

#endif
