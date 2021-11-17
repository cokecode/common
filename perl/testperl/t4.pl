use strict;
use warnings;
use IO::Handle;
IO::Handle->output_record_separator("\n");

sub f0 { }

sub f1 {
	print "arg length:", scalar(@_);
	my ($arg1, $arg2) = @_;
	print("arg1:", defined($arg1)? "defined" : "undefined");
	print($arg1);
	print("arg2:", defined($arg2)? "defined" : "undefined");
	print "return.";
}

sub f2 {
	print "arg length:", scalar(@_);
	my ($arg1, $arg2) = @_;
	print "arg1:", scalar(@$arg1);
	print "return.";
}

f1(f0(), "abc");
f2([f0()], "abc");
