#include "IApiHandler.h"
#include "ApiHandlerImpl.h"

using namespace durr;

std::shared_ptr<IApiHandler> _apiInstance = nullptr;

IApiHandler::IApiHandler() {
}

std::shared_ptr<IApiHandler> IApiHandler::getInstance(std::string apiKey) {
	if (_apiInstance == nullptr) {
		_apiInstance = std::make_shared<ApiHandlerImpl>(apiKey);
	}
	return _apiInstance;
}

void IApiHandler::deleteInstance() {
	if (_apiInstance)
		_apiInstance.reset();
}