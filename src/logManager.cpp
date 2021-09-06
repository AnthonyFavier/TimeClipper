#include "../include/logManager.hpp"

LogManager::LogManager()
{
	m_file.open("log.txt", ios::out);
	m_file << "opened" << endl;
	m_active = true;
}

LogManager::~LogManager()
{
	m_file.close();
}

void LogManager::activate()
{
	m_active = true;
}

void LogManager::disable()
{
	m_active = false;
}

bool LogManager::isActive()
{
	return m_active;
}

LogManager& operator <<(LogManager& obj, const string str)
{
	if(obj.isActive())
	{
		obj.m_file << str;
		cout << str;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, const char* str)
{
	if(obj.isActive())
	{
		obj.m_file << str;
		cout << str;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, int nb)
{
	if(obj.isActive())
	{
		obj.m_file << nb;
		cout << nb;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, long unsigned int nb)
{
	if(obj.isActive())
	{
		obj.m_file << nb;
		cout << nb;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, float nb)
{
	if(obj.isActive())
	{
		obj.m_file << nb;
		cout << nb;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, double nb)
{
	if(obj.isActive())
	{
		obj.m_file << nb;
		cout << nb;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, bool var)
{
	if(obj.isActive())
	{
		obj.m_file << var;
		cout << var;
	}
	return obj;
}
LogManager& operator <<(LogManager& obj, ostream&(*f)(ostream&))
{
	if(obj.isActive())
	{
		obj.m_file << f;
		cout << f;
	}
	return obj;
}
