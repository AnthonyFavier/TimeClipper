#include "../include/logManager.hpp"

LogManager::LogManager()
{
	m_file.open("log.txt", ios::out);
	m_file << "opened" << endl;
}

LogManager::~LogManager()
{
	m_file.close();
}

LogManager& operator <<(LogManager& obj, const string str)
{
	obj.m_file << str;
	cout << str;
	return obj;
}
LogManager& operator <<(LogManager& obj, const char* str)
{
	obj.m_file << str;
	cout << str;
	return obj;
}
LogManager& operator <<(LogManager& obj, int nb)
{
	obj.m_file << nb;
	cout << nb;
	return obj;
}
LogManager& operator <<(LogManager& obj, long unsigned int nb)
{
	obj.m_file << nb;
	cout << nb;
	return obj;
}
LogManager& operator <<(LogManager& obj, float nb)
{
	obj.m_file << nb;
	cout << nb;
	return obj;
}
LogManager& operator <<(LogManager& obj, double nb)
{
	obj.m_file << nb;
	cout << nb;
	return obj;
}
LogManager& operator <<(LogManager& obj, bool var)
{
	obj.m_file << var;
	cout << var;
	return obj;
}
LogManager& operator <<(LogManager& obj, ostream&(*f)(ostream&))
{
	obj.m_file << f;
	cout << f;
	return obj;
}
