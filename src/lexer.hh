#ifndef Y8SM_LEXER_HH
#define Y8SM_LEXER_HH

#include "_components.hh"

namespace Lexer {
	enum class TokenType {
		Opcode = 0,
		Label,
		RegisterParameter,
		IntegerParameter,
		LabelParameter,
		End
	};
	struct Token {
		TokenType   type;
		std::string contents;
		size_t      line;
	};

	std::vector <Token> Lex(std::string fname, std::string code);
	std::string         TokenTypeAsString(TokenType type);
	void                DumpTokens(const std::vector <Token>& tokens);
}

#endif