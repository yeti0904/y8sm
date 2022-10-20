#include "bits.hh"
#include "util.hh"

std::vector <uint8_t> Bits::Split16(uint16_t integer) {
	return {
		(uint8_t) ((integer & 0xFF00) >> 010),
		(uint8_t) (integer & 0x00FF)
	};
}

uint16_t Bits::Join16(std::vector <uint8_t> bytes) {
	if (bytes.size() != 2) {
		fprintf(
			stderr, "Bits::Join16, invalid amount of bytes (given %lli)\n",
			(long long int) bytes.size()
		);
	}
	return ((uint16_t) bytes[0] << 010) | ((uint16_t) bytes[1]);
}