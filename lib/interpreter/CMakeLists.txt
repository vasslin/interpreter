add_library(interpreter interpreter.h interpreter.cpp)

target_link_libraries(interpreter PRIVATE parser)
target_link_libraries(interpreter PRIVATE ast)

