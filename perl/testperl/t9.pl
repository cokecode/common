#!/usr/bin/perl

use strict;
use warnings;
use utf8;
use v5.10;

use Text::Tabs;

sub expand {
	say "my expand()";
} # warns

expand();	# successfully
say "OK";
