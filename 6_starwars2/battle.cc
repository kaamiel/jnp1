#include <iostream>
#include "battle.h"

Clock::Clock(Clock::Time startTime, Clock::Time maxTime)
        : currentTime(startTime), startTime(startTime), maxTime(maxTime) {
    assert(0 <= startTime);
    assert(startTime <= maxTime);
}

bool Clock::attackTime() const {
    return (currentTime % 2 == 0 || currentTime % 3 == 0) && (currentTime % 5 != 0);
}

bool Clock::tick(Clock::Time timeStep) {
    bool result = attackTime();
    currentTime = (currentTime + timeStep) % (maxTime + 1);
    return result;
}

SpaceBattle::SpaceBattle(const std::vector<std::shared_ptr<ImperialFormation>>& imperialFormations,
                         const std::vector<std::shared_ptr<RebelStarship>>& rebelStarships, const Clock& clock)
        : imperialFormations(imperialFormations), rebelStarships(rebelStarships), clock(clock) {}

size_t SpaceBattle::countImperialFleet() const {
    size_t result = 0;
    for (auto& imperialFormation : imperialFormations) {
        result += imperialFormation->countAlive();
    }
    return result;
}

size_t SpaceBattle::countRebelFleet() const {
    size_t result = 0;
    for (auto& rebelStarship : rebelStarships) {
        result += rebelStarship->countAlive();
    }
    return result;
}

void SpaceBattle::tick(Clock::Time timeStep) {
    if (countImperialFleet() == 0) {
        if (countRebelFleet() == 0) {
            std::cout << "DRAW\n";
        } else {
            std::cout << "REBELLION WON\n";
        }
    } else if (countRebelFleet() == 0) {
        std::cout << "IMPERIUM WON\n";
    } else {
        if (clock.tick(timeStep)) {
            for (auto& imperialFormation : imperialFormations) {
                for (auto& rebelStarship : rebelStarships) {
                    if (imperialFormation->getShield() > 0 && rebelStarship->getShield() > 0) {
                        imperialFormation->attack(*rebelStarship);
                    }
                }
            }
        }
    }
}

SpaceBattle::Builder& SpaceBattle::Builder::ship(std::shared_ptr<ImperialFormation> imperialFormation) {
    imperialFormations.push_back(imperialFormation);
    return *this;
}

SpaceBattle::Builder& SpaceBattle::Builder::ship(std::shared_ptr<RebelStarship> rebelStarship) {
    rebelStarships.push_back(rebelStarship);
    return *this;
}

SpaceBattle::Builder& SpaceBattle::Builder::startTime(Clock::Time t0) {
    startTimeVal = t0;
    return *this;
}

SpaceBattle::Builder& SpaceBattle::Builder::maxTime(Clock::Time t1) {
    maxTimeVal = t1;
    return *this;
}

SpaceBattle SpaceBattle::Builder::build() const {
    return SpaceBattle(imperialFormations, rebelStarships, Clock(startTimeVal, maxTimeVal));
}
