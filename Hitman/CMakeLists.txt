cmake_minimum_required(VERSION 3.16)
project(Hitman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

# Optionally force a compiler (not recommended if selected in GUI)
#set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

# Enable static linking of implicit libraries (standard C++ and GCC runtime)
add_compile_options(-static-libstdc++ -static-libgcc -pthread)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc -pthread")
execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libwinpthread.a
    OUTPUT_VARIABLE WINPTHREAD_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# SQL update operations
add_executable(SQLUpdater
    ${CMAKE_SOURCE_DIR}/src/SQLUpdater.cpp
    ${CMAKE_SOURCE_DIR}/src/sqlite3.c
)
add_custom_target(run_sql_updater ALL
    COMMAND SQLUpdater
    DEPENDS SQLUpdater
    COMMENT "Running SQLUpdater to patch hitman.db"
)

# Stuff for initializing resources
file(READ ${CMAKE_SOURCE_DIR}/resources/image1.png IMG1 HEX)
file(READ ${CMAKE_SOURCE_DIR}/resources/image2.png IMG2 HEX)
file(READ ${CMAKE_SOURCE_DIR}/resources/arial.ttf FNT1 HEX)
file(READ ${CMAKE_SOURCE_DIR}/resources/hitman.db DTB1 HEX)
configure_file(${CMAKE_SOURCE_DIR}/src/resourceUpdater.cpp.in ${CMAKE_BINARY_DIR}/resourceUpdater.cpp @ONLY)

# Resource update operations
add_executable(resourceUpdater ${CMAKE_BINARY_DIR}/resourceUpdater.cpp)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/resource.h
    COMMAND $<TARGET_FILE:resourceUpdater>
    DEPENDS resourceUpdater
)
add_custom_target(generate_resource_h ALL
    DEPENDS ${CMAKE_BINARY_DIR}/resource.h
    COMMENT "Running ResourceUpdater to load resource binaries"
)

# SFML install path
set(SFML_DIR "C:/Users/Owner/Documents/C++_Libraries/SFML-install/lib/cmake/SFML")
set(SFML_ROOT "C:/Users/Owner/Documents/C++_Libraries/SFML-install")

include_directories("${SFML_ROOT}/include")
link_directories("${SFML_ROOT}/lib")

add_executable(Hitman
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/sqlite3.c
)

set_target_properties(Hitman PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Make resource.h available
add_dependencies(Hitman generate_resource_h)
target_include_directories(Hitman PRIVATE ${CMAKE_BINARY_DIR})

# TGUI FetchContent
include(FetchContent)

set(TGUI_SHARED_LIBS OFF)
set(TGUI_BACKEND SFML_GRAPHICS)
set(TGUI_OPT_ALL_WIDGETS ON)

# Fetch TGUI
FetchContent_Declare(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG v1.10.0
)

FetchContent_MakeAvailable(TGUI)

# Point to static SFML libs and dependencies
link_directories("C:/Users/Owner/Documents/C++_Libraries/SFML-install/lib")

# Required for static SFML + TGUI
target_compile_definitions(Hitman PRIVATE SFML_STATIC TGUI_STATIC)

target_link_options(Hitman PRIVATE -mwindows)

# Link TGUI and SFML + dependencies + implicit libraries
target_link_libraries(Hitman
    tgui
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    opengl32
    winmm
    gdi32
    freetype
    #flac
    "C:/Users/Owner/Documents/C++_Libraries/SFML-install/lib/libFLAC.a"
    vorbis
    vorbisfile
    ogg
    "${WINPTHREAD_PATH}"
)