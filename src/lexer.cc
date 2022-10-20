#include "lexer.hh"
#include "util.hh"

const std::vector <std::string> registers = {
	"ac", "sp", "pc", "hl",
	"r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9"
};

std::vector <Lexer::Token> Lexer::Lex(std::string fname, std::string code) {
	std::vector <Lexer::Token> ret;
	size_t              line = 1;
	std::string         reading;
	bool                success = true;

	for (size_t i = 0; i <= code.length(); ++i) {
		if (code[i] == '\n') {
			++ line;
		}
		switch (code[i]) {
			case ' ':
			case ',':
			case '\0':
			case '\n': {
				if (reading.empty()) {
					continue;
				}

				if (ret.empty() || (ret.back().type == Lexer::TokenType::End)) {
					// opcode
					ret.push_back({
						Lexer::TokenType::Opcode, reading, line
					});
				}
				else {
					// parameter
					bool isRegister = false;
					for (auto& reg : registers) {
						if (reading == reg) {
							isRegister = true;
							break;
						}
					}

					if (isRegister) {
						// register
						ret.push_back({
							Lexer::TokenType::RegisterParameter, reading, line
						});
					}
					else if (Util::IsNumber(reading)) {
						ret.push_back({
							Lexer::TokenType::IntegerParameter, reading, line
						});
					}
					else {
						fprintf(
							stderr,
							"[ERROR] %s:%lli: can't interpret parameter: '%s'\n",
							fname.c_str(), (long long int) line, reading.c_str()
						);
						success = false;
					}

					if ((code[i] == '\n') || (code[i] == '\0')) {
						ret.push_back({Lexer::TokenType::End, "", line});
					}
				}
				reading = "";
				break;
			}
			default: {
				reading += code[i];
			}
		}
	}

	if (!success) {
		fprintf(stderr, "cannot continue with compilation\n");
		exit(EXIT_FAILURE);
	}

	return ret;
}

std::string Lexer::TokenTypeAsString(TokenType type) {
	switch (type) {
		case Lexer::TokenType::Opcode: {
			return "opcode";
		}
		case Lexer::TokenType::RegisterParameter: {
			return "registerParameter";
		}
		case Lexer::TokenType::IntegerParameter: {
			return "integerParameter";
		}
		case Lexer::TokenType::End: {
			return "end";
		}
	}
}

void Lexer::DumpTokens(const std::vector <Lexer::Token>& tokens) {
	for (size_t i = 0; i < tokens.size(); ++i) {
		std::string type = Lexer::TokenTypeAsString(tokens[i].type);

		printf(
			"(%lli) %s: '%s'\n",
			(long long int) i, type.c_str(), tokens[i].contents.c_str()
		);
	}
}