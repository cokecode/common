#!/usr/bin/perl -w

use strict;
use warnings;
use utf8;

use Carp;
use Data::Dumper;

use v5.10;

# test ref

my $v1 = "something";
my $v2 = [1, 2, 3];
my $v3 = { a => 1, b => 2};

my $v4 = \$v1;

say "v1: ", ref $v1;
say "v2: ", ref $v2;
say "v3: ", ref $v3;
say "v4: ", ref $v4;

my $v5 = Data::Dumper->new( [ ] );
my $v6 = \$v5;
my $v7 = \$v6;
my $v8 = ref $v5;
say "v5: ", ref $v5;
say "v5: true" if ref $v5;
say "v6: ", ref $v6;
say "v7: ", ref $v7;
say "v8: ", ref $v8;

my $v9 = \*STDOUT;
say "STDOU: ", ref (*STDOUT);
say "v9: ", ref $v9;
