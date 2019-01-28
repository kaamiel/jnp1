#ifndef STARWARS2_HELPER_H
#define STARWARS2_HELPER_H

#include <cstddef>

class Armed {
public:

    using AttackPower = int;

    virtual AttackPower getAttackPower() const = 0;

    virtual ~Armed() = default;

protected:

    Armed() = default;
};

class Shielded {
public:

    using ShieldPoints = int;

    virtual ShieldPoints getShield() const = 0;

    virtual ~Shielded() = default;

protected:

    Shielded() = default;
};

class Starship {
public:

    virtual void takeDamage(Armed::AttackPower damage) = 0;

    virtual size_t countAlive() const = 0;

    virtual ~Starship() = default;

protected:

    Starship() = default;
};

class ArmedStarship : public virtual Starship, public virtual Armed {
public:

    AttackPower getAttackPower() const override;

    ~ArmedStarship() override = default;

protected:

    explicit ArmedStarship(AttackPower attackPower);

private:

    AttackPower attackPower;
};

class ShieldedStarship : public virtual Starship, public virtual Shielded {
public:

    void takeDamage(Armed::AttackPower damage) override;

    size_t countAlive() const override;

    ShieldPoints getShield() const override;

    ~ShieldedStarship() override = default;

protected:

    explicit ShieldedStarship(ShieldPoints shieldPoints);

private:

    ShieldPoints shieldPoints;
};

#endif //STARWARS2_HELPER_H