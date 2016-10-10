The software is written in C++, so g++ should be installed. The build framework is cmake. 
Also Boost.Test and Boost.Serialization were used for unit tests and classes 
serialization respectively. Make sure, you have everything installed. 

How to build on Ubuntu 14.04 LTS:
1) Make sure you have g++, cmake and boost installed:
sudo apt-get install build-essential cmake libboost-test-dev libboost-serialization-dev

2) extract the archive, create the build directory and build the project into it:
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make

3) run tests:
make test

4) copy test data and run two applications: "build_tree" and "query_tree"
cp ../query_data.csv ../sample_data.csv ./apps
cd apps
./build_kdtree sample_data.csv tree.txt
./query_kdtree tree.txt query_data.csv output.txt

5) the output in output.txt file