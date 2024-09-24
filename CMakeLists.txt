cmake_minimum_required(VERSION 3.15)

project(
    svgandme 
    VERSION 0.1.0
    LANGUAGES CXX)

add_library(svgandme PUBLIC)
add_library(svgandme::svgandme ALIAS svgandme)

target_include_directories(svgandme 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(svgandme PRIVATE Blend2D::Blend2D)

# Install the library (optional)
install(TARGETS svgandme DESTINATION lib)
install(FILES ${PROJECT_SOURCE_DIR}/include DESTINATION include)
