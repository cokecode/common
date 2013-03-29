#! /usr/bin/perl

if (@ARGV > 0) {
	$nproc = shift;
}
else {
	$nproc = 1;
}

foreach $p (1 .. $nproc) {
	if (fork == 0) {
		for ($i = 0; $i < 10; $i++) {
			system("dd if=/dev/zero of=BIGFILE-$p-$i bs=1048576 count=1024");
		}
		exit 0;
	}
}

$SIG{INT} = \&sighdl;
$SIG{TERM} = 'IGNORE';

while (wait > 0) {}

sub sighdl {
	system("killall app-io-bound.pl");
	system("killall dd");
}
