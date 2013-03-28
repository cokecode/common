#!/usr/bin/perl
#*************************************************************************
#*  Copyright (c) 2008 www.taobao.com        ALL RIGHTS RESERVED.        *
#*************************************************************************

=head1 NAME

iSearchResultParser.pl - Dump iSearch result.

=head1 SYNOPSIS

iSearchResultParser.pl --help|h|?  brief help message

iSearchResultParser.pl --man       full documentation

Mail bug reports and suggestions to <guinan@taobao.com>.;

=head1 OPTIONS

=over 8

=item B<-h, --help>

Print a brief help message and exits 1.

=item B<--man>

Prints the manual page and exits.

=back

=head1 DESCRIPTION

B<This program> will read the iSearch result file or curl's stdout, parse it and then dump to stdout. Be careful, since the result is always big, you should redirect it to more or less to view it, or save it to file and then view it in editor.


=head1 Author: Nan Zheng (guinan@taobao.com), www.taobao.com, Inc.

=head1 No known bugs

=head1 $Date:$

=head1 $Revision:$

=pod

detailed description here

=cut

use strict;
use warnings;
#use diagnostics;
use English;
use Getopt::Long;
use Pod::Usage;
#use lib '../perl';

my $PROGNAME = $0;
our $VERSION ='1.0';

my ($opt_help,$opt_man,$opt_conf);
#parse command line options
Getopt::Long::Configure('bundling','auto_version','auto_help');
GetOptions (    "help|h|?"      => \$opt_help,
                "man"           => \$opt_man,
            ) || pod2usage(-verbose=>0);

pod2usage (-verbose => 1) if ($opt_help);
pod2usage (-verbose => 2) if ($opt_man);



#main logic here

my %result;
while (my $line=<>)
{
        my $docs_return=0;
        my @segs=split /\x1/,$line;
        my $index=0;
        #head
        $result{'head'}{'version'}=$segs[$index++];
        $result{'head'}{'query_status'}=$segs[$index++];
        $result{'head'}{'query_time'}=$segs[$index++];
        if ($result{'head'}{'query_status'} ne "OK")
        {
                next;
        }
        my $name= $segs[$index++];
        if ($name eq "const")#const
        {
                my $len=$segs[$index++];
                $result{'const'}{'docs_found'}=$segs[$index++];
                $result{'const'}{'docs_search'}=$segs[$index++];
                $result{'const'}{'docs_restrict'}=$segs[$index++];
                $docs_return=$segs[$index++];
                $result{'const'}{'docs_return'}=$docs_return;
                $result{'const'}{'has_prepage'}=$segs[$index++];
                $result{'const'}{'has_nextpage'}=$segs[$index++];
        }
        while ($index<@segs)
        {
                $name= $segs[$index++];
                if ($name eq "special")#specialKey
                {
                        my $len=$segs[$index++];
                        $result{'special'}{'key'}=$segs[$index++];
                        $result{'special'}{'category'}=$segs[$index++];
                        $result{'special'}{'kind'}=$segs[$index++];
                        my $synonyms_count=$segs[$index++];
                        my $len_of_relation=$segs[$index++];

                        $result{'special'}{'synonyms_count'}=$synonyms_count;
                        $result{'special'}{'len_of_relation'}=$len_of_relation;
                        while ($synonyms_count-- > 0)
                        {
                                my $seg =$segs[$index++];
                                push @{$result{'special'}{'synonyms'}},$seg;
                        }
                        while ($len_of_relation-- >0)   
                        {
                                my $seg =$segs[$index++];
                                push @{$result{'special'}{'relation'}},$seg;
                        }
                }
                elsif($name eq "echo")
                {
                        my $len=$segs[$index++];
                        $result{'echo'}{'do_echo_key'}=$segs[$index++];
                        $result{'echo'}{'echo_keys'}=$segs[$index++];
                        my $num_of_comb=$segs[$index++];
                        $result{'echo'}{'num_of_comb'}=$num_of_comb;
                        while ($num_of_comb-- > 0)
                        {
                                my $seg =$segs[$index++];
                                push @{$result{'echo'}{'combs'}},$seg;
                        }


                }
                elsif($name eq "stat")
                {
                        my $len=$segs[$index++];
                        my $stat_infos_num=$segs[$index++];
                        $result{'stat'}{'num_of_stat_infos'}=$stat_infos_num;
                        my $i=0;
                        while ($i<$stat_infos_num)
                        {
                                my $stat_num =$segs[$index++];
                                while ($stat_num -- >0)
                                {
                                        my $seg = $segs[$index++];
                                        push @{$result{'stat'}{'stat_infos'}[$i]},$seg;
                                }
                                $i++;
                        }
                }
                elsif($name eq "data")
                {
                        my $len=$segs[$index++];
                        my $field_count=$segs[$index++];
                        $result{'data'}{'field_count'}=$field_count;
                        my $i=0;
                        while ($i++<$field_count)
                        {
                                my $field_name =$segs[$index++];
                                push @{$result{'data'}{'field_name'}},$field_name;
                        }
                        $i=0;
                        while ($i<$docs_return)
                        {
                                my $j=0;
                                while($j++<$field_count)
                                {
                                        push @{$result{'data'}{'docs'}[$i]},$segs[$index++];
                                }
                                $i++;
                        }
                }
                else
                {
                        next if ($name eq '');
                }
        }
 
&print_result(\%result);
}

sub print_result
{
        my $ref=shift;
        my %result=%$ref;
        print "--------HEAD--------\n";
        print "Version:".$result{'head'}{'version'}."\tQueryStatus:".$result{'head'}{'query_status'}."\tQueryTime:".$result{'head'}{'query_time'}."\n";
        if (defined $result{'const'})
        {
                print "--------CONST--------\n";
                print "DocsFound:".$result{'const'}{'docs_found'}."\tDocsSearch:".$result{'const'}{'docs_search'}."\tDocsRestrict:".$result{'const'}{'docs_restrict'}."\tDocsReturn:".$result{'const'}{'docs_return'}."\n";
                print "\tHasPrepage:".$result{'const'}{'has_prepage'}."\tHasNextPage:".$result{'const'}{'has_nextpage'}."\n";
        }
        if (defined $result{'special'})
        {
                print "--------SPECIAL--------\n";
                print "Key:".$result{'special'}{'key'}."\tCategory:".$result{'special'}{'category'}."\tKind:".$result{'special'}{'kind'}."\n";
                print $result{'special'}{'synonyms_count'}." Synonyms:";
                foreach my $sym (@{$result{'special'}{'synonyms'}})
                {
                        print "\t".$sym;
                }
                print "\n";
                print $result{'special'}{'len_of_relation'}." Relations:";
                foreach my $sym (@{$result{'special'}{'relations'}})
                {
                        print "\t".$sym;
                }
                print "\n";
        }
        if (defined $result{'echo'})
        {
                print "--------ECHO--------\n";
                print "DoEchoKey:".$result{'echo'}{'do_echo_key'}."\tEchoKeys:".$result{'echo'}{'echo_keys'}."\n";
                print $result{'echo'}{'num_of_comb'}." Combinations:";
                foreach my $com (@{$result{'echo'}{'combs'}})
                {
                        print "\t".$com;
                }
                print "\n";
        }
        if (defined $result{'stat'})
        {
                print "--------STAT--------\n";
                print $result{'stat'}{'num_of_stat_infos'}." Stats:\n";
                foreach my $stat_info (@{$result{'stat'}{'stat_infos'}})
                {
                        foreach my $stat (@{$stat_info})
                        {
                                print $stat."\t";
                        }
                        print "\n\n";
                }

        }
        if (defined $result{'data'})
        {
                print "--------DATA--------\n";
                my $doc_num=@{$result{'data'}{'docs'}};
                print $doc_num." Docs\n";
                print "Field Names:\n";
                foreach my $field_name (@{$result{'data'}{'field_name'}})
                {
                        print "\t".$field_name."\n";
                }
                print "\n";
                my $index=1;
                foreach my $doc (@{$result{'data'}{'docs'}})
                {
                        print "Doc $index:\n";
                        $index++;
                        my $j=0;
                        foreach my $field (@{$doc})
                        {
                                unless (defined $field)
                                {
                                        print "\t$result{'data'}{'field_name'}[$j]:\tNULL\n";
                                }
                                else
                                {
                                        print "\t$result{'data'}{'field_name'}[$j]:\t".$field."\n";
                                }
                                $j++;
                        }
                        print "\n";
                }
        }
}

exit;
