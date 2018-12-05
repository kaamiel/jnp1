#include "wallet.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

// configuration options
static const bool showPassedTests = false;
static const bool showFailedTests = true;
static const bool showEmptyTests = false;
static const bool showTotalResuls = true;

typedef void TestFunc();

static int totalPoints = 0;
static int testCount;
static int passedCount;
static bool allTestsOk = true;

static void resetTestCounters() {
    testCount = 0;
    passedCount = 0;
}

static void printTestStats() {
    if (testCount == 0 && !showEmptyTests) {
        return;
    }

    string result;
    if (testCount == passedCount) {
        totalPoints++;
        result = "PASS";
    } else {
        allTestsOk = false;
        result = "FAIL";
    }
    cout << passedCount << "/" << testCount << "\t : " << result << endl;
}

static void printTotalResult() {
    cout << "---------------" << endl;
    cout << "Total points: " << totalPoints << endl;
}

#define check(condition) { \
    testCount++; \
    if (condition) { \
        passedCount++; \
        if (showPassedTests) { \
            cout << testCount << " passed" << endl; \
        } \
    } else { \
        if (showFailedTests) { \
            cout << testCount << " failed: " <<  #condition << endl; \
        } \
    } \
}

static const uint _UNITS_IN_B = 100000000;
static const int _B_LIMIT = 21 * 1000000L;

static void test1KnownByStudents() {
#if TEST_NUM == 101
    cout << __FUNCTION__ << endl;

    assert(Empty() == 0);
    assert(Empty() < Wallet(1));
    assert(Empty().getUnits() == 0);
    assert(Empty().opSize() == 1);
    assert(Empty()[0].getUnits() == 0);

    Wallet w1;
    assert(w1 == Empty());
    assert(w1.opSize() == 1);

    Wallet w2(1), w3(1);
    assert(w2.opSize() == 1);
    assert(w2.getUnits() == _UNITS_IN_B);

    assert(Wallet::fromBinary("100") == Wallet(4));
    assert(Wallet(" 1.2000 ") == Wallet("1,2"));

    assert(Wallet(2) + w2 == Wallet(3));
    assert(Wallet(1) + Wallet(2) == Wallet(3));
    assert(1 + 2 == Wallet(3));

    Wallet w4;
    assert(w4.opSize() == 1);

    w4 += 2;
    assert(w4.opSize() == 2);
    assert(w4[0] <= w4[1]);
    assert(w4[1].getUnits() == w4.getUnits());

    w4 *= 3;
    assert(w4.opSize() == 3);
    assert(w4[2].getUnits() == 6 * _UNITS_IN_B);

    assert(Wallet(move(w4)).opSize() == 4);
#endif
}

#if TEST_NUM == 201
static void test21TooMuchBStrConstructor() {
    try {
        Wallet("999999999999999999999999999999999999999999999999999999999999999999999999");
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}

static void test22TooMuchBFromBinary() {
    try {
        Wallet::fromBinary("100000000000000000000000000000000000000000000000000000000000");
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}
#endif

static void test2ConstrAndCmp() {
#if TEST_NUM == 201
    cout << __FUNCTION__ << endl;

    Wallet w1(2);
    Wallet w2 = Wallet(2);
    check(w1 == w2);

    check(Wallet() == Empty());

    check(0 == Empty());
    check(0 >= Empty());
    check(0 <= Empty());
    check(!(0 != Empty()));
    check(!(0 > Empty()));
    check(!(0 < Empty()));

    check(Empty() != 1);
    check(Empty() < 1);
    check(Empty() <= 1);
    check(!(Empty() == 1));
    check(!(Empty() > 1));
    check(!(Empty() >= 1));

    Wallet one(1);
    check(one == 1);

    Wallet w(Wallet(1));
    check(w == 1);

    check(Wallet::fromBinary("0") == Empty());
    check(Wallet::fromBinary("1") == 1);
    check(Wallet::fromBinary("11") == 3);
    check(Wallet::fromBinary("10000000000") == 1024);

    check(Wallet("0.00000001").getUnits() == 1);
    check(Wallet(" 0.00000001      ").getUnits() == 1);
    check(Wallet("99.99999999") < Wallet(100));
    check(Wallet("99.99999998") < Wallet("99.99999999"));
    check(Wallet("1,111") == Wallet("1.111"));
    check(Wallet("1.111000") == Wallet("1.111"));
    check(Wallet("9999999") == 9999999);

    check(Wallet(0) == 0);
    check(Wallet(1) == 1);
    check(Wallet(9999999) == 9999999);

    Wallet two(2);
    check(two == Wallet(2));
    check(two > Wallet(1));
    check(two < Wallet(3));

    test21TooMuchBStrConstructor();
    test22TooMuchBFromBinary();
#endif
}

static void test3OperationHistory() {
#if TEST_NUM == 301
    cout << __FUNCTION__ << endl;

    // correct balances
    Wallet w1;
    check(w1[0].getUnits() == 0);
    w1 += Wallet(1);
    check(w1[1].getUnits() == _UNITS_IN_B);

    // operation history is sorted
    Wallet w;
    w += Wallet(2);
    w *= 3;
    check(w.opSize() == 3);
    check(w[0] <= w[1]);
    check(w[1] <= w[2]);

    // history has correct time
    const int MS = 1000;
    for (int i = 0; i < 2; ++i) {
        Wallet w;
        usleep(5 * MS);
        w += 1;
        check(w.opSize() == 2);

        check(w[0] < w[1]);
        // additional checks
        check(w[0] <= w[1]);
        check(w[0] != w[1]);
        check(!(w[0] == w[1]));
        check(!(w[0] > w[1]));
        check(!(w[0] >= w[1]));
    }

    // operators and history
    {
        Wallet w, w1, w2{2};
        w = Wallet(2) + w2;
        check(w2.opSize() == 2 || w2.opSize() == 3);
        check(w2.getUnits() == 0);
        check(w.getUnits() == 4 * _UNITS_IN_B);
        w *= 5;
        check(w.getUnits() == 20 * _UNITS_IN_B);
        check((w * 2).getUnits() == 40 * _UNITS_IN_B);

        Wallet w3 = Wallet(1) + Wallet(2);
        check(w3.getUnits() == 3 * _UNITS_IN_B);
        assert(w3.opSize() == 2 || w3.opSize() == 3);

        Wallet w4 = Wallet(2) - Wallet(1);
        check(w4.getUnits() ==  _UNITS_IN_B);
        assert(w4.opSize() == 2 || w4.opSize() == 3);
    }

    // assignment to the same object -> do nothing
    {
        Wallet w;
        w += Wallet();
        check(w.opSize() == 2);
        w = move(w);
        check(w.opSize() == 2);
    }
#endif
}

#if TEST_NUM == 401
static void test41NegativeBalance1() {
    cout << __FUNCTION__ << endl;
    Wallet w1;
    try {
        w1 -= 1;
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}

static void test42NegativeBalance2() {
    cout << __FUNCTION__ << endl;
    try {
        Wallet() - 1;
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}

static void test43ToMuchMoney() {
    cout << __FUNCTION__ << endl;

    const int moreThanHalf = _B_LIMIT / 2 + 1;
    {
        Wallet w2(moreThanHalf);
    }
    // money should be returned here

    // no exception
    Wallet w2(moreThanHalf);

    try {
        Wallet w2(_B_LIMIT / 2);
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}

static void test44MoodleToMuchMoney() {
    cout << __FUNCTION__ << endl;
    Wallet w;
    const int btc = _B_LIMIT + 1;
    try {
        w += btc;
    } catch(...) {}
    check(w.getUnits() == 0);
}

static void test45MoodleNegativeBalance() {
    cout << __FUNCTION__ << endl;
    Wallet w(1);
    try {
        w -= 2;
    } catch(...) {}
    check(w.getUnits() == _UNITS_IN_B);
}

static void test46SmallWalletMultiplication() {
    cout << __FUNCTION__ << endl;
    Wallet w("0.00000001");
    uint nat = 1;
    uint units = 1;
    while (nat <= 1e9) {
        check((w * nat).getUnits() == units);
        nat *= 10;
        units *= 10;
    }
}
#endif

#if TEST_NUM == 402
static void test402SubtleBalanceOverflow1() {
    try {
        Wallet w1(1);
        w1 *= 0;
        Wallet w2(_B_LIMIT);
    } catch(...) {
        check(false);
        return;
    }
    check(true);
}

static void test402SubtleBalanceOverflow2() {
    try {
        constexpr int million = 1e6;
        Wallet w1(_B_LIMIT - million);
        w1 -= (million + 1);
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}

static void test402SubtleBalanceOverflow3() {
    try {
        constexpr int million = 1e6;
        Wallet w1(_B_LIMIT - million);
        [[maybe_unused]] volatile bool b = w1 < (million + 1);
    } catch(...) {
        check(true);
        return;
    }
    check(false);
}
#endif


static void test4Operations() {
#if TEST_NUM == 401
    cout << __FUNCTION__ << endl;

    {
        check(Wallet() + Wallet(3) == 3);
        check(Wallet(5) + Wallet(3) == Wallet(8));
        check(Wallet() + Wallet() == Empty());
    }

    {
        check(Wallet(5) - Wallet(3) == Wallet(2));
        check(Wallet(5) - Wallet(5) == Empty());
        check(Wallet() - Wallet() == Empty());
    }

    {
        Wallet w(5);
        check(w * 0 == 0);
        check(0 * w == 0);

        check(Empty() * 100 == Empty());
        check(100 * Empty() == Empty());
    }

    test41NegativeBalance1();
    test42NegativeBalance2();
    test43ToMuchMoney();
    test44MoodleToMuchMoney();
    test45MoodleNegativeBalance();
    test46SmallWalletMultiplication();
#endif
#if TEST_NUM == 402
    test402SubtleBalanceOverflow1();
    test402SubtleBalanceOverflow2();
    test402SubtleBalanceOverflow3();
#endif
}

#if TEST_NUM == 501
static string wallet2Str(const Wallet& wallet) {
    stringstream ss;
    ss << wallet;
    return ss.str();
}
#endif

#if TEST_NUM == 501
static string getCurrentDate() {
    time_t rawtime;
    time(&rawtime);
    tm *timeinfo = localtime(&rawtime);
    const int len = 11;
    char buffer[len];
    strftime(buffer, len, "%Y-%m-%d", timeinfo);
    return buffer;
}
#endif

static void test5Printing() {
#if TEST_NUM == 501
    cout << __FUNCTION__ << endl;

    cout << "Wallet printing" << endl;
    check(wallet2Str(Empty()) == "Wallet[0 B]");
    check(wallet2Str(Wallet(1)) == "Wallet[1 B]");
    check(wallet2Str(Wallet(999999)) == "Wallet[999999 B]");
    check(wallet2Str(Wallet("999,999")) == "Wallet[999,999 B]");
    check(wallet2Str(Wallet("999,0999")) == "Wallet[999,0999 B]");
    check(wallet2Str(Wallet("0,00000001")) == "Wallet[0,00000001 B]");

    cout << "Wallet operation printing" << endl;
    stringstream ss;
    ss << Wallet("1,00000001")[0];
    check(ss.str() == "Wallet balance is 1,00000001 B after operation made at day " + getCurrentDate());
#endif
}

static void test6Compilation() {
#if TEST_NUM >= 600 && TEST_NUM <= 700
    cout << __FUNCTION__ << endl;
#endif

    // should compile:
#if TEST_NUM == 601
    {
        const Wallet w(1);
        w.getUnits();
        check(true);
    }
#endif

#if TEST_NUM == 602
    {
        const Wallet w(1);
        w.opSize();
        check(true);
    }
#endif

#if TEST_NUM == 603
    {
        Wallet w(0);

        w = Wallet(2);
        w == 2;
        w >= 2;
        2 >= w;
        w <= 2;
        2 <= w;
        w > 1;
        1 < w;

        Wallet w1;
        w1 = 3;

        check(true);
    }
#endif

#if TEST_NUM == 604
    {
        Empty().getUnits();
        Empty().opSize();


        Wallet(1) + Wallet();
        Wallet(1) - Wallet();
        1 * Empty();
        Wallet w1{1}, w2{1};
        Wallet w3 = w1 * 42;
        Wallet w4 = 42 * w2;
        Wallet w5 = Wallet(1) * 42;
        Wallet w6 = 42 * Wallet(2);

        check(true);
    }
#endif

    // shouldn't compile
#if TEST_NUM == 605
    Wallet w(true);
    check(false);
#endif

#if TEST_NUM == 606
    Wallet w('a');
    check(false);
#endif

#if TEST_NUM == 607
    Wallet w(1.2);
    check(false);
#endif

#if TEST_NUM == 608
    Wallet w;
    w += "10";
    check(false);
#endif

#if TEST_NUM == 609
    Wallet w1, w2;
    w1 = w2 + "10";
    check(false);
#endif

#if TEST_NUM == 610
    Wallet w1, w2;
    f1 = "10" + f2;
    check(false);
#endif

#if TEST_NUM == 611
    Wallet w;
    w -= "10";
    check(false);
#endif

#if TEST_NUM == 612
    Wallet w1, w2;
    w1 = w2 - "10";
    check(false);
#endif

#if TEST_NUM == 613
    Wallet w1, w2;
    f1 = "10" - f2;
    check(false);
#endif

#if TEST_NUM == 614
    bool b;
    b = Empty() < true;
    check(false);
#endif

#if TEST_NUM == 615
    bool b;
    b = "1" <= Empty();
    check(false);
#endif

#if TEST_NUM == 616
    Empty() += Empty();
    check(false);
#endif

#if TEST_NUM == 617
    Empty() -= Empty();
    check(false);
#endif

#if TEST_NUM == 618
    Empty() *= 0;
    check(false);
#endif

#if TEST_NUM == 619
    Wallet(1) * Wallet(1);
    check(false);
#endif

#if TEST_NUM == 620
    Wallet w;
    w += 1;
    w[0] = w[1];
    check(false);
#endif

#if TEST_NUM == 621
    Wallet w1, w2;
    w1 = w2;
    check(false);
#endif
}

static void doTest(TestFunc* testFunc) {
    resetTestCounters();
    (*testFunc)();
    printTestStats();
}

int main() {
    array tests = {
            &test1KnownByStudents, &test2ConstrAndCmp, &test3OperationHistory,
            &test4Operations, &test5Printing, &test6Compilation,
    };

    for_each(tests.begin(), tests.end(), doTest);
    if (showTotalResuls) {
        printTotalResult();
    }

    return !allTestsOk;
}
