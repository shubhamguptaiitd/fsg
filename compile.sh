#g++-7 code_2/convert_input_to_gspan.cpp -o convert_to_gspan -O3
#g++-7 -I /usr/local/opt/boost/include index.cpp -o index -O3 
#./binaries/gSpan -f ./data/167.gspan.cp -s 0.40 -o -i

g++ index.cpp -I /home/apps/LIBBOOST/1.64.0/gnu_ucs71/include/ -oindex -O3
g++ convert_input_to_gspan.cpp -oconvert -O3
