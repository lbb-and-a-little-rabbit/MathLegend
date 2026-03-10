cmake_minimum_required(VERSION 3.26)

project(MathLegend
    VERSION 1.0
    LANGUAGES CXX
)

# 使用 C++20（SFML 3 推荐）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===== SFML 3 配置 =====
set(SFML_DIR
    "D:/SFML-3.0.2-windows-gcc-14.2.0-mingw-64-bit/SFML-3.0.2/lib/cmake/SFML" #sfml路径
)

find_package(SFML 3 REQUIRED COMPONENTS
    Graphics
    Window
    System
    Audio
)

# ===== 源文件 =====
file(GLOB_RECURSE MathLegend_SOURCES
    src/*.cpp
)

# ===== 核心库 =====
add_library(MathLegendCore ${MathLegend_SOURCES})

target_include_directories(MathLegendCore PUBLIC include)

target_link_libraries(MathLegendCore
    PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# 链接 PhysFS
target_link_libraries(MathLegendCore PUBLIC "D:/physfs/physfs/build/libphysfs.a")
target_include_directories(MathLegendCore PUBLIC "D:/physfs/physfs/src")

# ===== 游戏程序 =====
add_executable(MathLegend src/main.cpp)
target_link_libraries(MathLegend PRIVATE MathLegendCore)

# ===== 资源复制 =====
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:MathLegend>/assets
)