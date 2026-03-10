#pragma once

#include "InformationPlayer.h"
#include <string>


class Appliance {
protected:
    Appliance() = default;

    int level_ = 1;
    std::string name_;
    Position position_;
public:
    virtual ~Appliance() = default;

    virtual std::string getType() const = 0;
    virtual bool produce(InformationPlayer& player) = 0;
    virtual int getUpgradeCost() const = 0;

    virtual void upgrade();

    int getLevel() const { return level_; }
    std::string getName() const { return name_; }
    Position getPosition() const { return position_; }

    bool canInteract(float playerX, float playerY, float distance = 2.0f) const {
        return position_.isNearby(playerX, playerY, distance);
    }

};