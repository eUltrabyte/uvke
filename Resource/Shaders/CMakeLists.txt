cmake_minimum_required(VERSION 3.17)
project(uvke-Shaders)

if(WIN32)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
endif()

file(GLOB_RECURSE UVKE_GLSL_SOURCE
    "${CMAKE_SOURCE_DIR}/Resource/Shaders/*.frag"
    "${CMAKE_SOURCE_DIR}/Resource/Shaders/*.vert"
)

foreach(UVKE_GLSL_FILE ${UVKE_GLSL_SOURCE})
    get_filename_component(FILE_NAME ${UVKE_GLSL_FILE} NAME)
    set(SPIRV "${CMAKE_SOURCE_DIR}/Resource/Shaders/${FILE_NAME}.spv")
    add_custom_command(OUTPUT ${SPIRV} COMMAND ${GLSL_VALIDATOR} -V ${UVKE_GLSL_FILE} -o ${SPIRV} DEPENDS ${UVKE_GLSL_FILE})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(UVKE_GLSL_FILE)

add_custom_target(${PROJECT_NAME} DEPENDS ${SPIRV_BINARY_FILES})
