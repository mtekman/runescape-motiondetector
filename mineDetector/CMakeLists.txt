project(mineDetector)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} args.h
 blobops.h diffimage.h orefinder.h params.h
 playerfinder.h desktopops.h timeops.h typedefs.h
 blobprofile.h cvfuncs.h locatedropzone.h)
find_package( OpenCV REQUIRED )
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )

list( APPEND CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
