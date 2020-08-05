cmake_minimum_required( VERSION 3.15 )
project( dogirealms )

enable_language( CXX )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release;Native" )

set( CMAKE_CXX_FLAGS         "-std=c++20" )
set( CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -Wpedantic -Og -march=core2 -mtune=native -ggdb3" )
set( CMAKE_CXX_FLAGS_RELEASE "-w -O2 -march=core2 -mtune=generic -DNDEBUG" )
set( CMAKE_CXX_FLAGS_NATIVE  "-w -Ofast -march=native -mtune=native -DNDEBUG" )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release )
endif()
message( "Using build configuration: ${CMAKE_BUILD_TYPE}" )

find_package( Threads REQUIRED )
find_package( SDL2 REQUIRED )

include_directories( "src" )

file( GLOB_RECURSE MAIN_FILES 
	"src/*.cc"
	"src/**/*.cc"
)

add_executable( dogirealms ${MAIN_FILES} )
target_link_libraries( dogirealms PUBLIC Threads::Threads SDL2::SDL2 )
install( TARGETS dogirealms RUNTIME DESTINATION "bin" )
