# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)
# プロジェクト名と使用する言語を設定
project(Lampart CXX)
set (CMAKE_CXX_FLAGS "-std=c++14 -Wall")

# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(main App1.cpp MiddleWare.cpp Sender.cpp Logger.cpp Receiver.cpp RecvThread.cpp)