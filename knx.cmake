file(GLOB KNX_STACK          "${CMAKE_CURRENT_LIST_DIR}/*.[ch]")
list(APPEND KNX_STACK ${CMAKE_CURRENT_LIST_DIR}/knx.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/)
