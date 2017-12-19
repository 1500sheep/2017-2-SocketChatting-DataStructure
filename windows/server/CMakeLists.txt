# CMake powered build system for data structure.
# Create by Jaemyun Kim
# First draft is Dec. 01, 2017

file (GLOB SERVER_HDR "*.h*")
file (GLOB SERVER_SRC "*.c*")


# Variables setting for this project ####################################
# set variable for this project
SET(PROJ_NAME server)
SET(PROJ_FILES ${SERVER_HDR} ${SERVER_SRC})
SET(PROJ_LIBS ws2_32.lib)


# set build target #############################################
SET_SOURCE_FILES_PROPERTIES(${PROJ_FILES}
	PROPERTIES
	COMPILE_FLAGS "-D__NO_UI__ -D_CRT_SECURE_NO_WARNINGS")
ADD_EXECUTABLE(${PROJ_NAME} ${PROJ_FILES})
TARGET_LINK_LIBRARIES(${PROJ_NAME} ${PROJ_LIBS})
