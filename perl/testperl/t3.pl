use strict;
use warnings;
use IO::Handle;
IO::Handle->output_record_separator("\n");

my $v1 = ();
my $v2 = undef;
defined($v1) && print "v1 ok\n";
defined($v2) && print "v2 ok\n";
print($v1);
print($v2);
