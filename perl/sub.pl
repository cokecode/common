#! /usr/bin/perl -w

my $a = 'a: hello';
my $b = 'b: world';
my $c = 'c: perl!';

print("$a, $b, $c\n");
foo($a, $b, $c);
print("$a, $b, $c\n");

sub foo {
	print("$_[0], $_[1], $_[2]\n");
	$_[0] = 'A: HELLO';
	print("$_[0], $_[1], $_[2]\n");
}


my @array = ($a, $b, $c);
print join(' | ', @array), "\n";
bar(@array);
print join(' | ', @array), "\n";

sub bar {
	shift @_;
}

