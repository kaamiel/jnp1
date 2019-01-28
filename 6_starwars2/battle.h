#ifndef STARWARS2_BATTLE_H
#define STARWARS2_BATTLE_H

#include <vector>
#include "rebelfleet.h"
#include "imperialfleet.h"
#include "helper.h"

class Clock {
public:

    using Time = int;

    Clock(Time startTime, Time maxTime);

    bool attackTime() const;

    bool tick(Clock::Time timeStep);

private:

    Time currentTime;

    Time startTime;

    Time maxTime;
};

class SpaceBattle {
public:

    class Builder;

    SpaceBattle(const std::vector<std::shared_ptr<ImperialFormation>>& imperialFormations,
                const std::vector<std::shared_ptr<RebelStarship>>& rebelStarships, const Clock& clock);

    size_t countImperialFleet() const;

    size_t countRebelFleet() const;

    void tick(Clock::Time timeStep);

private:

    std::vector<std::shared_ptr<ImperialFormation>> imperialFormations;

    std::vector<std::shared_ptr<RebelStarship>> rebelStarships;

    Clock clock;
};

class SpaceBattle::Builder {
public:

    Builder() = default;

    Builder& ship(std::shared_ptr<ImperialFormation> imperialFormation);

    Builder& ship(std::shared_ptr<RebelStarship> rebelStarship);

    Builder& startTime(Clock::Time t0);

    Builder& maxTime(Clock::Time t1);

    SpaceBattle build() const;

private:

    std::vector<std::shared_ptr<ImperialFormation>> imperialFormations;

    std::vector<std::shared_ptr<RebelStarship>> rebelStarships;

    Clock::Time startTimeVal;

    Clock::Time maxTimeVal;
};

#endif //STARWARS2_BATTLE_H