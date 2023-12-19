cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (SERVICE_USER "user_hl_mai_lab_01")
SET (SERVICE_GOOD "good_hl_mai_lab_01")
SET (SERVICE_CART "cart_hl_mai_lab_01")

set (STD_CXX "c++17")
set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${SERVICE_USER} main_user.cpp 
                                 config/config.cpp
                                 database/cache.cpp
                                 database/database.cpp
                                 database/user.cpp
                                 )

add_executable(${SERVICE_GOOD} main_good.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/good.cpp
                                 )

add_executable(${SERVICE_CART} main_cart.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/cart.cpp
                                 )


target_include_directories(${SERVICE_USER} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${SERVICE_GOOD} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${SERVICE_CART} PRIVATE "${CMAKE_BINARY_DIR}")

target_compile_options(${SERVICE_USER} PRIVATE -Wall -Wextra -pedantic -Werror )
target_compile_options(${SERVICE_GOOD} PRIVATE -Wall -Wextra -pedantic -Werror )
target_compile_options(${SERVICE_CART} PRIVATE -Wall -Wextra -pedantic -Werror )

set_target_properties(${SERVICE_USER} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${SERVICE_GOOD} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${SERVICE_CART} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${SERVICE_USER} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(${SERVICE_GOOD} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(${SERVICE_CART} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${SERVICE_USER} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

target_link_libraries(${SERVICE_GOOD} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)
                             
target_link_libraries(${SERVICE_CART} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)