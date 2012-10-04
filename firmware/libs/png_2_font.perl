#!/usr/bin/perl

use strict;
use GD;

my @chars = (' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}');


my $image = GD::Image->newFromPng('16pt.png');
(my $width,my $height) = $image->getBounds();


my %charmap;

my $rowidx=0;
my $rowidx2=0;
foreach my $char(@chars)
{
	my $charwidth=0;
	my $done = 0;
	my @rows;
	$charmap{$char}->{'idx'} = $rowidx2*$height;
	while($done == 0)
	{
		my @row;
		foreach my $y (0..($height-1))
		{
			(my $r,my $g, my $b) = $image->rgb($image->getPixel($rowidx,$y));

			if($b-$r > 100)
			{
				$done = 1;
			}
			$row[$y]=$r;
			#warn "$char $rowidx $done $y";
		}

		push @rows,\@row;

		$rowidx++;

		if($rowidx > $width)
		{
			$done = 1;
		}

		if($done == 0)
		{
			$charwidth++;
			$rowidx2++;
			#warn 'inc';
		}
		else
		{
			#die;
		}


	}

	$charmap{$char}->{'width'} = $charwidth;
	$charmap{$char}->{'arr'} = \@rows;




}


open outfile,">font16pt.c";
print outfile "#include \"font16pt.h\"\n\n";
print outfile 'uint8_t charWidth_16pt[] = {';
print outfile join ',', (map { $charmap{$_}->{'width'} } @chars);
print outfile "};\n";
print outfile 'uint16_t startIdx_16pt[] = {';
print outfile join ',', (map { $charmap{$_}->{'idx'} } @chars);
print outfile "};\n";

print outfile 'uint8_t  charMap_16pt[] = {'."\n";
foreach my $char(@chars)
{
	my $charwidth = $charmap{$char}->{'width'};
	
	#print outfile "// $char\n";
	foreach my $y (0..($height-1))
	{
		foreach my $x (0..($charwidth-1))
		{	
			my $num = $charmap{$char}->{'arr'}->[$x]->[$y];
			$num = 0 if $num < 5;
			$num = 255 if $num > 251;
			$num = 255-$num;	
			print outfile '0x'.sprintf("%02X", $num).",";
		}
		print outfile "\n";
	
	}
	print outfile "\n\n";
}
print outfile "0};\n";
close outfile;
