#!/usr/bin/perl


#D0    B15
#D1    B0
#D2    B1
#D3    B2
#D4    B14
#D5    B13

# todo for version 1.2 : clear B12


use strict;


foreach my $col (0..63)
{
	my $word;

	if($col & 1<<0)
	{
		$word |= 1<<15
	}
	else
	{
		$word |= 1<<(15+16)
	}

	if($col & 1<<1)
	{
		$word |= 1<<0
	}
	else
	{
		$word |= 1<<16;
	}

	if($col & 1<<2)
	{
		$word |= 1<<1
	}
	else
	{
		$word |= 1<<17;
	}

	if($col & 1<<3)
	{
		$word |= 1<<2
	}
	else
	{
		$word |= 1<<18;
	}

	if($col & 1<<4)
	{
		$word |= 1<<14
	}
	else
	{
		$word |= 1<<(14+16);
	}

	if($col & 1<<5)
	{
		$word |= 1<<13
	}
	else
	{
		$word |= 1<<(13+16);
	}

	print "$word,";


}
