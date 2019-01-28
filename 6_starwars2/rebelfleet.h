#ifndef STARWARS2_REBELFLEET_H
#define STARWARS2_REBELFLEET_H

#include <cassert>
#include <memory>
#include "helper.h"
#include "imperialfleet.h"

class ImperialFormation;

class RebelStarship : public virtual ShieldedStarship {
public:

    using Speed = int;

    Speed getSpeed() const;

    virtual void onAttack(ImperialFormation& imperialFormation) const;

    ~RebelStarship() override = default;

protected:

    RebelStarship(ShieldPoints shieldPoints, Speed speed);

private:

    Speed speed;
};

class ArmedRebelStarship : public virtual RebelStarship, public virtual ArmedStarship {
public:

    void onAttack(ImperialFormation& imperialFormation) const override;

    ~ArmedRebelStarship() override = default;

protected:

    ArmedRebelStarship(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower);
};

class Explorer : public RebelStarship {
public:

    Explorer(ShieldPoints shieldPoints, Speed speed);
};

class StarCruiser : public ArmedRebelStarship {
public:

    StarCruiser(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower);
};

class XWing : public ArmedRebelStarship {
public:

    XWing(ShieldPoints shieldPoints, Speed speed, AttackPower attackPower);
};

std::shared_ptr<RebelStarship>
createExplorer(Explorer::ShieldPoints shieldPoints, Explorer::Speed speed);

std::shared_ptr<RebelStarship>
createStarCruiser(StarCruiser::ShieldPoints shieldPoints, StarCruiser::Speed speed,
                  StarCruiser::AttackPower attackPower);

std::shared_ptr<RebelStarship>
createXWing(XWing::ShieldPoints shieldPoints, XWing::Speed speed, XWing::AttackPower attackPower);

#endif //STARWARS2_REBELFLEET_H
