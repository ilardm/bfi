project( bfi )
cmake_minimum_required( VERSION 2.8 )

aux_source_directory( src/ SOURCES )
include_directories( ${PROJECT_SOURCE_DIR}/inc ${CMAKE_BINARY_DIR} )

set( Version "0.01" )
configure_file( ${PROJECT_SOURCE_DIR}/inc/rev.h.in ${CMAKE_BINARY_DIR}/rev.h )

if ( CMAKE_COMPILER_IS_GNUCC )
  set( ModeFlags "-O0 -g -D_DEBUG" )

  if ( DEFINED Mode AND Mode STREQUAL "release" )
	set( ModeFlags "-O3 -D_RELEASE" )
  endif()
else()
  set( ModeFlags "" )
endif()

add_definitions( ${ModeFlags} )
add_executable( ${PROJECT_NAME} ${SOURCES} )
