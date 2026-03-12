add_executable(SpaceGame_test test_wall.cpp)
target_link_libraries(SpaceGame_test
        GTest::gtest_main
)
target_include_directories(SpaceGame_tset PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

enable_testing()


add_test(NAME SpaceGame_test COMMAND SpaceGame_test)