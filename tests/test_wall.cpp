#include <gtest/gtest.h>
#include "wallet.h"
#include "NotEnoughMoney.h"
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
TEST(WalletTest, WithdrawMoreThanBalanceThrows) {
    wallet w(50);
    EXPECT_THROW(w.withdraw(100), NotEnoughMoney);
    EXPECT_EQ(w.getBal(), 50);
}
TEST(WalletTest, WithdrawExceptionMessage) {
    wallet w(30);
    try {
        w.withdraw(100);
        FAIL() << "Expect NotEnoughMoney";
    } catch (const NotEnoughMoney& e) {
        std::string msg = e.what();
        EXPECT_LT(msg.find("Need:100"), msg.length());
        EXPECT_LT(msg.find("available: 30"), msg.length());
    }
}
TEST(WalletTest, MultipleOperations) {
    wallet w(100);
    w.deposit(50);
    w.withdraw(30);
    w.deposit(80);
    w.withdraw(70);
    w.withdraw(30);
    EXPECT_EQ(w.getBal(), 100);
}
TEST(WalletTest, ExceptionDoesNotChangeBalance) {
    wallet w(75);
    int oldBalance = w.getBal();
    try {
        w.withdraw(100);
    } catch (const NotEnoughMoney&) {
    }
    EXPECT_EQ(w.getBal(), oldBalance);
}