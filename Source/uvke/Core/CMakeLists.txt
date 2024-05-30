cmake_minimum_required(VERSION 3.17)
project(uvke-Core)

set(UVKE_CORE_SOURCE
    Core.hpp
    Core.cpp
    Logger.hpp
    Logger.cpp
    Base.hpp
    Base.cpp
    Helper.hpp
    App.hpp
    App.cpp
)

if(UVKE_SHARED_LIBRARY)
    add_library(${PROJECT_NAME} SHARED ${UVKE_CORE_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_CORE_SOURCE})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UVKE_LIBRARIES})
