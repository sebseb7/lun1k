#!/usr/bin/perl

use strict;




`dfu-util -a 0 -s 0x08004000:0x4000 -U eeprom_data_1`;
`dfu-util -a 0 -s 0x08008000:0x4000 -U eeprom_data_2`;

open infile1,'eeprom_data_1';
my $data1 = <infile1>;
close infile1;
open infile2,'eeprom_data_2';
my $data2 = <infile2>;
close infile2;

unlink 'eeprom_data_1';
unlink 'eeprom_data_2';

open outfile,'>core/eeprom_data.c_new';

print outfile "#include \"stm32f4xx.h\"\n";
print outfile "\n";
print outfile "static const uint8_t eeprom_page1[16384] __attribute__ ((section (\".eeprom1\"))) __attribute__ ((used)) = {\n";
print outfile "\n";

foreach my $x (0..511)
{
	print outfile "\t";
	foreach my $y (0..31)
	{
		my $byte = substr($data1, $x*32+$y, 1);
		print outfile sprintf("%3i",ord($byte)).",";
	}
	print outfile "\n";
}
print outfile "\n";
print outfile "};\n";
print outfile "\n";
print outfile "static const uint8_t eeprom_page2[16384] __attribute__ ((section (\".eeprom2\"))) __attribute__ ((used)) = {\n";
print outfile "\n";
foreach my $x (0..511)
{
	print outfile "\t";
	foreach my $y (0..31)
	{
		my $byte = substr($data2, $x*32+$y, 1);
		print outfile sprintf("%3i",ord($byte)).",";
	}
	print outfile "\n";
}
print outfile "\n";
print outfile "};\n";

if(`diff core/eeprom_data.c_new core/eeprom_data.c`)
{
	rename 'core/eeprom_data.c', 'core/eeprom_data.c_backup';
	rename 'core/eeprom_data.c_new','core/eeprom_data.c';
}
else
{
	unlink 'core/eeprom_data.c_new';
}

close outfile;

__END__

#include "stm32f4xx.h"

//const uint16_t eeprom[2048] = {
static const uint8_t eeprom_page1[16384] __attribute__ ((section (".eeprom1"))) __attribute__ ((used)) = {

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};

static const uint8_t eeprom_page2[16384] __attribute__ ((section (".eeprom2"))) __attribute__ ((used)) = {

	0xff,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};

