#! /usr/bin/perl -w

use lib "BerkeleyDB-0.39";
use BerkeleyDB;

$login = 'root';
@info  = ( 0, 0, 'SuperUser', '/root', '/bin/tcsh' );

$db = new BerkeleyDB::Hash( -Filename => 'data.bdb',
                            -Flags => DB_CREATE ) or die "Cannot open file: $!";

$db->db_put( $login, join(':', @info) );

$db->db_get( $login, $val );

( $uid, $gid, $name, $home, $shell ) = split ':', $val;

print "$login:\t$uid|$gid $name \t$home\t$shell\n";

$db->db_close();
