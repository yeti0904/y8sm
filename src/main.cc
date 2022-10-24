#include "_components.hh"
#include "lexer.hh"
#include "fs.hh"
#include "compiler.hh"

int main(int argc, char** argv) {
	std::string srcFile;
	std::string out = "out.bin";
	bool        debug = false;

	if (argc < 2) {
		printf("Usage: %s [file/options]\n", argv[0]);
		return 0;
	}
	else for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg[0] == '-') {
			if ((arg == "-o") || (arg == "--output")) {
				if (i == argc - 1) {
					fprintf(stderr, "[ERROR] -o / --output expects 1 parameter\n");
					return 1;
				}

				++ i;
				out = argv[i];
			}
			else if ((arg == "-d") || (arg == "--debug")) {
				debug = true;
			}
		}
		else {
			srcFile = arg;
		}
	}

	auto tokens = Lexer::Lex(srcFile, FS::File::Read(srcFile));

	if (debug) {
		Lexer::DumpTokens(tokens);
	}
	
	auto bin    = Compiler::Compile(srcFile, tokens);

	FS::File::Binary::Write(out, bin);

	return 0;
}