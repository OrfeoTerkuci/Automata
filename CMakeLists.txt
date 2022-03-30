cmake_minimum_required(VERSION 3.0.0)
project(Automata VERSION 0.4.0)

#include(CTest)
#enable_testing()

set(DFA_sources
        DFA_test.cpp
        DFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
    )

set(NFA_sources
        NFA_test.cpp
        DFA.cpp
        NFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
)

set(SSC_sources
        SSC_test.cpp
        DFA.cpp
        NFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
)

set(MSSC_sources
        MSSC_test.cpp
        ENFA.cpp
        DFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
)

set(PA_sources
        Product_test.cpp
        DFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
)

set(RE_sources
        REGEX_test.cpp
        RE.cpp
        DFA.cpp
        ENFA.cpp
        Node.cpp
        transition.cpp
        transitionNFA.cpp
)

add_executable(DFA ${DFA_sources})

add_executable(NFA ${NFA_sources})

add_executable(SSC ${SSC_sources})

add_executable(MSSC ${MSSC_sources})

add_executable(PA ${PA_sources})

add_executable(RE ${RE_sources})

#install( TARGETS  DESTINATION ${PROJECT_SOURCE_DIR}/debug )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)