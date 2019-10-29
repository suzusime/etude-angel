cmake_minimum_required(VERSION 3.14)

project(etude-angel)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ANGELSCRIPT_LIBRARY_NAME angelscript) # OS X Framework以外と仮定

set(ANGEL_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/angelsdk)
add_subdirectory(${ANGEL_SDK_DIR}/angelscript/projects/cmake)

set(ANGEL_ADDON_SRCS
    ${ANGEL_SDK_DIR}/add_on/scriptbuilder/scriptbuilder.cpp
    ${ANGEL_SDK_DIR}/add_on/scriptstdstring/scriptstdstring.cpp
    )

add_executable(etude-angel etude-angel.cpp ${ANGEL_ADDON_SRCS})
target_link_libraries(etude-angel ${ANGELSCRIPT_LIBRARY_NAME})
target_include_directories(etude-angel PRIVATE
    ${ANGEL_SDK_DIR}/angelscript/include
    ${ANGEL_SDK_DIR}/add_on
    )

# コンパイラ依存のオプション
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # aliasがstrictな前提の最適化をしない
    target_compile_options(etude-angel PRIVATE "-fno-strict-aliasing")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # BOM無しUTF-8で書かれたソースコードを受け付け、入出力もUTF-8にする
    target_compile_options(etude-angel PRIVATE "/utf-8")
endif()

# スクリプトファイルを実行フォルダにコピー
add_custom_command(TARGET etude-angel POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hello.as $<TARGET_FILE_DIR:etude-angel>)

