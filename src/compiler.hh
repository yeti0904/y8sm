#ifndef Y8SM_COMPILER_HH
#define Y8SM_COMPILER_HH

#include "_components.hh"
#include "lexer.hh"

namespace Compiler {
	std::unordered_map <std::string, uint16_t>  GenerateLabels(
		std::string fname, const std::vector <Lexer::Token>& tokens
	);
	
	uint8_t               StringToOpcode(std::string str);
	uint8_t               StringToRegister(std::string str);
	size_t                RegSize(uint8_t reg);
	std::vector <uint8_t> Compile(
		std::string fname, const std::vector <Lexer::Token>& tokens
	);
}

#endif