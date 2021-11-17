#!/bin/bash
#
#Detected ixgbe

MAX_DEVICE_CNT=64
ip="/usr/ali/bin/ip"
log_file="/tmp/detected_ixgbe.log"

declare -a pfdevices

function DLOG() {
    echo "[`date`] [INFO] \"$@\"" | tee -a $log_file 
}

function LOG_ERR() { 
    echo -e "[`date`] [ERROR] $@" | tee -a $log_file
    RESULT_VAL=1
}

function get_pf() {
  idx=0
  for ((i=0;i<${MAX_DEVICE_CNT};i++)); do
    driver=`$ip link show eth${i} >/dev/null 2>&1 && ethtool -i eth${i} | grep driver | awk '{print $2}'`
    if [ "${driver}" == "ixgbe" ]; then
        DLOG "eth${i} is ixgbe driver" 
        pfdevices[idx]=eth${i}
        let idx=$idx+1
    fi
  done

  if [ ${#pfdevices[@]} -ne 2 ]; then
    DLOG "fatal error: totally ${#pfdevices[@]} PFs exists with ixgbe driver" 
    exit 1
  else
    DLOG "SR-IOV support"
  fi
}

get_pf

exit $RESULT_VAL
