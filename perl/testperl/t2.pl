use strict;
use warnings;
use IO::Handle;
IO::Handle->output_record_separator("\n");

sub f1 { }
sub f2 { return (); }
sub f3 { return; }
sub f4 { return undef; }

my %h1 = ( 'a' => f1() );
my %h2 = ( 'a' => f2() );
my %h3 = ( 'a' => f3() );
my %h4 = ( 'a' => f4() );
my %h5 = ( 'a' => undef );
my %h6 = ( 'a' => );

my $v1 = ();
my @v2 = ();
print $v1;
my $v3 = $v1;
print $v3;
print @v2;
print("defined v1") if defined($v1);
