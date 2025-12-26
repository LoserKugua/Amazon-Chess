cmake_minimum_required(VERSION 3.15)
project(AmazonChess CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON) 

# 查找 Qt 6 库
find_package(Qt6 COMPONENTS Widgets Gui Multimedia Concurrent REQUIRED)

set(GAME_CORE_SRC
    src/GameCore/GameEngine.cpp
    src/GameCore/Board.cpp
    src/GameCore/move.cpp
    src/GameCore/GameState.cpp
)

set(GUI_SRC
    src/GUI/ChessWindow.cpp
    src/GUI/ChessGrid.cpp
    src/GUI/GameMenu.cpp
    src/GUI/ChessBoard.cpp
    src/GUI/TwoPlayer.cpp
    src/GUI/ChessBoardView.cpp
    src/GUI/OnePlayer.cpp
    src/GUI/StyleSheets.cpp
    src/GUI/Settings.cpp
)

set(QRC
    resources/imageResources.qrc
)

add_executable(AmazonChess
    src/main.cpp
    src/AI/MCTS.cpp
    ${GAME_CORE_SRC}
    ${GUI_SRC}
    ${QRC}
)

set_target_properties(AmazonChess PROPERTIES OUTPUT_NAME "Amazons")

# 这个东西是拿来不生成debug的终端框用的
set_target_properties(AmazonChess PROPERTIES WIN32_EXECUTABLE TRUE)

target_compile_features(AmazonChess PRIVATE cxx_std_17)

target_link_libraries(AmazonChess PRIVATE Qt6::Widgets Qt6::Gui Qt6::Multimedia Qt6::Concurrent)

set_property(TARGET AmazonChess PROPERTY CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(TARGET AmazonChess PROPERTY CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(TARGET AmazonChess PROPERTY CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)