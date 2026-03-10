
#pragma once
#include <ostream>

class wallet {
private:
    int balance;
public:
    wallet (int initialAmount) noexcept;
    void withdraw(int amount);
    void deposit(int amount);
    int getBal() const;
};

