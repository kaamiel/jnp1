#include <regex>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "wallet.h"

static std::string unitsToString(unsigned long long units);

Wallet::Wallet() {
    operations.emplace_back();
}

Wallet::Wallet(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Wallet(int) – amount cannot be negative.");
    }
    throwIfLimitExceeded(static_cast<unsigned long long>(UNITS_IN_B) * amount);

    units = static_cast<unsigned long long>(UNITS_IN_B) * amount;
    globalUnitAmount += units;

    operations.emplace_back(units);
}

Wallet::Wallet(const char *amount) : Wallet(std::string(amount)) {}

Wallet::Wallet(const std::string& amount) {
    static std::regex regex{CONSTRUCTOR_REGEX};

    std::smatch match;
    if (!std::regex_match(amount, match, regex)) {
        throw std::invalid_argument("Invalid string passed to Wallet(string): " + amount);
    }

    std::string stringBeforeDecimal = match[1];
    std::string stringAfterDecimal = match[3];

    auto tenToLength = 1;
    for (unsigned i = 0; i < stringAfterDecimal.length(); ++i) {
        tenToLength *= 10;
    }

    auto unitsBeforeDecimal = stoull(stringBeforeDecimal) * UNITS_IN_B;
    auto unitsAfterDecimal = (stringAfterDecimal.empty() ? 0 : stoull(stringAfterDecimal) * UNITS_IN_B / tenToLength);
    auto parsedUnits = unitsBeforeDecimal + unitsAfterDecimal;

    throwIfLimitExceeded(parsedUnits);

    units = parsedUnits;
    globalUnitAmount += units;

    operations.emplace_back(units);
}

Wallet::Wallet(Wallet&& other) noexcept : units(other.units), operations(std::move(other.operations)) {
    other.units = 0;

    operations.emplace_back(units);
}

Wallet::Wallet(Wallet&& other1, Wallet&& other2) : units(other1.units + other2.units) {
    other1.units = 0;
    other2.units = 0;

    operations = std::vector<Operation>();
    operations.reserve(other1.operations.size() + other2.operations.size());

    std::merge(other1.operations.begin(), other1.operations.end(),
               other2.operations.begin(), other2.operations.end(), std::back_inserter(operations));
    operations.emplace_back(units);
}

Wallet Wallet::fromBinary(const std::string& amount) {
    static const std::regex regex{"^[01]+$"};

    if (!std::regex_match(amount, regex)) {
        throw std::invalid_argument("Wallet::fromBinary: Binary amount " + amount + " is in a wrong format.");
    }

    try {
        return Wallet(std::stoi(amount, nullptr, 2));
    }
    catch (std::out_of_range& exception) {
        throw std::invalid_argument("Wallet::fromBinary: Binary amount " + amount + " is too large.");
    }
}

Wallet::~Wallet() {
    globalUnitAmount -= units;
}

const Wallet& Empty() {
    static const Wallet emptySingleton = Wallet();

    return emptySingleton;
}

Wallet& Wallet::operator=(Wallet&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->units = other.units;
    other.units = 0;

    this->operations = std::move(other.operations);
    this->operations.emplace_back(units);

    return *this;
}

Wallet& Wallet::operator+=(Wallet& other) {
    this->units += other.units;
    other.units = 0ULL;

    this->operations.emplace_back(this->units);
    other.operations.emplace_back(other.units);

    return *this;
}

Wallet& Wallet::operator+=(Wallet&& other) {
    this->units += other.units;
    other.units = 0;

    this->operations.emplace_back(this->units);

    return *this;
}

Wallet& Wallet::operator-=(Wallet& other) {
    if (*this < other) {
        throw insufficient_funds(
                "Cannot perform subtraction, as the left-hand-side Wallet has less funds that the right-hand-side.");
    }
    this->units -= other.units;
    other.units *= 2;

    this->operations.emplace_back(this->units);
    other.operations.emplace_back(other.units);

    return *this;
}

Wallet& Wallet::operator-=(Wallet&& other) {
    if (*this < other) {
        throw insufficient_funds(
                "Cannot perform subtraction, as the left-hand-side Wallet has less funds that the right-hand-side.");
    }
    this->units -= other.units;
    other.units *= 2;

    this->operations.emplace_back(this->units);

    return *this;
}

Wallet& Wallet::operator*=(unsigned long long n) {
    if (n == 0) {
        globalUnitAmount -= this->units;
        this->units = 0;

        this->operations.emplace_back(0);
        return *this;
    }
    if (n == 1) {
        this->operations.emplace_back(this->units);
        return *this;
    }
    if (this->units > (GLOBAL_UNIT_LIMIT - globalUnitAmount) / (n - 1)) {
        throw global_limit_exceeded("Global B limit would be exceeded when multiplying " +
                                    std::to_string(this->units) + " by " + std::to_string(n));
    }

    globalUnitAmount += this->units * (n - 1);
    this->units *= n;

    this->operations.emplace_back(this->units);

    return *this;
}

Wallet operator+(Wallet&& lhs, Wallet& rhs) {
    Wallet wallet;
    wallet += (lhs += rhs);

    return wallet;
}

Wallet operator+(Wallet&& lhs, Wallet&& rhs) {
    Wallet wallet;
    wallet += (std::move(lhs) += std::move(rhs));

    return wallet;
}

Wallet operator-(Wallet&& lhs, Wallet& rhs) {
    Wallet wallet;
    wallet += (lhs -= rhs);

    return wallet;
}

Wallet operator-(Wallet&& lhs, Wallet&& rhs) {
    Wallet wallet;
    wallet += (std::move(lhs) -= std::move(rhs));

    return wallet;
}

Wallet operator*(const Wallet& wallet, unsigned long long n) {
    if (n == 0) {
        return Wallet{};
    }
    Wallet newWallet{wallet.units};
    newWallet *= n;

    return newWallet;
}

Wallet operator*(unsigned long long n, const Wallet& wallet) {
    return wallet * n;
}

bool operator==(const Wallet& lhs, const Wallet& rhs) {
    return lhs.units == rhs.units;
}

bool operator!=(const Wallet& lhs, const Wallet& rhs) {
    return lhs.units != rhs.units;
}

bool operator<(const Wallet& lhs, const Wallet& rhs) {
    return lhs.units < rhs.units;
}

bool operator<=(const Wallet& lhs, const Wallet& rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>(const Wallet& lhs, const Wallet& rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const Wallet& lhs, const Wallet& rhs) {
    return lhs > rhs || lhs == rhs;
}

std::ostream& operator<<(std::ostream& os, const Wallet& wallet) {
    return os << "Wallet[" << unitsToString(wallet.units) << " B]";
}

unsigned long long Wallet::getUnits() const {
    return units;
}

size_t Wallet::opSize() const {
    return operations.size();
}

const Operation& Wallet::operator[](size_t index) const {
    return operations.at(index);
}

Wallet::Wallet(unsigned long long units) {
    Wallet::throwIfLimitExceeded(units);

    this->units = units;
    globalUnitAmount += units;

    this->operations.emplace_back(units);
}

void Wallet::throwIfLimitExceeded(unsigned long long unitsCreated) {
    if (unitsCreated > GLOBAL_UNIT_LIMIT - globalUnitAmount) {
        throw global_limit_exceeded("Global B limit would be exceeded by creating " +
                                    std::to_string(unitsCreated) + " units.");
    }
}

Wallet::global_limit_exceeded::global_limit_exceeded(const std::string& message) : domain_error(message) {}

Wallet::insufficient_funds::insufficient_funds(const std::string& message) : domain_error(message) {}

using std::chrono::system_clock;

Operation::Operation() : Operation(0, system_clock::now()) {}

Operation::Operation(unsigned long long units) : Operation(units, system_clock::now()) {}

Operation::Operation(unsigned long long units, system_clock::time_point timestamp) :
        units(units), timestamp(std::chrono::time_point_cast<std::chrono::milliseconds>(timestamp)) {}

unsigned long long Operation::getUnits() const {
    return units;
}

bool Operation::operator==(const Operation& other) const {
    return timestamp == other.timestamp;
}

bool Operation::operator<(const Operation& other) const {
    return timestamp < other.timestamp;
}

std::ostream& operator<<(std::ostream& os, const Operation& operation) {
    static const size_t bufferSize = 16;
    static char dateBuffer[bufferSize];

    time_t time = system_clock::to_time_t(operation.timestamp);

    std::strftime(dateBuffer, bufferSize, "%Y-%m-%d", localtime(&time));

    return os << "Wallet balance is " << unitsToString(operation.units) << " B after operation made at day "
              << dateBuffer;
}

static std::string unitsToString(unsigned long long units) {
    auto beforeDecimal = units / Wallet::UNITS_IN_B;
    auto afterDecimal = units - (beforeDecimal * Wallet::UNITS_IN_B);

    if (afterDecimal == 0) {
        return std::to_string(beforeDecimal);
    }

    auto afterDecimalString = std::to_string(afterDecimal);

    static const size_t afterDecimalMaxSize = 8;
    std::string fill(afterDecimalMaxSize - afterDecimalString.length(), '0');

    while (afterDecimalString.back() == '0') {
        afterDecimalString.pop_back();
    }

    return std::to_string(beforeDecimal) + "," + fill + afterDecimalString;
}