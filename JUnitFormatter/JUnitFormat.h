#ifndef _JUNITFORMAT_LIB_H
#define _JUNITFORMAT_LIB_H

#include "../thirdparty/tinyxml2.h"

class JUnitFormatter {

public:
	static JUnitFormatter& getInstance();

	JUnitFormatter(JUnitFormatter const&) = delete;
	void operator=(JUnitFormatter const) = delete;



private:
	JUnitFormatter();


	// Assumes everything is in UTF-8, auto CRLF -> LF
	tinyxml2::XMLDocument* m_document;




};
#endif