#! /usr/bin/perl -w

use strict;
use FindBin;

my $vmtouch = "$FindBin::Bin/vmtouch";
my $cmd_vmtouch = "$vmtouch " . join(' ', @ARGV);
open PIPE, "$cmd_vmtouch | tr '\r' '\n' | uniq |" or die;
my $lastfile;
my $vmmap;
my @extra_lines;

print("cmdline: $cmd_vmtouch\n");

while (<PIPE>) {
    chomp;
    next unless $_;
    if (m{^\[.+\]\s+(\d+)/(\d+)}) {
        my ($resident, $total) = ($1, $2);
        #####printf("%8d %8d $lastfile\n", $resident, $total) if $resident > 0;
        if ($resident > 0) {
            $vmmap->{$lastfile}->{resident} = $resident;
            $vmmap->{$lastfile}->{total} = $total;
        }
    }
    elsif (/^\s+/) {
        #####print "$_\n";
        push @extra_lines, $_;
    }
    else {
        $lastfile = $_;
    }
}

## foreach my $filepath (sort keys %$vmmap) {
##     printf("%8d %8d $filepath\n",
##         $vmmap->{$filepath}->{resident},
##         $vmmap->{$filepath}->{total});
## }
my @keys = sort { $vmmap->{$a}->{resident} <=> $vmmap->{$b}->{resident} } keys %$vmmap;
foreach my $filepath (@keys) {
    printf("%8d %8d $filepath\n",
        $vmmap->{$filepath}->{resident},
        $vmmap->{$filepath}->{total});
}
print join "\n", @extra_lines;
print "\n";

