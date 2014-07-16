#! /usr/bin/perl -w

use strict;
use Socket;

sub ip_valid {
	my $str = shift;

	# strict checking
	if ($str !~ /^\d+\.\d+\.\d+\.\d+$/) {
		return undef;
	}

	return defined(inet_aton($str));
}

sub iprange_expand {
	my $iprange = shift;

	my ($ip1, $ip2);
	my @result;

	if ($iprange =~ /^(\S+)?~(\S+)$/) {
		my ($begin, $end) = ($1, $2);
		if ($end =~ /^\d+$/) { ## something like '192.168.100.2~10'
			my ($a, $b, $c, $d) = split /\./, $begin;
			$end = "$a.$b.$c.$end";
		}
		$ip1 = $begin;
		$ip2 = $end;
	}
	elsif ($iprange =~ m#^([.\d]+)/(\d+)$#) {
		my ($ippart, $depth) = ($1, $2);
		die "Invalid IP range [ $iprange ].\n" if $depth > 32;

		my $ip_number = inet_aton($ippart) or die "Invalid IP range [ $iprange ].\n";
		$ip_number = unpack 'N', $ip_number;

		my $offset = 32 - $depth;
		my $ip_begin = ($ip_number >> $offset) << $offset;
		my $ip_end = ((($ip_number >> $offset) + 1) << $offset) - 1;

		$ip1 = inet_ntoa(pack 'N', $ip_begin) or die;
		$ip2 = inet_ntoa(pack 'N', $ip_end) or die;
	}
	else {
		## single IP
		$ip1 = $ip2 = $iprange;
	}

	die "Invalid IP.\n" if !ip_valid($ip1);
	die "Invalid IP.\n" if !ip_valid($ip2);

	my $a = unpack 'N', inet_aton($ip1);
	my $b = unpack 'N', inet_aton($ip2);
	die "Invalid IP range, begin($ip1, $a) > end($ip2, $b).\n" if $a > $b;

	#die "IP range too large.\n" if ($b - $a >= 0xffff);

	for (my $n = $a; $n <= $b; $n++) {
		push @result, inet_ntoa(pack 'N', $n);
	}

	@result;
}

#my @iplist = iprange_expand "192.168.200.0/16";

my $iprange = shift;
my @iplist = iprange_expand $iprange;

print join "\n", @iplist;
