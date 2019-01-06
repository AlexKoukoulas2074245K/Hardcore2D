cmake_minimum_required(VERSION 3.1)
project(Ninja)

MACRO(SOURCE_GROUP_BY_FOLDER target)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${target}_SRC} ${${target}_HEADERS})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}/${target}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
ENDMACRO(SOURCE_GROUP_BY_FOLDER)

# Write demo-config.h
message("Generating header file: ${CMAKE_BINARY_DIR}/demo-config.h")
set(DEMO_ENABLE_MULTISAMPLE 0 CACHE BOOL "Enable multisample anti-aliasing")
configure_file(demo-config.h.in "${CMAKE_BINARY_DIR}/demo-config.h")

# Find SDL2 and OpenGL
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)
if(NOT WIN32)
    find_package(OpenGL REQUIRED)
endif()

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR})

add_executable(Ninja Main.cpp Main.h old/OldMain.cpp)
target_link_libraries(Ninja ${SDL2_LIBS} ${OPENGL_LIBRARIES})

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Ninja POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Ninja>)
    endforeach()
endif()

if(MSVC)
  target_compile_options(Ninja PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(Ninja PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)