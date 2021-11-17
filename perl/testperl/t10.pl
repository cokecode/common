#!/usr/bin/perl

use strict;
use warnings;
use utf8;
use v5.10;

use Carp;
use Data::Dumper;

my @a = ("a", "b");

push @a, (1, 2, 3);
say Dumper @a;		# 1-dimension array

@a = ("a", "b");
push @a, \(1, 2, 3);
say Dumper @a;		# 1-dimension array, but: (..., \1, \2, \3)

@a = ("a", "b");
push @a, [1, 2, 3];
say Dumper @a;		# 2-dimension array
