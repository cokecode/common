declare -a array
array[0]=0
array[1]=1
echo ${#array[@]}
for ((i = 0; i < ${#array[@]}; i++)); do
    echo ${array[$i]}
done
