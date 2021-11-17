#!/usr/bin/perl

use strict;
use warnings;
use utf8;
use v5.10;

use Carp;
use Data::Dumper;

sub var_in_package {
	my $varname = shift;
	return "$varname in package? " . ((exists ${main::}{$varname})? "yes" : "no");
}

say var_in_package("v1");	# no

our $v2;
say var_in_package("v2");	# yes

my $v3;
say var_in_package("v3");	# no (note here, not exist in package!!)

$::v4 = 1;					# warn: Name "main::v4" used only once: possible typo at ./t12.pl line 24.
say var_in_package("v4");	# yes

$::v5;						# warn: Useless use of a variable in void context at ./t12.pl line 27;
							# warn: Name "main::v5" used only once: possible typo at ./t12.pl line 27.
say var_in_package("v5");	# yes

$Foobar::v6;				# warn: Useless use of a variable in void context at ./t12.pl line 31.
say var_in_package("v6");	# no
say(exists ${Foobar::}{v6}? "true" : "false");	# true

say(exists ${Foobar::}{v7}? "true" : "false");	# false

say(exists ${Leslie::}{v8}? "true" : "false");	# false

# my $::v9 = 1;
# "my" variable $::v9 can't be in a package at ./t12.pl line 39, near "my $::v9 "
# Can't use global $::v9 in "my" at ./t12.pl line 39, near "my $::v9 "
# Execution of ./t12.pl aborted due to compilation errors.
#
