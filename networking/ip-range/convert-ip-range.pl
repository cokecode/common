#!/usr/bin/perl -w

use strict;
use warnings;
use Encode;
#use lib qw(perl_module);
#use ip_library;
binmode(STDIN,":utf8");
binmode(STDOUT,":utf8");

## input format:
##   ip-int ip-int
while (my $line = <STDIN>) {
    my ($int1, $int2) = split /\s+/, $line;
    my $ip1 = IPint_to_str($int1);
    my $ip2 = IPint_to_str($int2);
    print "$ip1 $ip2\n";
}

sub IPint_to_str{
	my $lip = shift;
	my @ips;
	if ($lip =~ /\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/){
		return $lip;
	}

	$ips[0] = $lip & 0xff;
	$lip = $lip >> 8;
	$ips[1] = $lip & 0xff;
	$lip = $lip >> 8;
	$ips[2] = $lip & 0xff;
	$lip = $lip >> 8;
	$ips[3] = $lip & 0xff;
	$lip = $lip >> 8;

	@ips = reverse @ips;
	my $ipstr = join(".",@ips);

	return $ipstr;

}

sub IPstr_to_int{
	my $ip = shift;
	my @ips;
	my $res = 0;
	my $i = 0;
	my $l;

	if ($ip =~ /\d+\.\d+\.\d+\.\d+/){
		@ips = split(/\./,$ip);
		foreach $l(reverse @ips){
			$l = $l << 8*$i;
			$res |= $l;
			$i += 1;
		}
	}elsif ($ip =~ /\d+/){
		return $ip;
	}else {
		return -1;
	}

	return $res;
}

