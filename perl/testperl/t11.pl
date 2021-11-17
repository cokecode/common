#!/usr/bin/perl

use strict;
use warnings;
use utf8;
use v5.10;

use Carp;
use Data::Dumper;

sub f1 { say "f1"; return 0 }
sub f2 { say "f2"; return 2 }
sub f3 { say "f3"; return 2 }

if ( f1 && f2 == f3 ) {
	say "OK"
}
