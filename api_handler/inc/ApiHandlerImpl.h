#ifndef _APIHANDLERIMPL_H_
#define _APIHANDLERIMPL_H_

#include "IApiHandler.h"
#include "curl/curl.h"

namespace durr {

class ApiHandlerImpl : public IApiHandler {
public:
	ApiHandlerImpl();
	virtual ~ApiHandlerImpl();
	bool getAPODBasicInfo(std::string& date, std::string& title) override;
	bool getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl, 
		std::string& explanation) override;
	bool listNeows(int& num) override;
	bool getNearestNeowsInfo(std::string& name, std::string& estDt, 
		std::string& missDistance) override;
private:
	CURL* curl = nullptr;
};

}

#endif	//_APIHANDLERIMPL_H_