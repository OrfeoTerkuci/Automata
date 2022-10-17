cmake_minimum_required(VERSION 3.0.0)
project(Automata VERSION 1.1.0)

#include(CTest)
#enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Ba1INF
set(DFA_sources
        Test_files/DFA_test.cpp
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(NFA_sources
        Test_files/NFA_test.cpp
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/NFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(SSC_sources
        Test_files/SSC_test.cpp
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/NFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(MSSC_sources
        Test_files/MSSC_test.cpp
        "src/Finite Automata/ENFA.cpp"
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(PA_sources
        Test_files/Product_test.cpp
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(RE_sources
        Test_files/REGEX_test.cpp
        "src/Finite Automata/RE.cpp"
        "src/Finite Automata/ENFA.cpp"
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

set(TFA_sources
        Test_files/TFA_test.cpp
        "src/Finite Automata/DFA.cpp"
        "src/Finite Automata/Node.cpp"
        "src/Finite Automata/transition.cpp"
        "src/Finite Automata/transitionNFA.cpp"
        )

# Ba2INF

set(CFG_sources
        Test_files/CFG_test.cpp
        "src/Context Free/CFG.cpp"
        "src/Context Free/Variable.cpp"
        )

set(CNF_sources
        "Test_files/CNF_test.cpp"
        "src/Context Free/CFG.cpp"
        "src/Context Free/Variable.cpp"
        )

add_executable(DFA ${DFA_sources})

add_executable(NFA ${NFA_sources})

add_executable(SSC ${SSC_sources})

add_executable(MSSC ${MSSC_sources})

add_executable(PA ${PA_sources})

add_executable(RE ${RE_sources})

add_executable(TFA ${TFA_sources})

add_executable(CFG ${CFG_sources})

add_executable(CNF ${CNF_sources})

#install( TARGETS  DESTINATION ${PROJECT_SOURCE_DIR}/debug )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)