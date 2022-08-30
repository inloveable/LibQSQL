# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LibQSql_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LibQSql_autogen.dir\\ParseCache.txt"
  "LibQSql_autogen"
  )
endif()
