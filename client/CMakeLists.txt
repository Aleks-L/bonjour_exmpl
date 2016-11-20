cmake_minimum_required(VERSION 2.8)

set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#aux_source_directory(. SRC_LIST)

##cmake -DCMAKE_BUILD_TYPE=Release
##cmake -DCMAKE_BUILD_TYPE=Debug

#########################################################################
project(bon-client)


#########################################################################
## Header files

#LIST (APPEND HDR_LIST
#    ${PROJECT_DIR}/include/common.h
#)

#########################################################################
## Source and header files

LIST (APPEND PAIR_LIST
    ${PROJECT_DIR}/bonjourbrowse
)

foreach (pair ${PAIR_LIST})
    LIST (APPEND SRC_LIST ${pair}.c)
    LIST (APPEND HDR_LIST ${pair}.h)
endforeach ()

LIST (APPEND SRC_LIST
    ${PROJECT_DIR}/main.c
)

#########################################################################
## Flags

set(CMAKE_CC_COMPILER gcc)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")


## Include dirs
include_directories(AFTER SYSTEM
        ${PROJECT_DIR}
        ${PROJECT_DIR}/../third_party
)

link_directories(${PROJECT_DIR}/../third_party)

ADD_DEFINITIONS(-std=gnu99
               )

#########################################################################
## Building
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDR_LIST})

target_link_libraries(${PROJECT_NAME}
    pthread
    dns_sd
)
