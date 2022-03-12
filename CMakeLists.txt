cmake_minimum_required(VERSION 3.0.0)
project(DFA VERSION 0.3.0)

include(CTest)
enable_testing()

set(exe_name DFA)

set(DFA_sources
    main.cpp
    DFA.cpp
    Node.cpp 
    transition.cpp
    )

add_executable(${exe_name} ${DFA_sources})
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)