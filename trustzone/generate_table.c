/*
 * Copyright (C) 2014 Humberto Naves <hsnaves@gmail.com>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <stdio.h>

void emit(FILE *fp, unsigned int addr, unsigned int flags)
{
	unsigned int val = (addr << 20) | flags;
	fprintf(fp, "\t.word\t0x%x\n", val);
}

int main(int argc, char **argv)
{
	FILE *fp;
	unsigned int i;

	fp = fopen("tzsw_table.S", "w");
	if (!fp) {
		fprintf(stderr, "can't open file for writing\n");
		return -1;
	}

	fprintf(fp, "/* Translation table\n");
	fprintf(fp, " * Each line represents a section (1 Mb block).\n");
	fprintf(fp, " * Zero entries represent that the section is not mapped.\n");
	fprintf(fp, " * For more details check ARM DDI 0406C.\n");
	fprintf(fp, " * WARNING: This file was auto-generated!\n");
	fprintf(fp, " */\n\n\n");
	fprintf(fp, "translation_table0:\n");
	for(i = 0; i < 0x1000; i++) {
		if (i == 0x20) {
			fprintf(fp, "\t/* PXN=0, B=0, C=0, AP=011, TEX=001, S=1, nG=0, nS=0 */\n");
			fprintf(fp, "\t/* Shareable, Secure, Global, Outer and Inner Non-cacheable, Full access */\n");
			emit(fp, i, 0x11C02);
		} else if (i >= 0x100 && i < 0x200) {
			if (i == 0x100) {
				fprintf(fp, "\t/* PXN=0, B=1, C=0, AP=011, TEX=000, S=1, nG=0, nS=0 */\n");
				fprintf(fp, "\t/* Shareable, Secure, Global, Shareable Device, Full access */\n");
			}
			emit(fp, i, 0x10C06);
		} else if (i >= 0x400) {
			if (i == 0x400) {
				fprintf(fp, "\t/* PXN=0, B=0, C=0, AP=011, TEX=001, S=1, nG=0, nS=0 */\n");
				fprintf(fp, "\t/* Shareable, Secure, Global, Outer and Inner Non-cacheable, Full access */\n");
			}
			emit(fp, i, 0x11C02);
		} else {
			fprintf(fp, "\t.word\t0\n");
		}
	}
	fclose(fp);
	return 0;
}
