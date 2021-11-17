#!/usr/bin/perl -w

use strict;
use warnings;
use v5.10;

my $v1 = "hello", "world", "perl";		# compiler warns here
my $v2 = ("hello", "world", "perl");	# compiler warns here
my $v3 = qw/hello world perl/;			# compiler warns here

say "v1: ", $v1;		# "hello"
say "v2: ", $v2;		# "perl"
say "v3: ", $v3;

my @v4 = "hello", "world", "perl";		# compiler warns here
my @v5 = ("hello", "world", "perl");	# OK
my @v6 = qw/hello world perl/;			# OK

say "v4: ", @v4;		# "hello"
say "v5: ", @v5;		# ARRAY
say "v6: ", @v6;		# ARRAY

say "done.";
