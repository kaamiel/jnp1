#include "battle.h"
#include "imperialfleet.h"
#include "rebelfleet.h"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

// Opis testów
// grupa 1 test z zadania
// grupa 2 tworzenie i poprawność statków rebelianckich
// grupa 3 tworzenie i poprawność statków imperialnych
// grupa 4 tworzenie i poprawność bitwy
// grupa 5 sprawdzenie logiki tick
// grupa 6 poprawność funkcji attack

using namespace std;

int main() {
    #if TEST_NUM == 101
    XWing<float> xwing(100.0f, 300000.0f, 50.0f);
    Explorer<int> explorer(150, 400000);
    StarCruiser<unsigned> cruiser(1234, 100000, 11);
    DeathStar<long> deathStar(10000, 75);
    TIEFighter<unsigned> fighter(50, 9);
    ImperialDestroyer<int> destroyer(150, 20);
    auto battle = SpaceBattle<short, 1, 23,
                              DeathStar<long>,
                              Explorer<int>,
                              TIEFighter<unsigned>,
                              XWing<float>>(deathStar,
                                            explorer,
                                            fighter,
                                            xwing);

    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 2);

    battle.tick(2);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1);
    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(4);
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);

    (void)cruiser;
    (void)destroyer;
    #endif

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
    XWing<int> ship1(100, MIN_SPEED_XWING, 10);
    assert(ship1.getShield() == 100);
    assert(ship1.getSpeed() == MIN_SPEED_XWING);
    assert(ship1.getAttackPower() == 10);

    XWing<double> ship2(100.0, (double)MIN_SPEED_XWING, 10.0);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getSpeed() == MIN_SPEED_XWING);
    assert(ship2.getAttackPower() == 10.0);
    #endif

    #if TEST_NUM == 202
    StarCruiser<int> ship1(100, MIN_SPEED_CRUISER, 10);
    assert(ship1.getShield() == 100);
    assert(ship1.getSpeed() == MIN_SPEED_CRUISER);
    assert(ship1.getAttackPower() == 10);

    StarCruiser<double> ship2(100.0, (double)MIN_SPEED_CRUISER, 10.0);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getSpeed() == MIN_SPEED_CRUISER);
    assert(ship2.getAttackPower() == 10.0);
    #endif

    #if TEST_NUM == 203
    Explorer<int> ship1(100, MIN_SPEED_EXPLORER);
    assert(ship1.getShield() == 100);
    assert(ship1.getSpeed() == MIN_SPEED_EXPLORER);

    Explorer<double> ship2(100.0, (double)MIN_SPEED_EXPLORER);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getSpeed() == MIN_SPEED_EXPLORER);
    #endif

    #if TEST_NUM == 204
    XWing<int> ship(100, MIN_SPEED_XWING, 10);
    ship.takeDamage(10);
    assert(ship.getShield() == 90);
    #endif

    #if TEST_NUM == 205
    // Nie powinno się kompilować (brak getAttackPower).
    Explorer<int> ship(100, MIN_SPEED_EXPLORER);
    ship.getAttackPower();
    #endif

    #if TEST_NUM == 206
    // Nie powinno się kompilować (brak konstruktora 3-argumentowego).
    Explorer<int> ship(100, MIN_SPEED_EXPLORER, 10);
    #endif

    #if TEST_NUM == 207
    // Nie powinno się kompilować (brak konstruktora 2-argumentowego).
    XWing<int> ship(100, MIN_SPEED_XWING);
    #endif

    #if TEST_NUM == 208
    // Nie powinno się kompilować (brak konstruktora 2-argumentowego).
    StarCruiser<int> ship(100, MIN_SPEED_CRUISER);
    #endif

    #if TEST_NUM == 209
    // Nie powinno się wykonać (prędkość poniżej minimum).
    XWing<int> ship(100, MIN_SPEED_XWING - 1, 10);
    #endif

    #if TEST_NUM == 210
    // Nie powinno się wykonać (prędkość powyżej maksimum).
    XWing<int> ship(100, MAX_SPEED_XWING + 1, 10);
    #endif

    #if TEST_NUM == 211
    // Nie powinno się wykonać (prędkość poniżej minimum).
    Explorer<int> ship(100, MIN_SPEED_EXPLORER - 1);
    #endif

    #if TEST_NUM == 212
    // Nie powinno się wykonać (prędkość powyżej maksimum).
    Explorer<int> ship(100, MAX_SPEED_EXPLORER + 1);
    #endif

    #if TEST_NUM == 213
    // Nie powinno się wykonać (prędkość poniżej minimum).
    StarCruiser<int> ship(100, MIN_SPEED_CRUISER - 1, 10);
    #endif

    #if TEST_NUM == 214
    // Nie powinno się wykonać (prędkość powyżej maksimum).
    StarCruiser<int> ship(100, MAX_SPEED_CRUISER + 1, 10);
    #endif

    #if TEST_NUM == 301
    DeathStar<int> ship1(100, 10);
    assert(ship1.getShield() == 100);
    assert(ship1.getAttackPower() == 10);

    DeathStar<double> ship2(100.0, 10.0);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getAttackPower() == 10.0);
    #endif

    #if TEST_NUM == 302
    TIEFighter<int> ship1(100, 10);
    assert(ship1.getShield() == 100);
    assert(ship1.getAttackPower() == 10);

    TIEFighter<double> ship2(100.0, 10.0);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getAttackPower() == 10.0);
    #endif

    #if TEST_NUM == 303
    ImperialDestroyer<int> ship1(100, 10);
    assert(ship1.getShield() == 100);
    assert(ship1.getAttackPower() == 10);

    ImperialDestroyer<double> ship2(100.0, 10.0);
    assert(ship2.getShield() == 100.0);
    assert(ship2.getAttackPower() == 10.0);
    #endif

    #if TEST_NUM == 304
    DeathStar<int> ship(100, 10);
    ship.takeDamage(10);
    assert(ship.getShield() == 90);
    #endif

    #if TEST_NUM == 305
    // Nie powinno się kompilować (brak konstruktora bezargumentowego).
    TIEFighter<int>();
    #endif

    #if TEST_NUM == 306
    // Nie powinno się kompilować (brak konstruktora bezargumentowego).
    DeathStar<int>();
    #endif

    #if TEST_NUM == 307
    // Nie powinno się kompilować (brak konstruktora bezargumentowego).
    ImperialDestroyer<int>();
    #endif

    #if TEST_NUM == 401
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    auto battle = SpaceBattle<int, 0, 23, DeathStar<int>, XWing<int>>(deathstar, xwing);
    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 402
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    auto battle = SpaceBattle<int, 0, 23, XWing<int>>(xwing);
    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 0);
    #endif

    #if TEST_NUM == 403
    DeathStar<int> deathstar(100, 10);
    auto battle = SpaceBattle<int, 0, 23, DeathStar<int>>(deathstar);
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 404
    auto battle = SpaceBattle<int, 0, 23>();
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 0);
    #endif

    #if TEST_NUM == 405
    // Nie powinno się kompilować (startTime > maxTime).
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    auto battle = SpaceBattle<int, 24, 23, DeathStar<int>, XWing<int>>(deathstar, xwing);
    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 406
    // Nie powinno się kompilować (maxTime niepoprawnego typu).
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    auto battle = SpaceBattle<int, 0, "23", DeathStar<int>, XWing<int>>(deathstar, xwing);
    #endif

    #if TEST_NUM == 407
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    // Nie powinno się kompilować (XWing jako parametr konstruktora niepoprawnego typu).
    auto battle = SpaceBattle<int, 0, 23, DeathStar<int>, XWing<float>>(deathstar, xwing);
    #endif

    #if TEST_NUM == 408
    // Nie powinno się kompilować (za dużo parametrów konstruktora).
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    SpaceBattle<int, 0, 23, DeathStar<int>, XWing<int>>(deathstar, xwing, xwing);
    #endif

    #if TEST_NUM == 409
    // Nie powinno się kompilować (za mało parametrów konstruktora).
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 35);
    SpaceBattle<int, 0, 23, DeathStar<int>, XWing<int>, XWing<int>>(deathstar, xwing);
    #endif


    #if TEST_NUM == 501 || TEST_NUM == 502 || TEST_NUM == 503
    streambuf* oldCoutStreamBuffer = cout.rdbuf();
    ostringstream capturedCout;
    cout.rdbuf(capturedCout.rdbuf());
    #endif

    #if TEST_NUM == 501
    DeathStar<int> deathstar(100, 110);
    XWing<int> xwing(100, MAX_SPEED_XWING, 10);
    auto battle = SpaceBattle<int, 1, 3, DeathStar<int>, XWing<int>>(deathstar, xwing);
    battle.tick(1);
    battle.tick(1);

    assert(capturedCout.str() == "IMPERIUM WON\n" || capturedCout.str() == "IMPERIUM WON");
    #endif

    #if TEST_NUM == 502
    DeathStar<int> deathstar(100, 10);
    XWing<int> xwing(100, MAX_SPEED_XWING, 110);
    auto battle = SpaceBattle<int, 1, 3, DeathStar<int>, XWing<int>>(deathstar, xwing);
    battle.tick(1);
    battle.tick(1);

    assert(capturedCout.str() == "REBELLION WON\n" || capturedCout.str() == "REBELLION WON");
    #endif

    #if TEST_NUM == 503
    DeathStar<int> deathstar(100, 50);
    XWing<int> xwing(100, MAX_SPEED_XWING, 50);
    auto battle = SpaceBattle<int, 1, 4, DeathStar<int>, XWing<int>>(deathstar, xwing);
    battle.tick(3);
    battle.tick(1);
    battle.tick(1);

    assert(capturedCout.str() == "DRAW\n" || capturedCout.str() == "DRAW");
    #endif

    #if TEST_NUM == 501 || TEST_NUM == 502 || TEST_NUM == 503
    cout.rdbuf(oldCoutStreamBuffer);
    #endif

    #if TEST_NUM == 504
    DeathStar<int> deathstar(100, 1);
    Explorer<int> explorer(1, MAX_SPEED_EXPLORER);
    auto battle = SpaceBattle<int, 1, 4, DeathStar<int>, Explorer<int>>(deathstar, explorer);
    battle.tick(1);

    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 505
    DeathStar<int> deathstar(100, 1);
    Explorer<int> explorer1(1, MAX_SPEED_EXPLORER);
    Explorer<int> explorer2(2, MAX_SPEED_EXPLORER);
    Explorer<int> explorer3(3, MAX_SPEED_EXPLORER);
    Explorer<int> explorer4(4, MAX_SPEED_EXPLORER);
    Explorer<int> explorer5(5, MAX_SPEED_EXPLORER);
    Explorer<int> explorer6(6, MAX_SPEED_EXPLORER);
    auto battle = SpaceBattle<int, 0, 25,
                              DeathStar<int>,
                              Explorer<int>,
                              Explorer<int>,
                              Explorer<int>,
                              Explorer<int>,
                              Explorer<int>,
                              Explorer<int>>(deathstar,
                                             explorer1,
                                             explorer2,
                                             explorer3,
                                             explorer4,
                                             explorer5,
                                             explorer6);
    assert(battle.countRebelFleet() == 6);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // 0 - atak

    assert(battle.countRebelFleet() == 5);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // 1 - atak

    assert(battle.countRebelFleet() == 4);
    assert(battle.countImperialFleet() == 1);

    battle.tick(2); // 2 - brak ataku

    assert(battle.countRebelFleet() == 4);
    assert(battle.countImperialFleet() == 1);

    battle.tick(2); // 4 - atak

    assert(battle.countRebelFleet() == 3);
    assert(battle.countImperialFleet() == 1);

    battle.tick(2); // 6 - brak ataku

    assert(battle.countRebelFleet() == 3);
    assert(battle.countImperialFleet() == 1);

    battle.tick(5); // 8 - brak ataku

    assert(battle.countRebelFleet() == 3);
    assert(battle.countImperialFleet() == 1);

    battle.tick(3); // 13 - brak ataku

    assert(battle.countRebelFleet() == 3);
    assert(battle.countImperialFleet() == 1);

    battle.tick(7); // 16 - atak

    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(2); // 23 - brak ataku

    assert(battle.countRebelFleet() == 2);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // 25 - atak

    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 1);

    battle.tick(1); // 0 - atak

    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 506
    DeathStar<int> deathstar(100, 1);
    StarCruiser<int> cruiser(2, MIN_SPEED_CRUISER, 1);
    auto battle = SpaceBattle<int, 0, 1, DeathStar<int>, StarCruiser<int>>(deathstar, cruiser);
    battle.tick(2);
    battle.tick(2);

    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 507
    DeathStar<int> deathstar(100, 1);
    StarCruiser<int> cruiser(2, MIN_SPEED_CRUISER, 1);
    auto battle = SpaceBattle<int, 0, 1, DeathStar<int>, StarCruiser<int>>(deathstar, cruiser);
    battle.tick(1);
    battle.tick(1);

    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 601
    auto deathstar = DeathStar<int>(100, 10);
    auto xwing = XWing<int>(100, MIN_SPEED_XWING + 1, 35);

    attack(deathstar, xwing);

    assert(deathstar.getShield() == 65);
    assert(deathstar.getAttackPower() == 10);
    assert(xwing.getShield() == 90);
    assert(xwing.getSpeed() == MIN_SPEED_XWING + 1);
    assert(xwing.getAttackPower() == 35);
    #endif

    #if TEST_NUM == 602
    auto deathstar = DeathStar<int>(100, 10);
    auto cruiser = StarCruiser<int>(100, MIN_SPEED_CRUISER + 1, 20);

    attack(deathstar, cruiser);

    assert(deathstar.getShield() == 80);
    assert(deathstar.getAttackPower() == 10);
    assert(cruiser.getShield() == 90);
    assert(cruiser.getSpeed() == MIN_SPEED_CRUISER + 1);
    assert(cruiser.getAttackPower() == 20);
    #endif

    #if TEST_NUM == 603
    auto deathstar = DeathStar<int>(100, 10);
    auto explorer = Explorer<int>(100, MIN_SPEED_EXPLORER + 1);

    attack(deathstar, explorer);

    assert(deathstar.getShield() == 100);
    assert(deathstar.getAttackPower() == 10);
    assert(explorer.getShield() == 90);
    assert(explorer.getSpeed() == MIN_SPEED_EXPLORER + 1);
    #endif

    #if TEST_NUM == 701
    auto battle = SpaceBattle<int, 0, 23, DeathStar<int>, XWing<int>>(DeathStar<int>(100, 10),
                                                                      XWing<int>(100, MAX_SPEED_XWING, 35));
    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 702
    auto battle = SpaceBattle<int, 0, 23, XWing<int>>(XWing<int>(100, MAX_SPEED_XWING, 35));
    assert(battle.countRebelFleet() == 1);
    assert(battle.countImperialFleet() == 0);
    #endif

    #if TEST_NUM == 703
    auto battle = SpaceBattle<int, 0, 23, DeathStar<int>>(DeathStar<int>(100, 10));
    assert(battle.countRebelFleet() == 0);
    assert(battle.countImperialFleet() == 1);
    #endif

    #if TEST_NUM == 704
    XWing<int> xwing(100, 300000, 50);
    ImperialDestroyer<int> destroyer(150, 20);

    clog << "XWing shield before battle " << xwing.getShield() << endl;
    clog << "ImperialDestroyer shield before battle " << destroyer.getShield() << endl;

    auto battle = SpaceBattle<short, 0, 5, ImperialDestroyer<int>, XWing<int>>(destroyer, xwing);
    battle.tick(1);
    battle.tick(3);

    clog << "XWing shield after battle " << xwing.getShield() << endl;
    clog << "ImperialDestroyer shield after battle " << destroyer.getShield() << endl;
    #endif

    return 0;
}
