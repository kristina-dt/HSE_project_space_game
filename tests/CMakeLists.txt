add_executable(SpaceGame test_wall.cpp)
target_include_directories(SpaceGame PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(SpaceGame
        GTest::gtest_main
)
enable_testing()


add_test(NAME SpaceGame COMMAND SpaceGame)