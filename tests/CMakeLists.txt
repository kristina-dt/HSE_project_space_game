add_executable(SpaceGame_test
        test_wall.cpp
        ../src/wallet.cpp
        test_ResourceOrder.cpp
        ../src/ResourceOrder.cpp
        test_Ship.cpp
        ../src/Ship.cpp
        test_InfoPlayer.cpp
        ../src/InformationPlayer.cpp
        ../include/NotEnoughMoney.h
        test_Appliance.cpp
        ../src/Appliance.cpp
        test_FoodMaker.cpp
        ../src/FoodMaker.cpp
        test_FuelMaker.cpp
        ../src/FuelMaker.cpp
        test_PartAssembler.cpp
        ../src/PartAssembler.cpp
        test_ProductionManager.cpp
        ../src/ProductionManager.cpp
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