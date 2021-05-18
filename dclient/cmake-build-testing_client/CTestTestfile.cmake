# CMake generated Testfile for 
# Source directory: /home/dushn/Testing_diplomski
# Build directory: /home/dushn/Testing_diplomski/cmake-build-testing_client
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test "client" "5000")
set_tests_properties(test PROPERTIES  PASS_REGULAR_EXPRESSION "port number is not correct" _BACKTRACE_TRIPLES "/home/dushn/Testing_diplomski/CMakeLists.txt;26;add_test;/home/dushn/Testing_diplomski/CMakeLists.txt;0;")
add_test(test1 "client" "0000er")
set_tests_properties(test1 PROPERTIES  PASS_REGULAR_EXPRESSION "random chars found" _BACKTRACE_TRIPLES "/home/dushn/Testing_diplomski/CMakeLists.txt;27;add_test;/home/dushn/Testing_diplomski/CMakeLists.txt;0;")
add_test(test2 "client" "5000")
set_tests_properties(test2 PROPERTIES  PASS_REGULAR_EXPRESSION "5000" _BACKTRACE_TRIPLES "/home/dushn/Testing_diplomski/CMakeLists.txt;28;add_test;/home/dushn/Testing_diplomski/CMakeLists.txt;0;")
