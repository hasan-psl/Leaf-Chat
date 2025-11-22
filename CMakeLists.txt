cmake_minimum_required(VERSION 3.10)
project(Leaf-Chat C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set default build type to Release for smallest binary size
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Disable rpath
set(CMAKE_SKIP_RPATH ON)
set(CMAKE_SKIP_BUILD_RPATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH OFF)

# Global compiler flags for smallest binary size
set(CMAKE_C_FLAGS_RELEASE "-Os -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -ffunction-sections -fdata-sections")

# Global linker flags for dead code elimination and symbol stripping
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -s")

# Disable debug info globally
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g0")

# Find pkg-config
find_package(PkgConfig REQUIRED)

# Find GTK3
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Find WebKit2GTK
pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.1)

# Add executable
add_executable(whatsapp main.c)

# Include directories
target_include_directories(whatsapp PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS})

# Link libraries
target_link_libraries(whatsapp ${GTK3_LIBRARIES} ${WEBKIT2_LIBRARIES})

# Link directories
target_link_directories(whatsapp PRIVATE ${GTK3_LIBRARY_DIRS} ${WEBKIT2_LIBRARY_DIRS})

# Compiler flags from pkg-config
target_compile_options(whatsapp PRIVATE ${GTK3_CFLAGS_OTHER} ${WEBKIT2_CFLAGS_OTHER})

