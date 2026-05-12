# CMake generated Testfile for 
# Source directory: D:/RPGDemo
# Build directory: D:/RPGDemo/build-test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AStarTests "D:/RPGDemo/build-test/test_astar.exe")
set_tests_properties(AStarTests PROPERTIES  _BACKTRACE_TRIPLES "D:/RPGDemo/CMakeLists.txt;45;add_test;D:/RPGDemo/CMakeLists.txt;0;")
add_test(DemoTests "D:/RPGDemo/build-test/test_demo.exe")
set_tests_properties(DemoTests PROPERTIES  _BACKTRACE_TRIPLES "D:/RPGDemo/CMakeLists.txt;46;add_test;D:/RPGDemo/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
