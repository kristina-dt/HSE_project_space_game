#include <gtest/gtest.h>
#include "Appliance.h"

TEST(PositionTest, Constructor) {
    Position p1, p2(3, -5);
    EXPECT_EQ(p1.x, 0); EXPECT_EQ(p1.y, 0);
    EXPECT_EQ(p2.x, 3); EXPECT_EQ(p2.y, -5);
}

TEST(PositionTest, IsNearby) {
    Position pos(5, 5);
    EXPECT_TRUE(pos.isNearby(5, 5));
    EXPECT_TRUE(pos.isNearby(6, 5));
    EXPECT_FALSE(pos.isNearby(8, 5, 2.0f));
    EXPECT_TRUE(pos.isNearby(10, 5, 5.0f));
}

TEST(ApplianceFromCppTest, Upgrade) {
    class TestAppliance : public Appliance {
    public:
        TestAppliance(std::string name, int x, int y) {
            name_ = name;
            position_ = Position(x, y);
        }
        std::string getType() const override { return "Test"; }
        bool produce(InformationPlayer&) override { return true; }
        int getUpgradeCost() const override { return 100; }
        int getCurrentPrice() const override { return 50; }
    };

    TestAppliance app("Test", 0, 0);

    EXPECT_EQ(app.getLevel(), 1);

    testing::internal::CaptureStdout();
    app.upgrade();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(app.getLevel(), 2);
    EXPECT_EQ(output, "Test upgraded to level 2!\n");

    app.upgrade();
    EXPECT_EQ(app.getLevel(), 3);
}