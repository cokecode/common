#!/usr/bin/perl -w

use strict;
use warnings;
use utf8;

use Carp;
use Data::Dumper;

use v5.10;

my %hash = (1, 2, 3, 4);
my $refhash = \%hash;

say $refhash->{1};
say ${refhash}->{1};

#say {$refhash}->{1};			# syntax error
#say( ({$refhash})->{1} );		# syntax error

say %{$refhash}{1};				# output "12", why?
#say( (%{$refhash}){1} );		# syntax error
say %{$refhash};

say( ($refhash)->{1} );			# OK
