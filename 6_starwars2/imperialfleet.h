#ifndef STARWARS2_IMPERIALFLEET_H
#define STARWARS2_IMPERIALFLEET_H

#include <vector>
#include <memory>
#include "helper.h"
#include "rebelfleet.h"

class RebelStarship;

class ImperialFormation : public virtual Starship, public virtual Shielded, public virtual Armed {
public:

    void attack(RebelStarship& rebelStarship);

    ~ImperialFormation() override = default;

protected:

    ImperialFormation() = default;
};

class ImperialStarship
        : public virtual ImperialFormation, public virtual ShieldedStarship, public virtual ArmedStarship {
public:

    ~ImperialStarship() override = default;

protected:

    ImperialStarship(ShieldPoints shieldPoints, AttackPower attackPower);
};

class Squadron : public ImperialFormation {
public:

    explicit Squadron(const std::vector<std::shared_ptr<ImperialFormation>>& ships);

    Squadron(std::initializer_list<std::shared_ptr<ImperialFormation>> ships);

    void takeDamage(AttackPower damage) override;

    ShieldPoints getShield() const override;

    AttackPower getAttackPower() const override;

    size_t countAlive() const override;

private:

    std::vector<std::shared_ptr<ImperialFormation>> ships;
};

class DeathStar : public ImperialStarship {
public:

    DeathStar(ShieldPoints shieldPoints, AttackPower attackPower);
};

class ImperialDestroyer : public ImperialStarship {
public:

    ImperialDestroyer(ShieldPoints shieldPoints, AttackPower attackPower);
};

class TIEFighter : public ImperialStarship {
public:

    TIEFighter(ShieldPoints shieldPoints, AttackPower attackPower);
};

std::shared_ptr<ImperialFormation>
createDeathStar(DeathStar::ShieldPoints shieldPoints, DeathStar::AttackPower attackPower);

std::shared_ptr<ImperialFormation>
createImperialDestroyer(ImperialDestroyer::ShieldPoints shieldPoints, ImperialDestroyer::AttackPower attackPower);

std::shared_ptr<ImperialFormation>
createTIEFighter(TIEFighter::ShieldPoints shieldPoints, TIEFighter::AttackPower attackPower);

std::shared_ptr<ImperialFormation> createSquadron(const std::vector<std::shared_ptr<ImperialFormation>>& ships);

std::shared_ptr<ImperialFormation> createSquadron(std::initializer_list<std::shared_ptr<ImperialFormation>> ships);

#endif //STARWARS2_IMPERIALFLEET_H
