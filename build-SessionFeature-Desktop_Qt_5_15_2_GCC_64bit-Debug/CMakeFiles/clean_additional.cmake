# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SessionFeature_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SessionFeature_autogen.dir/ParseCache.txt"
  "CMakeFiles/mytread_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/mytread_autogen.dir/ParseCache.txt"
  "CMakeFiles/reciver_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/reciver_autogen.dir/ParseCache.txt"
  "SessionFeature_autogen"
  "mytread_autogen"
  "reciver_autogen"
  )
endif()
