#include "ApiHandlerImpl.h"
#include <stdio.h>
#include <iostream>

using namespace durr;

struct curl_slist* headers = NULL;

const char* apodurl = "https://api.nasa.gov/planetary/apod?api_key=%s";
const char* neowsUrl = "https://api.nasa.gov/neo/rest/v1/feed?start_date=%s&end_date=%s&api_key=%s";
char cUrl[384];

static size_t writer(char* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static size_t callbackfunction(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	FILE* stream = (FILE*)userdata;
	if (!stream)
	{
		printf("!!! No stream\n");
		return 0;
	}
	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	return written;
}


ApiHandlerImpl::ApiHandlerImpl(std::string apiKey) : apiKey(apiKey) {
	curl = curl_easy_init();
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");
	parser = std::make_shared<ApiParser>();
}

ApiHandlerImpl::~ApiHandlerImpl() {
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
}

bool ApiHandlerImpl::getAPODBasicInfo(std::string& date, std::string& title) {
	CURLcode res;
	std::string readBuffer;
	sprintf(cUrl, apodurl, apiKey.c_str());
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, cUrl);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			char* ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct) {
				return parser->parseAPODInfo(readBuffer, date, title);
			}
		}
	}
	return false;
}

bool ApiHandlerImpl::getAPODDetailInfo(std::string& date, std::string& title, std::string& imgUrl,
	std::string& explanation) {
	CURLcode res;
	std::string readBuffer;
	sprintf(cUrl, apodurl, apiKey.c_str());
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, cUrl);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			char* ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct) {
				return parser->parseAPODInfo(readBuffer, date, title, imgUrl, explanation);
			}
		}
	}
	return false;
}

bool ApiHandlerImpl::listNeows(std::string& startDate, std::string& endDate, int& num) {
	CURLcode res;
	std::string readBuffer;
	sprintf(cUrl, neowsUrl, startDate.c_str(), endDate.c_str(), apiKey.c_str());
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, cUrl);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			char* ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct) {
				return parser->parseNeowsInfo(readBuffer, num);
			}
		}
	}
	return false;
}

bool ApiHandlerImpl::getNearestNeowsInfo(std::string& startDate, std::string& endDate, std::string& name, std::string& estDt,
	std::string& missDistance) {
	CURLcode res;
	std::string readBuffer;
	sprintf(cUrl, neowsUrl, startDate.c_str(), endDate.c_str(), apiKey.c_str());
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, cUrl);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			char* ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct) {
				return parser->parseNeowsInfo(readBuffer, name, estDt, missDistance);
			}
		}
	}
	return false;
}

bool ApiHandlerImpl::downloadImage(std::string& url, std::string& path) {
	CURLcode res;
	bool ret = false;
	FILE* fp = fopen(path.c_str(), "wb");
	if (!fp)
	{
		printf("!!! Failed to create file on the disk\n");
		return false;
	}
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackfunction);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			ret = true;
		}
	}
	fclose(fp);
	return ret;
}