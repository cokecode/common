#!/usr/bin/perl

use strict;
use warnings;
use utf8;

use Carp;
use Data::Dumper;

use v5.10;

my %foo = ("aaa", "bbb");
say %foo;

$foo{aaa} = "ccc";
say %foo;

my $r = \%foo;
$r->{aaa} = "ddd";
say %foo;
