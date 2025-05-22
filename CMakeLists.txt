cmake_minimum_required(VERSION 3.15)

project(malitop VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(malitop
    main.cpp
)

find_package(ncurses QUIET)

if(NOT ncurses_FOUND)
    add_custom_target(
        ncurses_build ALL
        COMMAND ./configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS="${CMAKE_CXX_FLAGS}" --host=${CMAKE_SYSTEM_PROCESSOR}-none-linux-gnu --prefix=${CMAKE_BINARY_DIR}/thirdparty/ncurses --without-develop --enable-static --disable-shared --without-ada --without-progs --without-tack --without-tests --without-manpages --with-termlib
        COMMAND make -j$(nproc)
        COMMAND make install
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/thirdparty/ncurses
    )

    target_include_directories(malitop PRIVATE ${CMAKE_BINARY_DIR}/thirdparty/ncurses/include/)
    add_dependencies(malitop ncurses_build)
else()
    message(STATUS "Using pre-installed ncurses")
    target_include_directories(malitop PRIVATE ${NCURSES_INCLUDE_DIRS})
    target_link_libraries(malitop PRIVATE ${NCURSES_LIBRARIES})
endif()

add_subdirectory(thirdparty/libGPUCounters)

target_include_directories(malitop PRIVATE
    ${CMAKE_SOURCE_DIR}/thirdparty/libGPUCounters/backend/device/include
    ${CMAKE_SOURCE_DIR}/thirdparty/libGPUCounters/hwcpipe/include)


target_link_libraries(malitop
    PRIVATE ${CMAKE_BINARY_DIR}/thirdparty/libGPUCounters/hwcpipe/libhwcpipe.a
            ${CMAKE_BINARY_DIR}/thirdparty/libGPUCounters/backend/device/libdevice.a
            ${CMAKE_BINARY_DIR}/thirdparty/ncurses/lib/libncurses.a
)

target_compile_options(malitop
    PRIVATE -Werror
            -Wswitch-default
            -Wswitch-enum
            -fexceptions
)
