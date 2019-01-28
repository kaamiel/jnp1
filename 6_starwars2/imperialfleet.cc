#include "imperialfleet.h"

void ImperialFormation::attack(RebelStarship& rebelStarship) {
    rebelStarship.takeDamage(getAttackPower());
    rebelStarship.onAttack(*this);
}

ImperialStarship::ImperialStarship(ShieldPoints shieldPoints, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), ArmedStarship(attackPower) {}

Squadron::Squadron(const std::vector<std::shared_ptr<ImperialFormation>>& ships) : ships(ships) {}

Squadron::Squadron(std::initializer_list<std::shared_ptr<ImperialFormation>> ships)
        : ships(ships.begin(), ships.end()) {}

void Squadron::takeDamage(Armed::AttackPower damage) {
    for (auto& ship : ships) {
        ship->takeDamage(damage);
    }
}

Squadron::ShieldPoints Squadron::getShield() const {
    Squadron::ShieldPoints result = 0;
    for (auto& ship : ships) {
        result += ship->getShield();
    }
    return result;
}

Squadron::AttackPower Squadron::getAttackPower() const {
    Squadron::AttackPower result = 0;
    for (auto& ship : ships) {
        if (ship->getShield() > 0) {
            result += ship->getAttackPower();
        }
    }
    return result;
}

size_t Squadron::countAlive() const {
    size_t result = 0;
    for (auto& ship : ships) {
        result += ship->countAlive();
    }
    return result;
}

DeathStar::DeathStar(ShieldPoints shieldPoints, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), ArmedStarship(attackPower), ImperialStarship(shieldPoints, attackPower) {}

ImperialDestroyer::ImperialDestroyer(ShieldPoints shieldPoints, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), ArmedStarship(attackPower), ImperialStarship(shieldPoints, attackPower) {}

TIEFighter::TIEFighter(ShieldPoints shieldPoints, AttackPower attackPower)
        : ShieldedStarship(shieldPoints), ArmedStarship(attackPower), ImperialStarship(shieldPoints, attackPower) {}

std::shared_ptr<ImperialFormation>
createDeathStar(DeathStar::ShieldPoints shieldPoints, DeathStar::AttackPower attackPower) {
    return std::make_shared<DeathStar>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialFormation>
createImperialDestroyer(ImperialDestroyer::ShieldPoints shieldPoints, ImperialDestroyer::AttackPower attackPower) {
    return std::make_shared<ImperialDestroyer>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialFormation>
createTIEFighter(TIEFighter::ShieldPoints shieldPoints, TIEFighter::AttackPower attackPower) {
    return std::make_shared<TIEFighter>(shieldPoints, attackPower);
}

std::shared_ptr<ImperialFormation> createSquadron(const std::vector<std::shared_ptr<ImperialFormation>>& ships) {
    return std::make_shared<Squadron>(ships);
}

std::shared_ptr<ImperialFormation> createSquadron(std::initializer_list<std::shared_ptr<ImperialFormation>> ships) {
    return std::make_shared<Squadron>(ships);
}
