cmake_minimum_required(VERSION 3.0.0)
project(Automata VERSION 0.4.0)

#include(CTest)
#enable_testing()

set(DFA_sources
        Test_files/DFA_test.cpp
        main_sources/DFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
    )

set(NFA_sources
        Test_files/NFA_test.cpp
        main_sources/DFA.cpp
        main_sources/NFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
)

set(SSC_sources
        Test_files/SSC_test.cpp
        main_sources/DFA.cpp
        main_sources/NFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
)

set(MSSC_sources
        Test_files/MSSC_test.cpp
        main_sources/ENFA.cpp
        main_sources/DFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
)

set(PA_sources
        Test_files/Product_test.cpp
        main_sources/DFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
)

set(RE_sources
        Test_files/REGEX_test.cpp
        main_sources/RE.cpp
        main_sources/ENFA.cpp
        main_sources/DFA.cpp
        help_sources/Node.cpp
        help_sources/transition.cpp
        help_sources/transitionNFA.cpp
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