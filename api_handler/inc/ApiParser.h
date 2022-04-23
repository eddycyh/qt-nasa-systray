#ifndef _APIPARSER_H_
#define _APIPARSER_H_

#include <string>

namespace durr {

class ApiParser {
public: 
	ApiParser() {};
	virtual ~ApiParser() {};
	bool parseAPODInfo(std::string& content, std::string& date, std::string& title);
	bool parseAPODInfo(std::string& content, std::string& date, std::string& title,
		std::string& imgUrl, std::string& explanation);
	bool parseNeowsInfo(std::string& content, std::string& name, std::string& estDt, 
		std::string& missDistance);
	bool parseNeowsInfo(std::string& content, int& num);
};

}

#endif // _APIPARSER_H_
