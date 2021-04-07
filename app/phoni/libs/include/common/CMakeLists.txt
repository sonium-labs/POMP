set(COMMON_SOURCES common.hpp)

add_library(common OBJECT ${COMMON_SOURCES})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(common)