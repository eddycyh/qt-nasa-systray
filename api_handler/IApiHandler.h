#ifndef _IAPIHANDLER_H_
#define _IAPIHANDLER_H_

#include <memory>
#include <string>

namespace durr {

class IApiHandler {
public:
	static std::shared_ptr<IApiHandler> getInstance(std::string apiKey);
	static void deleteInstance();
	IApiHandler();
	virtual ~IApiHandler() {};
	virtual bool getAPODBasicInfo(std::string& date, std::string& title) = 0;
	virtual bool getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl, std::string& explanation) = 0;
	virtual bool listNeows(std::string& startDate, std::string& endDate, int& num) = 0;
	virtual bool getNearestNeowsInfo(std::string& startDate, std::string& endDate,std::string& name, std::string& estDt, std::string& missDistance) = 0;
	virtual bool downloadImage(std::string& url, std::string& path) = 0;
};

}

#endif	//_IAPIHANDLER_H_