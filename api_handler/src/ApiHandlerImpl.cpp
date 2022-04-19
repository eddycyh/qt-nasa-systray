#include "ApiHandlerImpl.h"
#include <stdio.h>

using namespace durr;

ApiHandlerImpl::ApiHandlerImpl() {
	curl = curl_easy_init();
}

ApiHandlerImpl::~ApiHandlerImpl() {
	curl_easy_cleanup(curl);
}

bool ApiHandlerImpl::getAPODBasicInfo(std::string& date, std::string& title) {
	CURLcode res;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.nasa.gov/planetary/apod?api_key=70O0cmhoP9IZshLlLVoGlN0kKq0qNy6cjvZeIvLx");
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			char* ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct)
				printf("We received Content-Type: %s\n", ct);
		}
	}
	return false;
}

bool ApiHandlerImpl::getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl,
	std::string& explanation) {
	return true;
}

bool ApiHandlerImpl::listNeows(int& num) {
	return true;
}

bool ApiHandlerImpl::getNearestNeowsInfo(std::string& name, std::string& estDt,
	std::string& missDistance) {
	return true;
}