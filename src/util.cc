#include "util.hh"

bool Util::IsNumber(std::string str) {
	try {
		stod(str);
	}
	catch (std::exception&) {
		return false;
	}
	return true;
}