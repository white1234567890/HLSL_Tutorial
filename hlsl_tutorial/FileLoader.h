#pragma once
#include "singleton.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class FileLoader :
	public Singleton<FileLoader>
{
public:
	friend class Singleton<FileLoader>;

private:
	//property
	static std::ifstream ifs;	//input stream from file

	//forbiddone
	FileLoader(void){}
	FileLoader(const FileLoader &obj){}
	void operator = (const FileLoader &obj){}

protected:
	virtual ~FileLoader(void){}

public:
	static bool openCSVFile(std::string filename)
	{
		//open file
		ifs.open(filename.c_str());

		//if load fail
		if(ifs.fail())
		{
			ifs.close();
			return false;
		}

		return true;
	}

	static bool loadLineData_FROM_CSV(std::string &outStr)
	{
		if(!std::getline(ifs, outStr))
			return false;
		return true;
	}

	static void copyStringToStream(const std::string &inStr, std::istringstream &outStream)
	{
		outStream.str(inStr);
	}
	
	static void pickUpDataToToken(std::istringstream &inStream, std::string &token)
	{
		std::getline(inStream, token, ',');
	}
	
	static void convertDataToFloat(const std::string &token, float &convert)
	{
		convert = std::stof(token);
	}
	
	static void closeCSVFile()
	{
		ifs.close();
	}
};