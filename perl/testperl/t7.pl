#!/usr/bin/perl

use strict;
use warnings;
use utf8;

use Carp;
use Data::Dumper;
use Scalar::Util qw(isdual);

use v5.10;

my $v1 = 123;
my $v2 = "123";
say $v1 == $v2;		# true
say $v1 eq $v2;		# true

my $v3 = 0123;		# v3 is octal, is equal to 83.
my $v4 = "0123";
say "v3 = $v3";
say "v4 = $v4";
say "v4: ", $v4 + 0;
say $v3 == $v4;		# false
say $v3 eq $v4;		# false
say $v3 == ($v4 + 0);	# false
say "v4 is dual? ", isdual($v4);

my $v5 = 0x123;
my $v6 = "0x123";
say $v5 == $v6;		# warns: Argument "0x123" isn't numeric in numeric eq (==)
say $v5 eq $v6;		# false
say $v5 == $v6 + 0;	# true
