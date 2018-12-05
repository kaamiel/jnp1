#ifndef WALLET_WALLET_H
#define WALLET_WALLET_H

#include <string>
#include <stdexcept>
#include <utility>
#include <vector>
#include <chrono>

class Wallet {

public:

    // 1 coin = UNITS_IN_B units.
    static const unsigned UNITS_IN_B{100'000'000};

    // Hard limit for the amount of coins in existence.
    static const unsigned GLOBAL_B_LIMIT{21'000'000};
    static const auto GLOBAL_UNIT_LIMIT = static_cast<unsigned long long>(GLOBAL_B_LIMIT) * UNITS_IN_B;

    // Thrown if an operation would result in exceeding the GLOBAL_B_LIMIT.
    class global_limit_exceeded : public std::domain_error {
    public:
        explicit global_limit_exceeded(const std::string& message);
    };

    // Thrown if an operation would result in a Wallet's balance going negative.
    class insufficient_funds : std::domain_error {
    public:
        explicit insufficient_funds(const std::string& message);
    };

    // All ctors create zero or more new coins in the system. If coins creation would result in the GLOBAL_B_LIMIT
    // being exceeded, the global_limit_exceeded exception is thrown.

    // Creates an empty Wallet with an operation history containing one entry.
    Wallet();

    // Wallet with initial balance of `amount` B. Amount has to be non-negative.
    // Accepts an int instead of an unsigned to allow implicit usage as in wallet_example.cc.
    // The operation history of the created Wallet has one entry.
    Wallet(int amount);

    // Create a Wallet with initial balance equal to the value represented by the argument string. The string has
    // to conform to the CONSTRUCTOR_REGEX pattern. The operation history of the created Wallet has one entry.
    explicit Wallet(const char *amount);
    explicit Wallet(const std::string& amount);

    // Creates a Wallet with initial balance equal to the value represented by the argument string.
    // The value has to be in a binary, big-endian format. The operation history of the created Wallet has one entry.
    static Wallet fromBinary(const std::string& amount);

    // Coins cannot be copied.
    Wallet(const Wallet& other) = delete;
    Wallet& operator=(const Wallet& other) = delete;

    // Moves the coins and the operation history from the other Wallet.
    Wallet(Wallet&& other) noexcept;
    Wallet& operator=(Wallet&& other) noexcept;

    // Moves the coins and the operation histories from the other Wallets.
    // The operation history remains sorted by timestamps.
    Wallet(Wallet&& other1, Wallet&& other2);

    // No other ctors or implicit conversions are allowed.
    template<typename T>
    Wallet(T t) = delete;

    // Destroys the Wallet and all coins in it.
    ~Wallet();

    // Note: all +/- operators are mutating in respect to the rhs!
    // Consumes all coins in both Wallets and creates a new Wallet out of it as the result.
    friend Wallet operator+(Wallet&& lhs, Wallet& rhs);
    friend Wallet operator+(Wallet&& lhs, Wallet&& rhs);

    // Tries to transfer rhs.getUnits() coins from the lhs Wallet to the rhs Wallet and returns a new Wallet
    // with all coins that remained in lhs after the transfer.
    // Throws insufficient_funds if the balance would become negative.
    friend Wallet operator-(Wallet&& lhs, Wallet& rhs);
    friend Wallet operator-(Wallet&& lhs, Wallet&& rhs);

    // Consumes all coins in the other Wallet and transfers them to this Wallet.
    // Both this and other Wallet have one additional entry in the operation history.
    Wallet& operator+=(Wallet& other);
    Wallet& operator+=(Wallet&& other);

    // Tries to transfer other.getUnits() from this Wallet to other.
    // Throws insufficient_funds if the balance would become negative.
    // Both this and other Wallet have one additional entry in the operation history.
    Wallet& operator-=(Wallet& other);
    Wallet& operator-=(Wallet&& other);

    // Returns a new Wallet with n times wallet.getUnits() coins, effectively creating that many coins in the system,
    // thus it throws global_limit_exceeded, if the GLOBAL_B_LIMIT was exceeded.
    friend Wallet operator*(unsigned long long n, const Wallet& wallet);
    friend Wallet operator*(const Wallet& wallet, unsigned long long n);

    // Multiplies the amount of coins in this Wallet by n, effectively creating that many coins in the system,
    // thus it throws global_limit_exceeded, if the GLOBAL_B_LIMIT was exceeded.
    // Adds one entry to the operation history.
    Wallet& operator*=(unsigned long long n);

    // Comparison operators compare the amount of coins in both Wallets.
    friend bool operator==(const Wallet& lhs, const Wallet& rhs);
    friend bool operator!=(const Wallet& lhs, const Wallet& rhs);
    friend bool operator<(const Wallet& lhs, const Wallet& rhs);
    friend bool operator<=(const Wallet& lhs, const Wallet& rhs);
    friend bool operator>(const Wallet& lhs, const Wallet& rhs);
    friend bool operator>=(const Wallet& lhs, const Wallet& rhs);

    // Prints the Wallet onto the output stream in format "Wallet[b B]", where b is the amount of coins without
    // leading or trailing zeroes and with a comma as the decimal separator.
    friend std::ostream& operator<<(std::ostream& os, const Wallet& wallet);

    // Returns the amount of units currently in this Wallet.
    unsigned long long getUnits() const;

    // Returns the size of the operation history.
    size_t opSize() const;

    // Accesses the operation history.
    const class Operation& operator[](size_t index) const;

private:

    // Global counter for all coins in the system to check against the GLOBAL_B_LIMIT.
    inline static unsigned long long globalUnitAmount{0ULL};

    // Format of strings passed to the Wallet(const char*) and Wallet(std::string) ctors.
    inline static const char *const CONSTRUCTOR_REGEX{"^\\s*([0-9]{1,9})([.,]([0-9]{1,8}))?\\s*$"};

    // Amount of units currently in the Wallet.
    unsigned long long units{0ULL};

    // The operation history of the Wallet.
    std::vector<Operation> operations;

    // Internal ctor that creates a Wallet with a specified number of units in it.
    explicit Wallet(unsigned long long units);

    // Checks if creating unitsCreated new units would result in exceeding the GLOBAL_B_LIMIT and throws if yes.
    static void throwIfLimitExceeded(unsigned long long unitsCreated);
};

class Operation {

public:

    // Creates an operation with a default number of units equal to 0 and the creation time equal to now.
    Operation();

    // Creates an operation with a number of units and the creation time equal to now.
    Operation(unsigned long long units);

    // Creates an operation with a number of units and creation time equal to `units` and `timestamp` respectively.
    Operation(unsigned long long units, std::chrono::system_clock::time_point timestamp);

    // Returns the number of units in the Wallet after this operation.
    unsigned long long getUnits() const;

    // Comparison operators compare the creation time of this and other operation.
    bool operator==(const Operation& other) const;
    bool operator<(const Operation& other) const;

    // Prints the operation onto the output stream in format "Wallet units is b B after operation made at day d",
    // where b is the amount of coins after the operation without leading or trailing zeroes and with a comma
    // as the decimal separator and d is creation date in yyyy-mm-dd format.
    friend std::ostream& operator<<(std::ostream& os, const Operation& operation);

private:

    // Number of units in the Wallet after this operation.
    unsigned long long units{0ULL};

    // The time the operation was created.
    std::chrono::system_clock::time_point timestamp{std::chrono::system_clock::now()};

};

// Creates an empty wallet.
const Wallet& Empty();

// Auto-generate all other comparison operators for Operation.
using namespace std::rel_ops;

#endif //WALLET_WALLET_H
