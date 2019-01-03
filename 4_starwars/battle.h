#ifndef STARWARS_BATTLE_H
#define STARWARS_BATTLE_H

#include <cstddef>
#include <tuple>
#include <iostream>
#include <cassert>
#include "imperialfleet.h"
#include "rebelfleet.h"

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {

    static_assert(std::is_integral<T>::value, "Integral required.");

    static_assert(t0 <= t1);
    static_assert(t0 >= 0);

    std::tuple<S...> args;

    template<T N>
    struct Squares {
        static_assert(std::is_integral<T>::value, "Integral required.");

        static constexpr std::array<bool, N> getSquares() {
            std::array<bool, N> v{0};

            for (T n = 0; n * n < N; ++n) {
                v[n * n] = true;
            }

            return v;
        }

        static constexpr std::array<bool, N> arr = getSquares();

        static_assert(N > 0);
        static_assert(arr[0] == true);
    };

    Squares<t1> squares;

    T currentTime;

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    forEachImperial(std::tuple<Tp...>&) {}

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    forEachImperial(std::tuple<Tp...>& t) {
        tickShipImperial(std::get<I>(t));
        forEachImperial<I + 1, Tp...>(t);
    }

    template<typename U>
    inline void tickShipImperial(ImperialStarship<U>& ship) {
        if (ship.getShield() > 0) {
            forEachRebel(args, ship);
        }
    }

    template<typename U, bool canAttack, int minSpeed, int maxSpeed>
    inline void tickShipImperial(RebelStarship<U, canAttack, minSpeed, maxSpeed>&) {
    }

    template<std::size_t I = 0, typename... Tp, typename U>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    forEachRebel(std::tuple<Tp...>&, ImperialStarship<U>&) {}

    template<std::size_t I = 0, typename... Tp, typename U>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    forEachRebel(std::tuple<Tp...>& t, ImperialStarship<U>& imperialStarship) {
        tickShipRebel(std::get<I>(t), imperialStarship);
        forEachRebel<I + 1, Tp...>(t, imperialStarship);
    }

    template<typename U, typename U2>
    inline void tickShipRebel(ImperialStarship<U>&, ImperialStarship<U2>&) {
    }

    template<typename U, bool canAttack, int minSpeed, int maxSpeed, typename U2>
    inline void
    tickShipRebel(RebelStarship<U, canAttack, minSpeed, maxSpeed>& ship, ImperialStarship<U2>& imperialStarship) {
        if (ship.getShield() > 0) {
            attack(imperialStarship, ship);
        }
    }

    template<typename U>
    inline bool isImperial(ImperialStarship<U>& ship) {
        return ship.getShield() > 0;
    }

    template<typename U, bool canAttack, int minSpeed, int maxSpeed>
    inline bool isImperial(RebelStarship<U, canAttack, minSpeed, maxSpeed>&) {
        return false;
    }

    template<typename U>
    inline bool isRebel(ImperialStarship<U>&) {
        return false;
    }

    template<typename U, bool canAttack, int minSpeed, int maxSpeed>
    inline bool isRebel(RebelStarship<U, canAttack, minSpeed, maxSpeed>& ship) {
        return ship.getShield() > 0;
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), size_t>::type
    forEachCountImperial(std::tuple<Tp...>&) {
        return 0;
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), size_t>::type
    forEachCountImperial(std::tuple<Tp...>& t) {
        return (isImperial(std::get<I>(t)) ? 1 : 0) + forEachCountImperial<I + 1, Tp...>(t);
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), size_t>::type
    forEachCountRebel(std::tuple<Tp...>&) {
        return 0;
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), size_t>::type
    forEachCountRebel(std::tuple<Tp...>& t) {
        return (isRebel(std::get<I>(t)) ? 1 : 0) + forEachCountRebel<I + 1, Tp...>(t);
    }

public:

    SpaceBattle(S... args) : args(std::forward<S>(args)...), currentTime(t0) {
    }

    size_t countImperialFleet() {
        return forEachCountImperial(args);
    }

    size_t countRebelFleet() {
        return forEachCountRebel(args);
    }

    void tick(T timeStep) {
        assert(timeStep >= 0);

        if (currentTime == t1) {
            currentTime = 0;
        }

        size_t impCnt = countImperialFleet();
        size_t rebCnt = countRebelFleet();

        if (impCnt == 0 && rebCnt == 0) {
            std::cout << "DRAW\n";
            return;
        } else if (impCnt == 0) {
            std::cout << "REBELLION WON\n";
            return;
        } else if (rebCnt == 0) {
            std::cout << "IMPERIUM WON\n";
            return;
        }

        if (squares.arr[currentTime]) {
            forEachImperial(args);
        }

        currentTime = (currentTime + timeStep) % t1;
    }
};

#endif //STARWARS_BATTLE_H
