mkdir build
cd build
cd ..
cmake CMakeLists.txt
make 
./gtest
./loctest < test.txt