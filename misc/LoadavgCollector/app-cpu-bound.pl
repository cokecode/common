#! /usr/bin/perl

if (@ARGV > 0) {
	$nproc = shift;
}
else {
	$nproc = 1;
}

foreach (1 .. $nproc) {
	if (fork == 0) {
		for (;;) {
			rand();		# in user-space
			time();		# in kernel-space
		}
		exit 0;
	}
}

while (wait > 0) {}
