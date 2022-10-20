#include "error.hh"

void Errors::Expected(std::string expected, std::string fname, size_t line, std::string got) {
	fprintf(
		stderr, "[ERROR] %s:%lli: expected %s, got %s\n",
		fname.c_str(), (long long int) line, expected.c_str(), got.c_str()
	);
	exit(EXIT_FAILURE);
}