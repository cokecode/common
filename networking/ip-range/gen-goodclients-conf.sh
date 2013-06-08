#! /bin/bash
# zhangxun@taobao.com
# 2013.06

function filter_by_area()
{
    local global_ip_data="$1"
    local idname="$2"
    local filterstr="$3"

    local subdir="IPDATA-$idname"
    [ -d $subdir ] || mkdir $subdir

    echo "==> Generate IP/netmask for $idname ..."

    cat $global_ip_data | grep -E "$filterstr" | awk -F, '{print $1, $2}' |
        ./convert-ip-range.pl > $subdir/iprange.txt &&
    ./merge-ip-range < $subdir/iprange.txt > $subdir/iprange-merged.txt &&
    ./analyze-ip-range < $subdir/iprange-merged.txt |
        awk '{print $1, "gray"}' > $subdir/goodclients-ip-$idname.`date +%Y%m%d`.conf

    if [ $? -ne 0 ]; then
        echo "($idname) Failed."
        return $?
    fi
}

ipdata="ipdata_code_with_maxmind.txt.utf8"

if [ ! -f $ipdata ]; then
    echo "ERROR: $ipdata not exist."
    exit 1
fi

#filter_by_area $ipdata CN ",CN," || exit 1
filter_by_area $ipdata HK ",HK," || exit 1
#filter_by_area $ipdata MO ",MO," || exit 1
#filter_by_area $ipdata TW ",TW," || exit 1
#filter_by_area $ipdata BIGCHINESE ",CN,|,HK,|,MO,|,TW," || exit 1

ls -lrt IPDATA-*/goodclients-ip-*.conf

#filter_by_area $ipdata ",US," goodclients-usa.conf
#filter_by_area $ipdata ",JP,|.KR," goodclients-japan-korea.conf

#EUROPEFLAGS=",RU,|,UA,|,FR,|,ES,|,SE,|,NO,|,DE,|,FI,|,PL,|,IT,|,GB,|,RO,|,BY,|,KZ,|,GR,|,BG,|,IS,|,HU,|,PT,|,RS,|,IE,|,AT,|,CZ,|,GE,|,GS,|,LT,|,LV,|,HR,|,BA,|,SK,|,EE,|,DK,|,NL,|,CH,|,MD,|,BE,|,AL,|,MK,|,TR,|,SI,|,ME,|,CY,|,AZ,|,LU,|,AD,|,MT,|,LI,|,SM,|,MC,|,VA,"
#filter_by_area $ipdata "$EUROPEFLAGS" goodclients-europe.conf

