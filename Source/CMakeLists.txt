cmake_minimum_required(VERSION 2.8)
project( BackgroundRemover )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( BackgroundRemover BackgroundRemover.cpp )
target_link_libraries( BackgroundRemover ${OpenCV_LIBS} )
target_link_libraries( BackgroundRemover )

project( BackgroundRemover_Nichole )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( BackgroundRemover_Nichole BackgroundRemover_Nichole.cpp )
target_link_libraries( BackgroundRemover_Nichole ${OpenCV_LIBS} )
target_link_libraries( BackgroundRemover_Nichole )
project( BackgroundRemover_Brian )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( BackgroundRemover_Brian bkgd_bts.cpp )
target_link_libraries( BackgroundRemover_Brian ${OpenCV_LIBS} )
target_link_libraries( BackgroundRemover_Brian )
