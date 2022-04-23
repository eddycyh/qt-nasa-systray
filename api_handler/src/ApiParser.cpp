#include <ApiParser.h>
#include <jsmn.h>

using namespace durr;

char resultBuffer[4192];

static int jsoneq(const char* json, jsmntok_t* tok, const char* s) {
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
		strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

bool ApiParser::parseAPODInfo(std::string& content, std::string& date, std::string& title) {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128];

	jsmn_init(&p);
	r = jsmn_parse(&p, content.c_str(), strlen(content.c_str()), t,
		sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		return false;
	}
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return false;
	}
	for (i = 1; i < r; i++) {
		if (jsoneq(content.c_str(), &t[i], "date") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			date = std::string(resultBuffer);
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "title") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			title = std::string(resultBuffer);
			i++;
		}
		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
				content.c_str() + t[i].start);
		}
	}
	return true;
}

bool ApiParser::parseAPODInfo(std::string& content, std::string& date, std::string& title,
	std::string& imgUrl, std::string& explanation) {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; 

	jsmn_init(&p);
	r = jsmn_parse(&p, content.c_str(), strlen(content.c_str()), t,
		sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		return false;
	}
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return false;
	}
	for (i = 1; i < r; i++) {
		if (jsoneq(content.c_str(), &t[i], "date") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			date = std::string(resultBuffer);
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "title") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			title = std::string(resultBuffer);
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "url") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			imgUrl = std::string(resultBuffer);
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "explanation") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			explanation = std::string(resultBuffer);
			i++;
		}
		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
				content.c_str() + t[i].start);
		}
	}
	return true;
}

bool ApiParser::parseNeowsInfo(std::string& content, std::string& name, std::string& estDt,
	std::string& missDistance) {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[2048];

	jsmn_init(&p);
	r = jsmn_parse(&p, content.c_str(), strlen(content.c_str()), t,
		sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		return false;
	}
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return false;
	}

	std::string tempName, tempDt, tempDistance;
	float fDistance = 0.0;
	for (i = 1; i < r; i++) {
		if (jsoneq(content.c_str(), &t[i], "name") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			if (fDistance) {
				tempName = std::string(resultBuffer);
			}
			else {
				name = std::string(resultBuffer);
			}
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "close_approach_date_full") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			if (fDistance) {
				tempDt = std::string(resultBuffer);
			}
			else {
				estDt = std::string(resultBuffer);
			}
			i++;
		}
		else if (jsoneq(content.c_str(), &t[i], "lunar") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			if (fDistance) {
				tempDistance = std::string(resultBuffer);
				fDistance = std::stof(resultBuffer);
				float tDistance = std::stof(missDistance);
				if (tDistance > fDistance) {
					name = tempName;
					estDt = tempDt;
					missDistance = tempDistance;
				}
			}
			else {
				missDistance = std::string(resultBuffer);
				fDistance = std::stof(resultBuffer);
			}
			i++;
		}
		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
				content.c_str() + t[i].start);
		}
	}
	return true;
}

bool ApiParser::parseNeowsInfo(std::string& content, int& num) {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[2048];

	jsmn_init(&p);
	r = jsmn_parse(&p, content.c_str(), strlen(content.c_str()), t,
		sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		return false;
	}
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return false;
	}
	for (i = 1; i < r; i++) {
		if (jsoneq(content.c_str(), &t[i], "element_count") == 0) {
			sprintf(resultBuffer, "%.*s", t[i + 1].end - t[i + 1].start, content.c_str() + t[i + 1].start);
			num = std::stoi(resultBuffer);
			i++;
		}
		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
				content.c_str() + t[i].start);
		}
	}
	return true;
}