
set (glut_template_src main.c display_glut.c math_lib.c)

include_directories (./)
include_directories (${OPENGL_INCLUDE_DIR})
include_directories (${GLUT_INCLUDE_DIR})

# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -fPIC -Wall -Wextra -pedantic -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -fPIC -Wall -Wextra -pedantic -O3")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

add_executable (glut_template ${glut_template_src})
target_link_libraries (glut_template
		${CMAKE_THREAD_LIBS_INIT}
		${OPENGL_LIBRARIES}
		${GLUT_LIBRARIES}
		${M_LIB})
