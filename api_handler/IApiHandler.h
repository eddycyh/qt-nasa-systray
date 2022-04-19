#ifndef _IAPIHANDLER_H_
#define _IAPIHANDLER_H_

#include <memory>
#include <string>

namespace durr {

class IApiHandler {
public:
	static std::shared_ptr<IApiHandler> getInstance();
	static void deleteInstance();
	IApiHandler();
	virtual ~IApiHandler() {};
	virtual bool getAPODBasicInfo(std::string& date, std::string& title) = 0;
	virtual bool getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl, std::string& explanation) = 0;
	virtual bool listNeows(int& num) = 0;
	virtual bool getNearestNeowsInfo(std::string& name, std::string& estDt, std::string& missDistance) = 0;
};

}

#endif	//_IAPIHANDLER_H_