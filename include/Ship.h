#pragma once

#include "Order.h"

#include <memory>
#include <optional>
#include <string>

class Ship {
private:
    float x_;
    float y_;
    float speed_;
    bool active_;
    bool movingRight_ = true;
    float screenLimit_ = 900.0f;
    std::optional<std::unique_ptr<Order>> order_;

public:
    static constexpr float START_X = 120.0f;
    static constexpr float DOCK_X = 20.0f;;
    static constexpr float DEFAULT_SPEED = 2.0f;

    Ship();
    Ship(float startX, float y, float speed);
    Ship(float startX, float y, float speed, std::unique_ptr<Order> order);

    void update(float station);

    bool isDocked() const noexcept;
    bool hasOrder() const noexcept;
    bool isActive() const noexcept;

    float getX() const noexcept;
    float getY() const noexcept;
    float getSpeed() const noexcept;

    std::string getOrderInfo() const;
    Order* getOrder() const noexcept;

    void setOrder(std::unique_ptr<Order> order);
    void clearOrder();
    int completeOrderAndGetReward();
};