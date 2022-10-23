#include "compiler.hh"
#include "constants.hh"
#include "bits.hh"
#include "error.hh"

std::unordered_map <std::string, uint16_t> Compiler::GenerateLabels(
	std::string fname, const std::vector <Lexer::Token>& tokens
) {
	std::unordered_map <std::string, uint16_t> ret;
	uint16_t                                   size = 0;
	
	for (size_t i = 0; i < tokens.size(); ++i) {
		switch (tokens[i].type) {
			case Lexer::TokenType::Opcode: {
				auto instruction = Compiler::StringToOpcode(tokens[i].contents);
				switch (instruction) {
					case YETI8_INSTRUCTION_NOP:
					case YETI8_INSTRUCTION_HLT: {
						size += 1;
						break;
					}
					case YETI8_INSTRUCTION_STO:
					case YETI8_INSTRUCTION_LD: {
						size += 4;
						break;
					}
					case YETI8_INSTRUCTION_RCP:
					case YETI8_INSTRUCTION_CMP:
					case YETI8_INSTRUCTION_ADD:
					case YETI8_INSTRUCTION_SUB:
					case YETI8_INSTRUCTION_MUL:
					case YETI8_INSTRUCTION_DIV:
					case YETI8_INSTRUCTION_AND:
					case YETI8_INSTRUCTION_OR:
					case YETI8_INSTRUCTION_XOR:
					case YETI8_INSTRUCTION_LSH:
					case YETI8_INSTRUCTION_RSH: {
						size += 3;
						break;
					}
					case YETI8_INSTRUCTION_NOT: {
						size += 2;
						break;
					}
					case YETI8_INSTRUCTION_MOV:
					case YETI8_INSTRUCTION_MVI: {
						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						auto reg     = Compiler::StringToRegister(tokens[i].contents);
						auto regSize = Compiler::RegSize(reg);

						size += 2 + regSize;
						break;
					}
					default: {
						fprintf(
							stderr, "[ERROR] %s:%lli: Unrecognised instruction %i\n",
							fname.c_str(), (long long int) tokens[i].line, instruction
						);
						exit(EXIT_FAILURE);
					}
				}
				break;
			}
			case Lexer::TokenType::Label: {
				fprintf(stderr, "warning: labels seem to cause undesired effects in yeti-8\n");
				ret[tokens[i].contents] = size;
				break;
			}
			default: break;
		}
	}

	return ret;
}

uint8_t Compiler::StringToOpcode(std::string str) {
	if (str == "nop") {
		return YETI8_INSTRUCTION_NOP;
	}
	else if (str == "sto") {
		return YETI8_INSTRUCTION_STO;
	}
	else if (str == "ld") {
		return YETI8_INSTRUCTION_LD;
	}
	else if (str == "rcp") {
		return YETI8_INSTRUCTION_RCP;
	}
	else if (str == "mov") {
		return YETI8_INSTRUCTION_MOV;
	}
	else if (str == "mvi") {
		return YETI8_INSTRUCTION_MVI;
	}
	else if (str == "cmp") {
		return YETI8_INSTRUCTION_CMP;
	}
	else if (str == "add") {
		return YETI8_INSTRUCTION_ADD;
	}
	else if (str == "sub") {
		return YETI8_INSTRUCTION_SUB;
	}
	else if (str == "mul") {
		return YETI8_INSTRUCTION_MUL;
	}
	else if (str == "div") {
		return YETI8_INSTRUCTION_DIV;
	}
	else if (str == "not") {
		return YETI8_INSTRUCTION_NOT;
	}
	else if (str == "push") {
		return YETI8_INSTRUCTION_PUSH;
	}
	else if (str == "pop") {
		return YETI8_INSTRUCTION_POP;
	}
	else if (str == "and") {
		return YETI8_INSTRUCTION_AND;
	}
	else if (str == "or") {
		return YETI8_INSTRUCTION_OR;
	}
	else if (str == "xor") {
		return YETI8_INSTRUCTION_XOR;
	}
	else if (str == "lsh") {
		return YETI8_INSTRUCTION_LSH;
	}
	else if (str == "rsh") {
		return YETI8_INSTRUCTION_RSH;
	}
	else if (str == "hlt") {
		return YETI8_INSTRUCTION_HLT;
	}

	fprintf(stderr, "Unrecognised instruction %s\n", str.c_str());
	exit(EXIT_FAILURE);
}

uint8_t Compiler::StringToRegister(std::string str) {
	if (str == "ac") {
		return YETI8_REGISTER_AC;
	}
	else if (str == "sp") {
		return YETI8_REGISTER_SP;
	}
	else if (str == "pc") {
		return YETI8_REGISTER_PC;
	}
	else if (str == "hl") {
		return YETI8_REGISTER_HL;
	}
	else if (str == "r1") {
		return YETI8_REGISTER_R1;
	}
	else if (str == "r2") {
		return YETI8_REGISTER_R2;
	}
	else if (str == "r3") {
		return YETI8_REGISTER_R3;
	}
	else if (str == "r4") {
		return YETI8_REGISTER_R4;
	}
	else if (str == "r5") {
		return YETI8_REGISTER_R5;
	}
	else if (str == "r6") {
		return YETI8_REGISTER_R6;
	}
	else if (str == "r7") {
		return YETI8_REGISTER_R7;
	}
	else if (str == "r8") {
		return YETI8_REGISTER_R8;
	}
	else if (str == "r9") {
		return YETI8_REGISTER_R9;
	}

	fprintf(stderr, "[ERROR] Unrecognised register %s\n", str.c_str());
	exit(EXIT_FAILURE);
}

size_t Compiler::RegSize(uint8_t reg) {
	switch (reg) {
		case YETI8_REGISTER_SP:
		case YETI8_REGISTER_PC:
		case YETI8_REGISTER_HL: {
			return 2;
		}
		default: return 1;
	}
}

std::vector <uint8_t> Compiler::Compile(
	std::string fname, const std::vector <Lexer::Token>& tokens
) {
	std::vector <uint8_t> ret;


	auto labels = Compiler::GenerateLabels(fname, tokens);

	for (size_t i = 0; i < tokens.size(); ++i) {
		switch (tokens[i].type) {
			case Lexer::TokenType::Label:
			case Lexer::TokenType::End: break;
			case Lexer::TokenType::Opcode: {
				Lexer::Token opcode = tokens[i];
				uint8_t      opcodeByte = Compiler::StringToOpcode(opcode.contents);

				/*bool found = false;
				for (auto& size : lineSizes) {
					if (size.first == tokens[i].line) {
						found              = true;
						currentBinarySize += size.second;
					}
				}

				if (!found) {
					fprintf(
						stderr,
						"[ERROR] %s:%lli Size of line was not calculated\nThis is a bug\n",
						fname.c_str(), (long long int) tokens[i].line
					);
					exit(EXIT_FAILURE);
				}*/

				ret.push_back(opcodeByte);
				switch (opcodeByte) {
					case YETI8_INSTRUCTION_NOP:
					case YETI8_INSTRUCTION_HLT: {
						continue;
					}
					case YETI8_INSTRUCTION_STO:
					case YETI8_INSTRUCTION_LD: {
						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						uint8_t reg = Compiler::StringToRegister(tokens[i].contents);
						ret.push_back(reg);
						
						++ i;
						if (tokens[i].type != Lexer::TokenType::IntegerParameter) {
							Errors::Expected(
								"integer", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						
						for (auto& byte : Bits::Split16(stoi(tokens[i].contents))) {
							ret.push_back(byte);
						}
						break;
					}
					case YETI8_INSTRUCTION_RCP:
					case YETI8_INSTRUCTION_CMP:
					case YETI8_INSTRUCTION_ADD:
					case YETI8_INSTRUCTION_SUB:
					case YETI8_INSTRUCTION_MUL:
					case YETI8_INSTRUCTION_DIV:
					case YETI8_INSTRUCTION_AND:
					case YETI8_INSTRUCTION_OR:
					case YETI8_INSTRUCTION_XOR:
					case YETI8_INSTRUCTION_LSH:
					case YETI8_INSTRUCTION_RSH: {
						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						uint8_t reg = Compiler::StringToRegister(tokens[i].contents);
						ret.push_back(reg);

						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						reg = Compiler::StringToRegister(tokens[i].contents);
						ret.push_back(reg);
						break;
					}
					case YETI8_INSTRUCTION_MOV:
					case YETI8_INSTRUCTION_MVI: {
						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						uint8_t reg = Compiler::StringToRegister(tokens[i].contents);
						ret.push_back(reg);
						
						++ i;
						if (tokens[i].type != Lexer::TokenType::IntegerParameter) {
							if (tokens[i].type != Lexer::TokenType::LabelParameter) {
								Errors::Expected(
									"integer", fname, tokens[i].line,
									Lexer::TokenTypeAsString(tokens[i].type)
								);
							}

							if (!labels.count(tokens[i].contents)) {
								fprintf(
									stderr, "[ERROR] %s:%lli: No such label: %s\n",
									fname.c_str(), (long long int) tokens[i].line,
									tokens[i].contents.c_str()
								);
								exit(EXIT_FAILURE);
							}
							if (Compiler::RegSize(reg) != 2) {
								fprintf(
									stderr, "[ERROR] %s:%lli: can't put address in this register\n",
									fname.c_str(), (long long int) tokens[i].line
								);
								exit(EXIT_FAILURE);
							}

							uint16_t labelPointer = labels[tokens[i].contents];
							for (auto& byte : Bits::Split16(labelPointer)) {
								ret.push_back(byte);
							}
							
							continue;
						}

						if (Compiler::RegSize(reg) == 2) {
							for (auto& byte : Bits::Split16(stoi(tokens[i].contents))) {
								ret.push_back(byte);
							}
						}
						else {
							ret.push_back(std::stoi(tokens[i].contents));
						}
						break;
					}
					case YETI8_INSTRUCTION_NOT:
					case YETI8_INSTRUCTION_PUSH:
					case YETI8_INSTRUCTION_POP: {
						++ i;
						if (tokens[i].type != Lexer::TokenType::RegisterParameter) {
							Errors::Expected(
								"register", fname, tokens[i].line,
								Lexer::TokenTypeAsString(tokens[i].type)
							);
						}
						uint8_t reg = Compiler::StringToRegister(tokens[i].contents);
						ret.push_back(reg);
						break;
					}
				}
				break;
			}
			default: {
				fprintf(
					stderr, "[ERROR] %s:%lli: unexpected token: %s\n",
					fname.c_str(), (long long int) tokens[i].line,
					Lexer::TokenTypeAsString(tokens[i].type).c_str()
				);
				exit(EXIT_FAILURE);
			}
		}
	}

	return ret;
}