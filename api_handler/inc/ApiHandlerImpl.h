#ifndef _APIHANDLERIMPL_H_
#define _APIHANDLERIMPL_H_

#include "IApiHandler.h"
#include "ApiParser.h"
#include "curl/curl.h"

namespace durr {

class ApiHandlerImpl : public IApiHandler {
public:
	ApiHandlerImpl(std::string apiKey);
	virtual ~ApiHandlerImpl();
	bool getAPODBasicInfo(std::string& date, std::string& title) override;
	bool getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl, 
		std::string& explanation) override;
	bool listNeows(std::string& startDate, std::string& endDate, int& num) override;
	bool getNearestNeowsInfo(std::string& startDate, std::string& endDate, std::string& name, std::string& estDt,
		std::string& missDistance) override;
	bool downloadImage(std::string& url, std::string& path) override;
private:
	CURL* curl = nullptr;
	std::string apiKey;
	std::shared_ptr<ApiParser> parser = nullptr;
};

}

#endif	//_APIHANDLERIMPL_H_