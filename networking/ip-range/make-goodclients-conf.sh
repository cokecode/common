#! /bin/bash
# zhangxun@taobao.com
# 2013.06

IPDATA_URL="http://ip.taobao.org:9999/ipdata/ipdata_code_with_maxmind.txt.utf8.tar.gz"

function filter_by_area()
{
    local global_ip_data="$1"
    local idname="$2"
    local filterstr="$3"
    local level=$4

    local subdir="$subdirprefix-$idname"
    [ -d $subdir ] || mkdir $subdir

    echo "==> Generate IP/netmask for $idname ..."

    cat $global_ip_data | grep -E "$filterstr" | awk -F, '{print $1, $2}' |
        ./convert-ip-range.pl > $subdir/iprange.txt &&
    ./merge-ip-range < $subdir/iprange.txt > $subdir/iprange-merged.txt &&
    ./analyze-ip-range < $subdir/iprange-merged.txt |
        awk '{print $1, "'$level'"}' > $subdir/goodclients-ip-$idname.`date +%Y%m%d`.conf

    if [ $? -ne 0 ]; then
        echo "($idname) Failed."
        return $?
    fi
}

function filter_for_usa()
{
    local global_ip_data="$1"
    local idname="$2"
    local filterstr="$3"
    local level=$4

    local subdir="IPDATA-$idname"
    [ -d $subdir ] || mkdir $subdir

    echo "==> Generate IP/netmask for $idname ..."

    cat $global_ip_data | grep -E "$filterstr" | awk -F, '{print $1, $2}' |
        ./convert-ip-range.pl > $subdir/iprange.txt &&
	perl -alne '($a, $b) = @F[0,1]; $a =~ s/\d+\.\d+$/0.0/; $b =~ s/\d+\.\d+$/255.255/; print "$a $b"' < $subdir/iprange.txt | uniq  > $subdir/iprange-simplified.txt 
    ./merge-ip-range < $subdir/iprange-simplified.txt > $subdir/iprange-merged.txt &&
    ./analyze-ip-range < $subdir/iprange-merged.txt |
        awk '{print $1, "'$level'"}' > $subdir/goodclients-ip-$idname.`date +%Y%m%d`.conf

    if [ $? -ne 0 ]; then
        echo "($idname) Failed."
        return $?
    fi
}

function mainproc()
{
	local ipdata="$1"
	local subdir="$2"

	if [ ! -f "$ipdata" ]; then
		echo "ERROR: $ipdata not exist."
		exit 1
	fi

	subdirprefix=$subdir/IPDATA

	filter_by_area $ipdata CN ",CN," 0 || exit 1
	filter_by_area $ipdata HK ",HK," 0 || exit 1
	filter_by_area $ipdata MO ",MO," 0 || exit 1
	filter_by_area $ipdata TW ",TW," 0 || exit 1
	filter_by_area $ipdata SG ",SG," 0 || exit 1
	filter_by_area $ipdata MY ",MY," 0 || exit 1

	filter_for_usa $ipdata US ",US," 1 || exit 1
	filter_by_area $ipdata AU ",AU," 1 || exit 1

	filter_by_area $ipdata JP ",JP," 2 || exit 1
	filter_by_area $ipdata KR ",KR," 2 || exit 1

	ls -lrt $subdir/IPDATA-*/goodclients-ip-*.conf

	wc -l $subdir/IPDATA-*/goodclients-ip-*.conf

	mkdir $subdir/results
	cp $subdir/IPDATA-*/goodclients-ip-*.conf $subdir/results

	echo
	echo "Result: $subdir/results/goodclients-*.conf"
	echo
}

################################################################################
## main entry

if [ $# -lt 1 ]; then
	echo "Usage: $0 [ ipdata_code_with_maxmind.txt.utf8 | download-and-build ]"
	exit 1
fi

case $1 in
	download-and-build)
		set -x
		filename_tgz=`basename $IPDATA_URL`
		filename=`basename $IPDATA_URL .tar.gz`
		subdir=`date +%F`
		mkdir $subdir 2>/dev/null
		cd $subdir || exit 1
		rm -f $filename_tgz $filename
		wget $IPDATA_URL || exit 2
		tar zxf $filename_tgz || exit 3
		cd -
		mainproc $subdir/$filename $subdir
		;;
	*)
		ipdata=$1
		if [ ! -f $ipdata ]; then
			echo >&2 "IPdata not exists."
			exit 1
		fi

		mainproc $ipdata .
		;;
esac

