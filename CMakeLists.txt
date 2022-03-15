cmake_minimum_required(VERSION 3.0.0)
project(DFA VERSION 0.3.0)

#include(CTest)
#enable_testing()

set(DFA_sources
    DFA_test.cpp
    DFA.cpp
    Node.cpp 
    transition.cpp
    )

set(NFA_sources
    NFA_test.cpp
    NFA.cpp
    Node.cpp 
    transition.cpp
)

add_executable(DFA ${DFA_sources})

add_executable(NFA ${NFA_sources})

install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/debug )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)