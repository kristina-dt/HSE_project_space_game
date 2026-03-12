#include <gtest/gtest.h>
#include <InformationPlayer.h>

TEST(InformationPlayerTest, Constructor) {
    InformationPlayer player("TestPlayer", 10.5f, 20.5f);
    EXPECT_EQ(player.getName(), "TestPlayer");
    EXPECT_EQ(player.getX(), 10.5f);
    EXPECT_EQ(player.getY(), 20.5f);
    EXPECT_EQ(player.getWal().getBal(), 100);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Food), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Drinks), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Details), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Decorations), 0);
}
TEST(InformationPlayerTest, ConstructorDefault) {
    InformationPlayer player("TestPlayer", 2.0f, 3.0f);

    EXPECT_EQ(player.getX(), 2.0f);
    EXPECT_EQ(player.getY(), 3.0f);
}
