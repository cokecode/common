################################################################################
# A simple implementation of hashmap based on bash.
# Examples,
#
#   hmap_create myhash
#   hmap_set myhash id e79692a6eec6d3dfcc3e8672fb58774f
#   hmap_get myhash id
#   hmap_create ctxdata
#   hmap_set ctxdata counter 0
#   hmap_inc ctxdata counter
#
# zhangxun@taobao.com
# 2010-03-19
################################################################################
function hmap_create()
{
    local hashmap_obj_name=$1

    if [ "$hashmap_obj_name" == "" ]; then
        echo "cannot create hashmap object: empty name" >&2
        return 1
    fi

    local script="
        declare -a __hmap_obj_${hashmap_obj_name}_keys;
        declare -a __hmap_obj_${hashmap_obj_name}_vals;
    "
    eval $script
}

function hmap_set()
{
    local hmapname=$1
    local key=$2
    local val=$3
    local hkeys=__hmap_obj_${hmapname}_keys
    local hvals=__hmap_obj_${hmapname}_vals

    local script="
        local array_size=\${#$hkeys[@]};
        local ok=0;
        local i;

        for ((i = 0; i < \$array_size; i++)); do
            if [ \"$key\" == \${$hkeys[\$i]} ]; then
                $hvals[\$i]=$val;
                ok=1;
            fi;
        done;

        if [ \$ok -eq 0 ]; then
            $hkeys[\$array_size]=$key;
            $hvals[\$array_size]=$val;
        fi;
    ";
    #echo $script | sed -e 's/;/\n/g'
    eval $script
    return
}

function hmap_get()
{
    local hmapname=$1
    local key=$2
    local val=$3
    local hkeys=__hmap_obj_${hmapname}_keys
    local hvals=__hmap_obj_${hmapname}_vals

    local script="
        local array_size=\${#$hkeys[@]};
        local i;
        for ((i = 0; i < \$array_size; i++)); do
            if [ \${$hkeys[\$i]} == \"$key\" ]; then
                echo \${$hvals[\$i]};
            fi;
        done
    "
    eval $script
}

function hmap_inc()
{
    local hmapname=$1
    local key=$2
    local hkeys=__hmap_obj_${hmapname}_keys
    local hvals=__hmap_obj_${hmapname}_vals

    local script="
        local array_size=\${#$hkeys[@]};
        local ok=0;
        local i;
        for ((i = 0; i < \$array_size; i++)); do
            if [ \${$hkeys[\$i]} == \"$key\" ]; then
                (($hvals[\$i]=\${$hvals[\$i]}+1));
                ok=1;
            fi;
        done;
        if [ \$ok -eq 0 ]; then
            $hkeys[\$array_size]=$key;
            $hvals[\$array_size]=1;
        fi;
    "
    eval $script
}

function hmap_keys()
{
    local hmapname=$1
    local hkeys=__hmap_obj_${hmapname}_keys
    local hvals=__hmap_obj_${hmapname}_vals

    local script="
        echo \${$hkeys[@]}
    "
    eval $script
}
################################################################################

# vim:ts=4:sw=4:ai:et
