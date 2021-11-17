#!/usr/bin/perl -w
# A joyful script - print random characters to screen.

my @CHARS = split //, '~!@#$%^&*-_+=,.<>/?[]{}\|';
my $LEN = scalar(@CHARS);

while (1) {
	my $n = int(rand() * 80);
	my $chidx = 1 + int(rand() * ($LEN - 1));
	my $ch = $CHARS[$chidx];
	print $ch x $n, "\n";

	# sleep some milliseconds, don't flush the screen
	select undef, undef, undef, (0.1 + rand());
}

