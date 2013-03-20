while [ $# -gt 0 ]; do
    case $1 in 
        --prod) opt_prodname=$2; shift;;
        --firstts) opt_firstts=$2; shift;;
        -b|--background) opt_background=yes;;
        --stop) opt_stop=yes;;
        --debug) opt_debug=yes;;
        --indaemon) opt_indaemon=yes;;  ## do not use this option directly
        *) usage;;
    esac
    shift
done
