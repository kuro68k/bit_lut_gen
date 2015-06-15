// bit_lut_gen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>

int main(int argc, char* argv[])
{
	uint32_t	bits = 0;
	uint8_t		line = 0;


	if ((argc != 2) || (strlen(argv[1]) != 8))
	{
		printf("bit_lut_gen <XXXXXXXX>\n");
		printf("(bit)        76543210\n");
		printf("A = always 0\n");
		printf("B = always 1\n");
		return -1;
	}

	for (uint8_t i = 0; i < 8; i++)
	{
		// A = always 0
		if ((argv[1][i] == 'A') || (argv[1][i] == 'a'))
		{
			bits |= 0xA << ((7 - i) * 4);
			continue;
		}

		// B = always 1
		if ((argv[1][i] == 'B') || (argv[1][i] == 'b'))
		{
			bits |= 0xB << ((7 - i) * 4);
			continue;
		}

		if ((argv[1][i] < '0') || (argv[1][i] > '9'))
		{
			printf("Bad number.\n");
			return -1;
		}
		bits |= (argv[1][i] - '0') << ((7-i) * 4);
	}
	printf("// Bit positions: %08X\n", bits);
	printf("// (bit)          76543210\n");
	printf("\n");



	printf("const __flash uint8_t bit_translation_lut[256] = {\n");

	for (uint16_t i = 0; i < 256; i++)
	{
		uint8_t input = i & 0xFF;
		uint8_t	output = 0;
		for (uint8_t b = 0; b < 8; b++)
		{
			if (((bits >> (b * 4)) & 0xF) == 0xA)	// always 0
				continue;

			if (((bits >> (b * 4)) & 0xF) == 0xB)	// always 1
			{
				output |= (1 << b);
				continue;
			}

			uint8_t mask = 1 << ((bits >> (b * 4)) & 0xF);
			if (input &  mask)
				output |= (1 << b);
		}
		printf("0x%02X,", output);
		
		line++;
		if (line > 15)
		{
			printf("\n");
			line = 0;
		}
	}
	printf("};\n");

	return 0;
}

