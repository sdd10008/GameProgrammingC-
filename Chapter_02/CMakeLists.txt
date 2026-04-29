cmake_minimum_required(VERSION 3.20)
project(Chapter_02 VERSION 0.1.0)

# C++17を使用
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 依存ライブラリを検索
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# 実行ファイルを作成
add_executable(${PROJECT_NAME} 
    src/main.cpp
 "include/Actor.h" "include/Component.h" "src/Game.cpp" "src/Actor.cpp" "src/Component.cpp")

# ライブラリをリンク
target_link_libraries(${PROJECT_NAME} PRIVATE 
    fmt::fmt
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

# インクルードディレクトリ
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)