add_executable(SpaceG test_wall.cpp)
target_include_directories(SpaceG PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(SapceG
        GTest::gtest_main
)
enable_testing()


add_test(NAME SpaceG COMMAND SpaceG)