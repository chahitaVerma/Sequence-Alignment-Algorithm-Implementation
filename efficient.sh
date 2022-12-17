input_file=$1
output_file=$2

g++ -std=c++11 -o compiledFile efficient.cpp 
chmod 777 compiledFile
./compiledFile "$input_file" "$output_file"