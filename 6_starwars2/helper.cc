#include "helper.h"

ArmedStarship::ArmedStarship(Armed::AttackPower attackPower) : attackPower(attackPower) {}

Armed::AttackPower ArmedStarship::getAttackPower() const {
    return attackPower;
}

ShieldedStarship::ShieldedStarship(Shielded::ShieldPoints shieldPoints) : shieldPoints(shieldPoints) {}

void ShieldedStarship::takeDamage(Armed::AttackPower damage) {
    shieldPoints = (shieldPoints < damage ? 0 : shieldPoints - damage);
}

size_t ShieldedStarship::countAlive() const {
    return (shieldPoints > 0 ? 1 : 0);
}

Shielded::ShieldPoints ShieldedStarship::getShield() const {
    return shieldPoints;
}
