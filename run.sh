for i in $(seq $1 $2)
do
    perf stat -x , -r ${3:=1} -e task-clock ./build/src/ddt-exec $i
done
