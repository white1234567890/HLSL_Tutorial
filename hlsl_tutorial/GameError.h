#pragma once
//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/1/11
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <exception>

namespace gameErrorNS
{
	//Error code
	//Negative value is must shut down critical error
	//Positive valie is warning

	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

//GameError class
//If error was detected by GameEngine, this is throwed
//Inherit std::exception
class GameError :
	public std::exception
{
private:
	int errorCode;
	std::string message;

public:
	//Default constructor
	GameError(void) throw() : errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game") {}

	//Copy constructor
	GameError(const GameError& e) throw() : std::exception(e), errorCode(e.errorCode), message(e.message) {}

	//Constructor with argument
	GameError(int code, const std::string &s) throw() : errorCode(code), message(s) {}

	//Assignment operator
	GameError& operator = (const GameError& rhs) throw()
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}

	//Destructor
	virtual ~GameError() throw() {};

	//Override on super class
	virtual const char* what() const throw() {return this->getMessage();}
	const char* getMessage() const throw() {return message.c_str();}
	int getErrorCode() const throw() {return errorCode;}
};
