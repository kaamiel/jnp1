// Opis testów
// grupa 1 test z zadania
// grupa 2 tworzenie i poprawność statków rebelianckich
// grupa 3 tworzenie i poprawność statków imperialnych
// grupa 4 tworzenie i poprawność bitwy
// grupa 5 sprawdzenie logiki tick i podstawowego atakowania
// grupa 6 poprawność atakowania z uwzględnieniem Squadron

#if TEST_NUM == 101

#include <cassert>
#include "imperialfleet.h"
#include "rebelfleet.h"
#include "battle.h"

int main() {
    auto xwing = createXWing(100, 300000, 50);
    auto explorer = createExplorer(150, 400000);
    auto cruiser = createStarCruiser(1234, 100000, 11);
    auto deathStar = createDeathStar(10000, 75);
    auto fighter = createTIEFighter(50, 9);
    auto destroyer = createImperialDestroyer(150, 20);
    auto squadron = createSquadron({deathStar, fighter});

    auto battle = SpaceBattle::Builder()
        .ship(squadron)
        .startTime(2)
        .maxTime(23)
        .ship(xwing)
        .ship(explorer)
        .build();

    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 2);

    battle.tick(3);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(4);
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // Wypisuje "IMPERIUM WON\n".

    return 0;
}

#else

#include "battle.h"
#include "rebelfleet.h"
#include "imperialfleet.h"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    static const int MIN_SPEED_CRUISER = 99'999;
    static const int MAX_SPEED_CRUISER = 299'795;
    (void)MIN_SPEED_CRUISER;
    (void)MAX_SPEED_CRUISER;
    static const int MIN_SPEED_XWING = 299'796;
    static const int MAX_SPEED_XWING = 2'997'960;
    (void)MIN_SPEED_XWING;
    (void)MAX_SPEED_XWING;
    static const int MIN_SPEED_EXPLORER = 299'796;
    static const int MAX_SPEED_EXPLORER = 2'997'960;
    (void)MIN_SPEED_EXPLORER;
    (void)MAX_SPEED_EXPLORER;

    #if TEST_NUM == 201
        auto ship = createXWing(100, MIN_SPEED_XWING, 10);
        assert(ship->getShield() == 100);
        assert(ship->getSpeed() == MIN_SPEED_XWING);
    #endif

    #if TEST_NUM == 202
        auto ship = createStarCruiser(100, MIN_SPEED_CRUISER, 10);
        assert(ship->getShield() == 100);
        assert(ship->getSpeed() == MIN_SPEED_CRUISER);
    #endif

    #if TEST_NUM == 203
        auto ship = createExplorer(100, MIN_SPEED_EXPLORER);
        assert(ship->getShield() == 100);
        assert(ship->getSpeed() == MIN_SPEED_EXPLORER);
    #endif

    #if TEST_NUM == 204
        auto ship = createXWing(100, MIN_SPEED_XWING, 10);
        ship->takeDamage(10);
        assert(ship->getShield() == 90);
    #endif

    #if TEST_NUM == 205
        // Nie powinno się wykonać (prędkość poniżej minimum).
        auto ship = createXWing(100, MIN_SPEED_XWING - 1, 10);
    #endif

    #if TEST_NUM == 206
        // Nie powinno się wykonać (prędkość powyżej maksimum).
        auto ship = createXWing(100, MAX_SPEED_XWING + 1, 10);
    #endif

    #if TEST_NUM == 207
        // Nie powinno się wykonać (prędkość poniżej minimum).
        auto ship = createExplorer(100, MIN_SPEED_EXPLORER - 1);
    #endif

    #if TEST_NUM == 208
        // Nie powinno się wykonać (prędkość powyżej maksimum).
        auto ship = createExplorer(100, MAX_SPEED_EXPLORER + 1);
    #endif

    #if TEST_NUM == 209
        // Nie powinno się wykonać (prędkość poniżej minimum).
        auto ship = createStarCruiser(100, MIN_SPEED_CRUISER - 1, 10);
    #endif

    #if TEST_NUM == 210
        // Nie powinno się wykonać (prędkość powyżej maksimum).
        auto ship = createStarCruiser(100, MAX_SPEED_CRUISER + 1, 10);
    #endif

    #if TEST_NUM == 211
        // Nie powinno się kompilować (brak getAttackPower).
        auto ship = createExplorer(100, MIN_SPEED_EXPLORER);
        ship->getAttackPower();
    #endif

    #if TEST_NUM == 212
        // Nie powinno się kompilować (brak metody fabrykującej 3-argumentowej).
        auto ship = createExplorer(100, MIN_SPEED_EXPLORER, 10);
    #endif

    #if TEST_NUM == 213
        // Nie powinno się kompilować (brak metody fabrykującej 2-argumentowej).
        auto ship = createXWing(100, MIN_SPEED_XWING);
    #endif

    #if TEST_NUM == 214
        // Nie powinno się kompilować (brak metody fabrykującej 2-argumentowej).
        auto ship = createStarCruiser(100, MIN_SPEED_CRUISER);
    #endif

    #if TEST_NUM == 301
        auto ship = createDeathStar(100, 10);
        assert(ship->getShield() == 100);
        assert(ship->getAttackPower() == 10);
    #endif

    #if TEST_NUM == 302
        auto ship = createTIEFighter(100, 10);
        assert(ship->getShield() == 100);
        assert(ship->getAttackPower() == 10);
    #endif

    #if TEST_NUM == 303
        auto ship = createImperialDestroyer(100, 10);
        assert(ship->getShield() == 100);
        assert(ship->getAttackPower() == 10);
    #endif

    #if TEST_NUM == 304
        auto ship = createSquadron({createDeathStar(100, 10), createTIEFighter(100, 10)});
        assert(ship->getShield() == 200);
        assert(ship->getAttackPower() == 20);
    #endif

    #if TEST_NUM == 305
        auto ship = createDeathStar(100, 10);
        ship->takeDamage(10);
        assert(ship->getShield() == 90);
    #endif

    #if TEST_NUM == 306
        // Nie powinno się kompilować (brak metody fabrykującej bezparametrowej).
        auto ship = createTIEFighter();
    #endif

    #if TEST_NUM == 307
        // Nie powinno się kompilować (brak metody fabrykującej bezparametrowej).
        auto ship = createDeathStar();
    #endif

    #if TEST_NUM == 308
        // Nie powinno się kompilować (brak metody fabrykującej bezargumentowej).
        auto ship = createImperialDestroyer();
    #endif

    #if TEST_NUM == 401
        auto deathstar = createDeathStar(100, 10);
        auto xwing = createXWing(100, MAX_SPEED_XWING, 35);
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(23)
                .ship(deathstar)
                .ship(xwing)
                .build();
        assert(battle.countRebelFleet() == 1);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 402
        auto xwing = createXWing(100, MAX_SPEED_XWING, 35);
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(23)
                .ship(xwing)
                .build();
        assert(battle.countRebelFleet() == 1);
        assert(battle.countImperialFleet() == 0);
    #endif

    #if TEST_NUM == 403
        auto deathstar = createDeathStar(100, 10);
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(23)
                .ship(deathstar)
                .build();
        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 404
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(23)
                .build();
        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 0);
    #endif

    #if TEST_NUM == 405
        // Nie powinno działać poprawnie (startTime > maxTime).
        auto deathstar = createDeathStar(100, 10);
        auto xwing = createXWing(100, MAX_SPEED_XWING, 35);
        auto battle = SpaceBattle::Builder()
                .startTime(24)
                .maxTime(23)
                .build();
    #endif

    #if TEST_NUM == 501 || TEST_NUM == 502 || TEST_NUM == 503
        streambuf* oldCoutStreamBuffer = cout.rdbuf();
        ostringstream capturedCout;
        cout.rdbuf(capturedCout.rdbuf());
    #endif

    #if TEST_NUM == 501
        auto deathstar = createDeathStar(100, 110);
        auto xwing = createXWing(100, MAX_SPEED_XWING, 10);
        auto battle = SpaceBattle::Builder()
                .startTime(1)
                .maxTime(3)
                .ship(deathstar)
                .ship(xwing)
                .build();
        battle.tick(1);
        battle.tick(1);
        battle.tick(1);

        assert(capturedCout.str() == "IMPERIUM WON\n" || capturedCout.str() == "IMPERIUM WON");
    #endif

    #if TEST_NUM == 502
        auto deathstar = createDeathStar(100, 10);
        auto xwing = createXWing(100, MAX_SPEED_XWING, 110);
        auto battle = SpaceBattle::Builder()
                .startTime(1)
                .maxTime(3)
                .ship(deathstar)
                .ship(xwing)
                .build();
        battle.tick(1);
        battle.tick(1);
        battle.tick(1);

        assert(capturedCout.str() == "REBELLION WON\n" || capturedCout.str() == "REBELLION WON");
    #endif

    #if TEST_NUM == 503
        auto deathstar = createDeathStar(100, 50);
        auto xwing = createXWing(100, MAX_SPEED_XWING, 50);
        auto battle = SpaceBattle::Builder()
                .startTime(1)
                .maxTime(3)
                .ship(deathstar)
                .ship(xwing)
                .build();
        battle.tick(1);
        battle.tick(1);
        battle.tick(1);
        battle.tick(1);

        assert(capturedCout.str() == "DRAW\n" || capturedCout.str() == "DRAW");
    #endif

    #if TEST_NUM == 501 || TEST_NUM == 502 || TEST_NUM == 503
        cout.rdbuf(oldCoutStreamBuffer);
    #endif

    #if TEST_NUM == 504
        auto deathstar = createDeathStar(100, 1);
        auto explorer = createExplorer(1, MAX_SPEED_EXPLORER);
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(4)
                .ship(deathstar)
                .ship(explorer)
                .build();
        battle.tick(1);

        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 505
        auto deathstar = createDeathStar(100, 1);
        auto explorer1 = createExplorer(1, MAX_SPEED_EXPLORER);
        auto explorer2 = createExplorer(2, MAX_SPEED_EXPLORER);
        auto explorer3 = createExplorer(3, MAX_SPEED_EXPLORER);
        auto explorer4 = createExplorer(4, MAX_SPEED_EXPLORER);
        auto explorer5 = createExplorer(5, MAX_SPEED_EXPLORER);
        auto explorer6 = createExplorer(6, MAX_SPEED_EXPLORER);
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(25)
                .ship(deathstar)
                .ship(explorer1)
                .ship(explorer2)
                .ship(explorer3)
                .ship(explorer4)
                .ship(explorer5)
                .ship(explorer6)
                .build();
        assert(battle.countRebelFleet() == 6);
        assert(battle.countImperialFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countRebelFleet() == 5);
        assert(battle.countImperialFleet() == 1);

        battle.tick(2); // 3 - atak

        assert(battle.countRebelFleet() == 4);
        assert(battle.countImperialFleet() == 1);

        battle.tick(1); // 5 - brak ataku

        assert(battle.countRebelFleet() == 4);
        assert(battle.countImperialFleet() == 1);

        battle.tick(1); // 6 - atak

        assert(battle.countRebelFleet() == 3);
        assert(battle.countImperialFleet() == 1);

        battle.tick(3); // 7 - brak ataku

        assert(battle.countRebelFleet() == 3);
        assert(battle.countImperialFleet() == 1);

        battle.tick(5); // 10 - brak ataku

        assert(battle.countRebelFleet() == 3);
        assert(battle.countImperialFleet() == 1);

        battle.tick(3); // 15 - brak ataku

        assert(battle.countRebelFleet() == 3);
        assert(battle.countImperialFleet() == 1);

        battle.tick(7); // 18 - atak

        assert(battle.countRebelFleet() == 2);
        assert(battle.countImperialFleet() == 1);

        battle.tick(3); // 25 - brak ataku

        assert(battle.countRebelFleet() == 2);
        assert(battle.countImperialFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countRebelFleet() == 1);
        assert(battle.countImperialFleet() == 1);

        battle.tick(1); // 3 - atak

        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 506
        auto deathstar = createDeathStar(100, 1);
        auto cruiser = createStarCruiser(2, MIN_SPEED_CRUISER, 1);
        auto battle = SpaceBattle::Builder()
                .startTime(0)
                .maxTime(2)
                .ship(deathstar)
                .ship(cruiser)
                .build();
        battle.tick(5);
        battle.tick(6);
        battle.tick(1);

        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 507
        auto deathstar = createDeathStar(100, 1);
        auto cruiser = createStarCruiser(2, MIN_SPEED_CRUISER, 1);
        auto battle = SpaceBattle::Builder()
                .startTime(0)
                .maxTime(2)
                .ship(deathstar)
                .ship(cruiser)
                .build();
        battle.tick(2);
        battle.tick(3);
        battle.tick(1);

        assert(battle.countRebelFleet() == 0);
        assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 601
        auto xwing = createXWing(1000, MAX_SPEED_XWING, 1);
        auto fighter1 = createTIEFighter(1, 1);
        auto fighter2 = createTIEFighter(2, 1);
        auto fighter3 = createTIEFighter(3, 1);
        auto fighter4 = createTIEFighter(4, 1);
        auto fighter5 = createTIEFighter(5, 1);
        auto fighter6 = createTIEFighter(6, 1);
        auto squadron = createSquadron({fighter1, fighter2, fighter3, fighter4, fighter5, fighter6});
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(25)
                .ship(xwing)
                .ship(squadron)
                .build();
        assert(battle.countImperialFleet() == 6);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countImperialFleet() == 5);
        assert(battle.countRebelFleet() == 1);

        battle.tick(2); // 3 - atak

        assert(battle.countImperialFleet() == 4);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 5 - brak ataku

        assert(battle.countImperialFleet() == 4);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 6 - atak

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(3); // 7 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(5); // 10 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(3); // 15 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(7); // 18 - atak

        assert(battle.countImperialFleet() == 2);
        assert(battle.countRebelFleet() == 1);

        battle.tick(3); // 25 - brak ataku

        assert(battle.countImperialFleet() == 2);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countImperialFleet() == 1);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 3 - atak

        assert(battle.countImperialFleet() == 0);
        assert(battle.countRebelFleet() == 1);
    #endif

    #if TEST_NUM == 602
        auto xwing = createXWing(1000, MAX_SPEED_XWING, 1);
        auto explorer = createExplorer(10, MAX_SPEED_EXPLORER);
        auto ship1 = createImperialDestroyer(1, 1);
        auto ship2 = createTIEFighter(2, 1);
        auto ship3 = createDeathStar(3, 1);
        auto ship4 = createTIEFighter(4, 1);
        auto ship5 = createTIEFighter(5, 1);
        auto ship6 = createImperialDestroyer(6, 1);
        auto squadron = createSquadron({ship1, ship2, ship3});
        auto battle = SpaceBattle::Builder()
                .startTime(2)
                .maxTime(25)
                .ship(xwing)
                .ship(squadron)
                .ship(ship4)
                .ship(ship5)
                .ship(ship6)
                .build();
        assert(battle.countImperialFleet() == 6);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countImperialFleet() == 5);
        assert(battle.countRebelFleet() == 1);

        battle.tick(2); // 3 - atak

        assert(battle.countImperialFleet() == 4);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 5 - brak ataku

        assert(battle.countImperialFleet() == 4);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 6 - atak

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(3); // 7 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 10 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(7); // 11 - brak ataku

        assert(battle.countImperialFleet() == 3);
        assert(battle.countRebelFleet() == 1);

        battle.tick(2); // 18 - atak

        assert(battle.countImperialFleet() == 2);
        assert(battle.countRebelFleet() == 1);

        battle.tick(8); // 20 - brak ataku

        assert(battle.countImperialFleet() == 2);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 2 - atak

        assert(battle.countImperialFleet() == 1);
        assert(battle.countRebelFleet() == 1);

        battle.tick(1); // 3 - atak

        assert(battle.countImperialFleet() == 0);
        assert(battle.countRebelFleet() == 1);
    #endif

    return 0;
}

#endif
