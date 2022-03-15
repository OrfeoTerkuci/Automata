cmake_minimum_required(VERSION 3.0.0)
project(Automata VERSION 0.4.0)

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
    DFA.cpp
    NFA.cpp
    Node.cpp 
    transition.cpp
)

set(SSC_sources
    SSC_test.cpp
    DFA.cpp 
    NFA.cpp 
    Node.cpp 
    transition.cpp 
)

add_executable(DFA ${DFA_sources})

add_executable(NFA ${NFA_sources})

add_executable(SSC ${SSC_sources})

#install( TARGETS  DESTINATION ${PROJECT_SOURCE_DIR}/debug )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)