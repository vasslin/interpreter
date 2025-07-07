add_library(parser parser.h parser.cpp)

target_link_libraries(parser PRIVATE nodes)
target_link_libraries(parser PRIVATE lexer)
target_link_libraries(parser PRIVATE ast)
