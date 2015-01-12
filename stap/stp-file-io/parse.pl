#!/usr/local/bin/perl -w
# parse the output of monitor_file_rw.stp
# feiman.wxg 2013-05

use strict;
use FileHandle;
use File::Basename;
use Data::Dumper;

my $index = 0;
my $openfile = {};
my $device = {};
my $not_recognized_files = "";
my $interval = 10;

sub list_open_files
{
	my $openfileFd =  new FileHandle("sudo lsof |");
	
	while (<$openfileFd>)
	{
		chomp;
		my @elements = split /\s+/, $_;

		#my $num = @elements;
		#print "num $num\n";
		next if (@elements != 9);
		next if ($elements[4] !~ m/(DIR|REG)/);
		$openfile->{"$elements[5]+$elements[7]"}->{"$elements[8]"}++;

	}
	close($openfileFd);
}
sub deal_sigle_file
{
	my $line = shift @_;
	#print "$line \n";
	my @elements = split /\s+/, $line;
	my $dev;

	if ( ! exists $device->{$elements[0]})
	{
		$dev = `stat /dev/$elements[0] | awk 'NR==3{print \$NF}'`;
		#print "dev $dev\n";
		chomp($dev);
		$device->{$elements[0]} = $dev; 
	}
	else
	{
		$dev = $device->{$elements[0]};
	}

	my $key = "$dev+$elements[1]";
	#print "key $key\n";

	if (! exists $openfile->{$key})
	{
		#print "[Not recognized] Dev:/dev/$elements[0] inode:$elements[1]\n";
		my $speed = int($elements[3] / ($interval*1024));
		$not_recognized_files = "[Not recognized] Dev:/dev/$elements[0]  inode:$elements[1]   Speed:$speed KB/s\n$not_recognized_files";
		return;
	}
	foreach my $file (keys %{$openfile->{$key}})
	{
		my $rw = $elements[2] == 0? "READ" : "WRITE";
		my $speed = int($elements[3] / ($interval*1024));
		print "File:$file [$rw]  Speed: $speed KB/s\n";
	}

}

#### main

my $cur_dir = dirname("$0");
my $stapresultFd = new FileHandle("sudo stap $cur_dir/monitor_file_rw.stp |"); 

while ( 1 )
{
	my $str = <$stapresultFd>;
	chomp($str);

	# a new output from monitor_file_rw.stp
	if ($str =~ m/^monitor_file_rw\.stp start$/)
	{
		#$interval = $1;
		# list the linux's opened files
		$openfile = {};
		$not_recognized_files = "";
		list_open_files();
		#print Dumper($openfile);
	
		sleep 1;
		# deal with the output
		my $cur_date = `date`;
		chomp($cur_date);
		print "$cur_date  ########## Interval: $interval\n\n";
		while ( 1 )
		{
			my $line = <$stapresultFd>;
			chomp($line);
			if ($line =~ m/^monitor_file_rw\.stp end$/)
			{
				print "\n";
				print $not_recognized_files;

				print "\n\n";
				last;
			}
			deal_sigle_file($line);
		}
	}
	else
	{
		print "Error, should not reach here\n";
	}
	
}
