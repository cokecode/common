#! /usr/bin/perl -w

use strict;
use Getopt::Long;

my ($opt_help, $opt_mod, $opt_file);
Getopt::Long::Configure('bundling','auto_version','auto_help');

## pass_through: keep remains args in @ARGV
##Getopt::Long::Configure('bundling','auto_version','auto_help', 'pass_through');

GetOptions (    "help|h|?"      => \$opt_help,
				"mod|m=i"		=> \$opt_mod,
				"file|f=s"		=> \$opt_file,
            ) || usage();

defined $opt_mod or $opt_mod = 64;
defined $opt_file or $opt_file = "/tmp/1.txt";
print("mod = $opt_mod\n");
print("file = $opt_file\n");

sub usage {
	exit;
}

