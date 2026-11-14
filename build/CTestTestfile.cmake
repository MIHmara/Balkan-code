# CMake generated Testfile for 
# Source directory: C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game
# Build directory: C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(all_tests "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/build/Debug/maze_tests.exe")
  set_tests_properties(all_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;34;add_test;C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(all_tests "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/build/Release/maze_tests.exe")
  set_tests_properties(all_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;34;add_test;C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(all_tests "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/build/MinSizeRel/maze_tests.exe")
  set_tests_properties(all_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;34;add_test;C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(all_tests "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/build/RelWithDebInfo/maze_tests.exe")
  set_tests_properties(all_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;34;add_test;C:/Users/Martin/OneDrive/Pictures/maze-game/maze-game/CMakeLists.txt;0;")
else()
  add_test(all_tests NOT_AVAILABLE)
endif()
