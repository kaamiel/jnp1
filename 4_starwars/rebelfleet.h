#ifndef STARWARS_REBELFLEET_H
#define STARWARS_REBELFLEET_H

#include <cassert>
#include <type_traits>

template<typename U, bool canAttack, int minSpeed, int maxSpeed>
class RebelStarship {

    U shieldStrength;

    U speed;

    U attackStrength;

public:

    typedef U valueType;

    template<typename = typename std::enable_if<canAttack, U>>
    RebelStarship(const U _shieldStrength, const U _speed, const U _attackStrength)
            : shieldStrength(_shieldStrength), speed(_speed), attackStrength(_attackStrength) {
        assert(minSpeed <= speed && speed <= maxSpeed);
        assert(shieldStrength >= 0);
        assert(attackStrength >= 0);
    }

    template<typename = typename std::enable_if<!canAttack, U>>
    RebelStarship(const U _shieldStrength, const U _speed)
            : shieldStrength(_shieldStrength), speed(_speed), attackStrength(0) {
        assert(minSpeed <= speed && speed <= maxSpeed);
        assert(shieldStrength >= 0);
        assert(attackStrength >= 0);
    }

    U getShield() const {
        return shieldStrength;
    }

    U getSpeed() const {
        return speed;
    }

    void takeDamage(U damage) {
        if (damage > shieldStrength) {
            damage = shieldStrength;
        }
        shieldStrength -= damage;
    }

    template<bool enabled = canAttack, typename = typename std::enable_if_t<enabled, U>>
    U getAttackPower() const {
        return attackStrength;
    }
};

template<typename U>
using Explorer = RebelStarship<U, false, 299796, 2997960>;

template<typename U>
using StarCruiser = RebelStarship<U, true, 99999, 299795>;

template<typename U>
using XWing = RebelStarship<U, true, 299796, 2997960>;

#endif //STARWARS_REBELFLEET_H