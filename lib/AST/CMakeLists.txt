add_library(ast AST.h AST.cpp)

target_link_libraries(ast PRIVATE nodes)