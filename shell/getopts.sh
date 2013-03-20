#! /bin/bash

while getopts hfx:y: arg; do
    case $arg in
    h|-help)
        echo "help"
        ;;
    f)
        echo "flag"
        ;;
    x)
        echo "x = $OPTARG"
        ;;
    y)
        echo "y = $OPTARG"
        ;;
    *)
        exit 1
    esac
done

# bash builtin getopts seems not support option like "x::"
# does not support long options
