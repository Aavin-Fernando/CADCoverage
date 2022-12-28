#include "JUnitFormat.h"

JUnitFormatter::JUnitFormatter() {
	m_document = new tinyxml2::XMLDocument();
}

JUnitFormatter& JUnitFormatter::getInstance() {
	static JUnitFormatter ist;
	return ist;
}