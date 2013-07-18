#! /usr/bin/perl -w

use strict;
use Socket;

#my $str = '192.168.200.0/24';
my $str = shift;
my $ip;
my $depth;

if ($str =~ m#^([.\d]+)/(\d+)$#) {
	($ip, $depth) = ($1, $2);
	print("$ip, $depth\n");

	my $ip_number = inet_aton($ip) or die;
	$ip_number = unpack 'N', $ip_number;
	print "$ip_number\n";

	my $offset = 32 - $depth;
	my $ip_begin = ($ip_number >> $offset) << $offset;
	my $ip_end = ((($ip_number >> $offset) + 1) << $offset) - 1;
	printf "%08X, %08X\n", $ip_begin, $ip_end;

	print inet_ntoa(pack 'N', $ip_begin), ", ", inet_ntoa(pack 'N', $ip_end), "\n";
}


