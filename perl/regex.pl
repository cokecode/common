#! /usr/bin/perl -w

use strict;

my $string = "Hello, world!";

my $result = $string =~ s/wor/WOR/;
# $result should be: 1 (success)

print $result? "Success" : "Failure", "\n";

$result = $string =~ s/wor/WOR/;
# $result should be: undef (failure)

print $result? "Success" : "Failure", "\n";
print !$result, "\n";

=comment

$result = $string =~ s/WOR/WAR/r;	# Note: s/../../r
# $string is unchanged.
# $result should be: Hello, WARld!

print "$result\n";

=cut

