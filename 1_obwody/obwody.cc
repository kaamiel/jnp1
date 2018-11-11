/**
 * authors: Piotr Krzywicki, Kamil Dubil
 * date: 16.10.2018
 */

#include <utility>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <set>
#include <unordered_set>

namespace {
    using namespace std;

    // Definitions of Element related types and constants
    inline namespace ElementTraits {
        // Types
        using ElementId = string;
        using Description = string;
        using Node = int;
        using Nodes = vector<Node>;
        using ElementNo = int;

        enum class ElementType {
            TRANSISTOR = 0,
            DIODE,
            RESISTOR,
            CAPACITOR,
            VOLTAGE_SOURCE,
        };

        using Element = tuple<ElementType, ElementId, Description, Nodes>;

        // "Constants"
        static const unordered_map<ElementType, size_t> nodesMapping {
                {ElementType::TRANSISTOR,     3},
                {ElementType::DIODE,          2},
                {ElementType::RESISTOR,       2},
                {ElementType::CAPACITOR,      2},
                {ElementType::VOLTAGE_SOURCE, 2},
        };

        static const unordered_map<ElementId::value_type, ElementType> typesMapping {
                {'T', ElementType::TRANSISTOR},
                {'D', ElementType::DIODE},
                {'R', ElementType::RESISTOR},
                {'C', ElementType::CAPACITOR},
                {'E', ElementType::VOLTAGE_SOURCE},
        };

        static const unordered_map<ElementType, ElementId::value_type> inverseTypesMapping {
                {ElementType::TRANSISTOR,     'T'},
                {ElementType::DIODE,          'D'},
                {ElementType::RESISTOR,       'R'},
                {ElementType::CAPACITOR,      'C'},
                {ElementType::VOLTAGE_SOURCE, 'E'},
        };
    }

    // Basic Joins related types and constants
    inline namespace JoinTraits {
        using JoinId = int;
        constexpr JoinId groundId {0};
    }

    inline namespace Output {
        void error(const string &line, int lineNumber) {
            cerr << "Error in line " << lineNumber << ": " << line << endl;
        }

        // Print, separating values, with no separator after last element
        template<typename C, typename P>
        void printSeparating(const C &container, P &printer, const string &append = "") {
            static const char *separator = ", ";
            for (auto it = container.begin(); next(it) != container.end(); ++it) {
                printer << append << *it << separator;
            }
            printer << append << *container.rbegin();
        }
    }

    inline namespace Parser {
        Element parseEntry(const string &line, unordered_set<ElementId> &ids) {
            // Regex used to parse lines
            static std::string regexCommon = R"((?:0|[1-9][0-9]{0,8}))[\s]+([A-Z0-9][a-zA-Z0-9,\-\/]*)((?:[\s]+(?:0|[1-9][0-9]{0,8})))";
            static std::string notTransistor = R"(^[\s]*([DRCE])" + regexCommon + R"({2})[\s]*$)";
            static std::string transistor = R"(^[\s]*([T])" + regexCommon + R"({3})[\s]*$)";
            static regex notTransistorRetrieve(notTransistor);
            static regex transistorRetrieve(transistor);

            // Actual matching
            smatch fields;
            if (!regex_search(line, fields, transistorRetrieve) &&
                !regex_search(line, fields, notTransistorRetrieve)) {
                throw invalid_argument {""};
            }

            auto[id, type, desc] = tuple(fields[1], typesMapping.at(string {fields[1]}[0]), fields[2]);

            // Check for repetitive ids
            if (ids.find(id) != ids.end()) {
                throw invalid_argument {""};
            }

            // Extract node values and check for not being the same
            stringstream ss(fields[3]);
            Nodes nodes(nodesMapping.at(type));
            copy(istream_iterator<Node> {ss}, istream_iterator<Node> {}, nodes.begin());

            if (set(nodes.begin(), nodes.end()).size() <= 1) {
                throw invalid_argument {""};
            }

            ids.insert(id);
            return {type, id, desc, nodes};
        }

        vector<Element> read() {
            vector<Element> result;
            string line;
            unordered_set<ElementId> ids;

            // Parse input line by line
            for (int lineNumber {1}; getline(cin, line); ++lineNumber) {
                if (line.empty()) continue;

                // Log in case of error
                try {
                    result.push_back(parseEntry(line, ids));
                } catch (invalid_argument &) {
                    error(line, lineNumber);
                }
            }

            return result;
        }
    }

    inline namespace Result {
        auto calculateJoinsAndBill(const vector<Element> &elements) {
            map<JoinId, int> joins;
            map<ElementType, map<Description, set<ElementNo>>> bill;

            // Create histogram of used joins
            // And group ids by type and description
            joins[groundId] = 0;
            for (const Element &element : elements) {
                const auto&[type, id, description, nodes] = element;
                set<Node> nodesUnique(nodes.begin(), nodes.end());
                for_each(nodesUnique.begin(), nodesUnique.end(), [&](Node node) { ++joins[node]; });
                bill[type][description].insert(stoi(id.substr(1, id.length() - 1)));
            }

            return make_pair(joins, bill);
        }

        void showBilling(const map<ElementType, map<Description, set<ElementNo>>> &bill) {
            // Iterate over element types
            for (const auto &billing : bill) {
                // Calculate inverse of map descriptions -> ids
                map<set<ElementNo>, Description> inverse;
                for (const auto &row : billing.second) {
                    inverse[row.second] = row.first;
                }

                // Show sorted result
                for (const auto &row : inverse) {
                    printSeparating(row.first, cout, string(1, inverseTypesMapping.at(billing.first)));
                    cout << ": " << row.second << endl;
                }
            }
        }

        void checkForUnconnected(const map<JoinId, int> &joins) {
            // Fetch all unconnected
            set<int> unconnected;
            for (const auto &join : joins) {
                if (join.second < 2) {
                    unconnected.insert(join.first);
                }
            }

            // Show warning if there are some
            if (!unconnected.empty()) {
                cerr << "Warning, unconnected node(s): ";
                printSeparating(unconnected, cerr);
                cerr << endl;
            }
        }
    }
}

int main() {
    // Parse input
    auto elements = read();

    // Calculate intermediate results
    auto[joins, bill] = calculateJoinsAndBill(elements);

    // Output in appropriate way
    showBilling(bill);
    checkForUnconnected(joins);
}
