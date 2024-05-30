cmake_minimum_required(VERSION 3.17)
project(uvke-Audio)

set(UVKE_AUDIO_SOURCE
    AudioManager.hpp
    AudioManager.cpp
)

if(UVKE_SHARED_LIBRARY)
    add_library(${PROJECT_NAME} SHARED ${UVKE_AUDIO_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_AUDIO_SOURCE})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UVKE_LIBRARIES})
