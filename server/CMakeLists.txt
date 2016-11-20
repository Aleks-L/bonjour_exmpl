cmake_minimum_required(VERSION 2.8)

set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#aux_source_directory(. SRC_LIST)

##cmake -DCMAKE_BUILD_TYPE=Release
##cmake -DCMAKE_BUILD_TYPE=Debug

#########################################################################
project(bon-server)


################################################
## Platform dependency

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LIB_SUFIX, ".so")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif()

#########################################################################
## Header files

LIST (APPEND HDR_LIST
    ${PROJECT_DIR}/serv_common.h
)

#########################################################################
## Source and header files

LIST (APPEND CLASS_LIST
    ${PROJECT_DIR}/bonjourservice
    ${PROJECT_DIR}/filehandler
    ${PROJECT_DIR}/server
)

foreach (class ${CLASS_LIST})
    LIST (APPEND SRC_LIST ${class}.cpp)
    LIST (APPEND HDR_LIST ${class}.h)
endforeach ()

LIST (APPEND SRC_LIST
    ${PROJECT_DIR}/main.cpp
)

#########################################################################
## Flags

set(CMAKE_CC_COMPILER gcc)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g -O0 -D_DEBUG=1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -D_DEBUG=1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

## Include dirs
include_directories(AFTER SYSTEM
        ${PROJECT_DIR}
        ${PROJECT_DIR}/../third_party
)

link_directories(${PROJECT_DIR}/../third_party)

ADD_DEFINITIONS(-std=c++11
               )

#########################################################################
## Building
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDR_LIST})

target_link_libraries(${PROJECT_NAME}
    pthread
    dns_sd
)
