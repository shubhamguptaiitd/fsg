./convert "$1" "gspan_input_file.gspan"
n=`grep -o t gspan_input_file.gspan | wc -l`
sup="0.3"
if [ "$n" -gt 90000 ]
then
         sup="0.45"
elif [ "$n" -gt 80000 ]
then
         sup="0.40"
elif [ "$n" -gt 70000 ]
then
         sup="0.35"
elif [ "$n" -gt 65000 ] 
then
         sup="0.25"
elif [ "$n" -gt 55000 ]
then
        sup="0.20"
elif [ "$n" -gt 30000 ]
then
        sup="0.17"
elif [ "$n" -gt 18000 ]
then
        sup="0.15"
else
        sup="0.14"
fi

echo $sup > tmp
#echo "$n" > tmp
./binaries/gSpan -f "gspan_input_file.gspan" -s "$sup" -o -i
./index "$1" "gspan_input_file.gspan.fp"