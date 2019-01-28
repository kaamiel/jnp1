#include "rebelfleet.h"

RebelStarship::RebelStarship(ShieldPoints shieldPoints, Speed speed)
        : ShieldedStarship(shieldPoints), speed(speed) {}

RebelStarship::Speed RebelStarship::getSpeed() const {
    return speed;
}

void RebelStarship::onAttack(ImperialFormation&) const {}

ArmedRebelStarship::ArmedRebelStarship(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), RebelStarship(shieldPoints, speed), ArmedStarship(attackPower) {}

void ArmedRebelStarship::onAttack(ImperialFormation& imperialFormation) const {
    imperialFormation.takeDamage(getAttackPower());
}

Explorer::Explorer(ShieldPoints shieldPoints, Speed speed)
        : ShieldedStarship(shieldPoints), RebelStarship(shieldPoints, speed) {
    assert(299796 <= speed && speed <= 2997960);
}

StarCruiser::StarCruiser(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), RebelStarship(shieldPoints, speed), ArmedStarship(attackPower),
          ArmedRebelStarship(shieldPoints, speed, attackPower) {
    assert(99999 <= speed && speed <= 299795);
}

XWing::XWing(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), RebelStarship(shieldPoints, speed), ArmedStarship(attackPower),
          ArmedRebelStarship(shieldPoints, speed, attackPower) {
    assert(299796 <= speed && speed <= 2997960);
}

std::shared_ptr<RebelStarship>
createExplorer(Explorer::ShieldPoints shieldPoints, Explorer::Speed speed) {
    return std::make_shared<Explorer>(shieldPoints, speed);
}

std::shared_ptr<RebelStarship>
createStarCruiser(StarCruiser::ShieldPoints shieldPoints, StarCruiser::Speed speed,
                  StarCruiser::AttackPower attackPower) {
    return std::make_shared<StarCruiser>(shieldPoints, speed, attackPower);
}

std::shared_ptr<RebelStarship>
createXWing(XWing::ShieldPoints shieldPoints, XWing::Speed speed, XWing::AttackPower attackPower) {
    return std::make_shared<XWing>(shieldPoints, speed, attackPower);
}
