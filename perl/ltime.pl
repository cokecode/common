#! /usr/bin/perl -w

use strict;
use POSIX qw(mktime);

if (@ARGV > 0 and "$ARGV[0]" =~ /--help|-h/) {
        print("Examples,\n");
        print("   $0 1265679193 1265679195\n");
        print("   $0 1265679193\n");
        print("   $0 '2010-02-09 09:33:10'\n");
        exit 0;
}

push @ARGV, time if @ARGV == 0;

foreach my $ts (@ARGV) {
        if ($ts =~ /^(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d):(\d\d)$/) {
                my ($year, $mon, $mday, $hour, $min, $sec) = ($1, $2, $3, $4, $5, $6);
                my $ts = mktime($sec, $min, $hour, $mday, $mon - 1, $year - 1900);
                print_time($ts);
                next;
        }

        if (length $ts > 10) {
                $ts /= 1000000;
        }

        print_time($ts);
}

sub print_time {
        my ($ts) = @_;
    my ($sec,$min,$hour,$mday,$mon,$year);

    ($sec,$min,$hour,$mday,$mon,$year) = localtime($ts);
    printf("[%10d] %4d-%02d-%02d %02d:%02d:%02d %s\n",
        $ts, 1900 + $year, 1 + $mon, $mday, $hour, $min, $sec, "CST");

    ($sec,$min,$hour,$mday,$mon,$year) = gmtime($ts);
    printf("[%10d] %4d-%02d-%02d %02d:%02d:%02d %s\n",
        $ts, 1900 + $year, 1 + $mon, $mday, $hour, $min, $sec, "GMT");
}


