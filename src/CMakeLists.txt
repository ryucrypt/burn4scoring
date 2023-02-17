project(burn4scoring)

find_package(cdt)

add_contract( burn4scoring burn4scoring burn4scoring.cpp )
target_include_directories( burn4scoring PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( burn4scoring ${CMAKE_SOURCE_DIR}/../ricardian )