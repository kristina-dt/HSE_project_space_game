#include <gtest/gtest.h>
#include "wallet.h"
TEST(WalletTest, ConstructorWithPositiveAmount) {
    wallet w(100);
    EXPECT_EQ(w.getBal(), 100);
}
TEST(WalletTest, ConstructorWithZeroAmount) {
    wallet w(0);
    EXPECT_EQ(w.getBal(), 0);
}
TEST(WalletTest, ConstructorWithNegativeAmount) {
    wallet w(-50);
    EXPECT_EQ(w.getBal(), 0);
}
TEST(WalletTest, DepositMoney) {
    wallet w(100);
    w.deposit(50);
    EXPECT_EQ(w.getBal(), 150);

    w.deposit(25);
    EXPECT_EQ(w.getBal(), 175);
}
TEST(WalletTest, DepositFromZero) {
    wallet w(0);
    w.deposit(200);
    EXPECT_EQ(w.getBal(), 200);
}
TEST(WalletTest, WithdrawMoney) {
    wallet w(100);
    w.withdraw(40);
    EXPECT_EQ(w.getBal(), 60);
}
