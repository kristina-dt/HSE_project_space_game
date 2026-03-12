add_executable(SpaceGame_test
        test_wall.cpp
        ../src/wallet.cpp
        test_ResourceOrder.cpp
        ../src/ResourceOrder.cpp
        test_Ship.cpp
        ../src/Ship.cpp
        test_InfoPlayer.cpp
        ../src/InformationPlayer.cpp
)
target_link_libraries(SpaceGame_test
        GTest::gtest_main
        GTest::gtest
        SFML::Graphics
)
target_include_directories(SpaceGame_test PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

enable_testing()

add_test(NAME SpaceGame_test COMMAND SpaceGame_test)