
set(MINGW_PREFIX /usr/i686-w64-mingw32/)

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)

# Where is the target environment located
set(CMAKE_FIND_ROOT_PATH "${MINGW_PREFIX}/sys-root/mingw")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CROSS_TARGET i686-w64-mingw32)

# Luajit needs a 32bit the host compiler - in x86_64 this means building 32bit
# executables
set(HOST_C_COMPILER cc)
set(HOST_C_FLAGS -m32)
set(HOST_EXE_LINKER ld)
set(HOST_EXE_LINKER_FLAGS -m32)

