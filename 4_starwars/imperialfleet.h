#ifndef STARWARS_IMPERIALFLEET_H
#define STARWARS_IMPERIALFLEET_H

#include "rebelfleet.h"

template<typename U>
class ImperialStarship {

    U shieldStrength;

    U attackStrength;

public:

    typedef U valueType;

    ImperialStarship(U _shieldStrength, U _attackStrength)
            : shieldStrength(_shieldStrength), attackStrength(_attackStrength) {
        assert(shieldStrength >= 0);
        assert(attackStrength >= 0);
    }

    U getShield() const {
        return shieldStrength;
    }

    U getAttackPower() const {
        return attackStrength;
    }

    void takeDamage(U damage) {
        if (damage > shieldStrength) {
            damage = shieldStrength;
        }
        shieldStrength -= damage;
    }
};

template<typename U>
using DeathStar = ImperialStarship<U>;

template<typename U>
using ImperialDestroyer = ImperialStarship<U>;

template<typename U>
using TIEFighter = ImperialStarship<U>;

template<typename U, bool canAttack, int minSpeed, int maxSpeed, typename I>
void attackIfPossible(RebelStarship<U, canAttack, minSpeed, maxSpeed>& rebelShip, I& imperialShip) {
    if constexpr (canAttack) {
        imperialShip.takeDamage(rebelShip.getAttackPower());
    }
}

template<typename I, typename R>
void attack(I& imperialShip, R& rebelShip) {
    bool rebAtt = rebelShip.getShield() > 0;

    if (imperialShip.getShield() > 0) {
        rebelShip.takeDamage(imperialShip.getAttackPower());
    }

    if (rebAtt) {
        attackIfPossible(rebelShip, imperialShip);
    }
}

#endif //STARWARS_IMPERIALFLEET_H
