cmake_minimum_required(VERSION 3.5)
project(Automata VERSION 1.1.0)

#include(CTest)
#enable_testing()

set(CMAKE_CXX_FLAGS_INIT "-Werror -Wall -Wextra -Wpedantic")

set(CMAKE_CXX_STANDARD 14)

# Ba1INF
set(DFA_sources
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(NFA_sources
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/NFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(MSSC_sources
        "src/Finite Automata/ENFA.cpp"
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(RE_sources
        "src/Finite Automata/RE.cpp"
        "src/Finite Automata/ENFA.cpp"
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

# Ba2INF

set(CFG_sources
        "src/Context Free/CFG.cpp"
        "src/Context Free/Variable.cpp"
        )

set(PDA_sources
        Test_files/PDA_test.cpp
        "src/Context Free/PDA.h"
        "src/Context Free/PDA.cpp"
        "src/Context Free/CFG.h"
        "src/Context Free/CFG.cpp"
        "src/Context Free/State.cpp"
        "src/Context Free/Transition.cpp"
        "src/Context Free/Variable.h"
        "src/Context Free/Variable.cpp"
        )

set(LL1_sources
        "src/Context Free/CFG.cpp"
        "src/Context Free/Variable.cpp"
        )

add_executable(DFA ${DFA_sources} Test_files/DFA_test.cpp)

add_executable(NFA ${NFA_sources} Test_files/NFA_test.cpp)

add_executable(SSC ${NFA_sources} Test_files/SSC_test.cpp)

add_executable(MSSC ${MSSC_sources} Test_files/MSSC_test.cpp)

add_executable(PA ${DFA_sources} Test_files/Product_test.cpp)

add_executable(RE ${RE_sources} Test_files/REGEX_test.cpp)

add_executable(TFA ${DFA_sources} Test_files/TFA_test.cpp)

add_executable(CFG ${CFG_sources} Test_files/CFG_test.cpp)

add_executable(CNF ${CFG_sources} "Test_files/CNF_test.cpp")

add_executable(CYK ${CFG_sources} "Test_files/CYK_test.cpp")

add_executable(PDA ${PDA_sources})

add_executable(LL1 ${CFG_sources} "Test_files/LL1_test.cpp")

INSTALL(TARGETS DFA NFA SSC MSSC PA RE TFA CFG CNF CYK PDA LL1 DESTINATION "${CMAKE_BINARY_DIR}/installed")

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "default install path" FORCE)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)