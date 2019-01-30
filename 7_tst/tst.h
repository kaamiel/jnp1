#ifndef TST_H
#define TST_H

#include <string>
#include <memory>
#include <vector>

namespace Detail {
    // Funkcja fold na zakresie wyznaczonym przez iteratory działa następująco:
    //   functor(...functor(functor(acc, *first), *(first + 1))..., *(last - 1))
    // W szczególnym przypadku first == last fold zwraca acc.
    template<typename Acc, class Iter, class Functor>
    Acc fold(Iter first, Iter last, Acc acc, Functor functor) {
        return (first == last ? acc : Detail::fold(std::next(first), last, functor(acc, *first), functor));
    }
}

// Klasa trójkowych drzew wyszukiwań (ternary search tree).
template<typename C = char>
class TST {
public:

    // Tworzy puste drzewo.
    TST() = default;

    // Tworzy drzewo zawierające słowo str.
    explicit TST(const std::basic_string<C>& str) : TST(str.c_str()) {}

    // Tworzy drzewo zawierające słowo str.
    explicit TST(const C *str)
            : TST(*str == '\0' ? TST() : TST(*str, TST(), TST(str + 1), TST(), *(str + 1) == '\0')) {}

    // Zwraca drzewo z dodanym słowem str.
    TST operator+(const std::basic_string<C>& str) const {
        return *this + str.c_str();
    }

    // Zwraca drzewo z dodanym słowem str.
    TST operator+(const C *str) const {
        return *str == '\0' ? *this :
               (empty() ? TST(*str, TST(), TST(str + 1), TST(), *(str + 1) == '\0') :
                (str[0] < value() ? TST(value(), left() + str, center(), right(), word()) :
                 (*str == value() ? TST(value(), left(), center() + (str + 1), right(), word() || *(str + 1) == '\0') :
                  TST(value(), left(), center(), right() + str, word()))));
    }

    // Zwraca znak przechowywany w korzeniu drzewa.
    C value() const {
        return empty() ? throw EmptyTreeException("TST::value() – Tree is empty.") : root->value;
    }

    // Zwraca true, jeśli w tym wierzchołku drzewa kończy się słowo i false w przeciwnym przypadku.
    bool word() const {
        return empty() ? throw EmptyTreeException("TST::word() – Tree is empty.") : root->word;
    }

    // Zwraca lewe poddrzewo.
    TST left() const {
        return empty() ? throw EmptyTreeException("TST::left() – Tree is empty.") : root->left;
    }

    // Zwraca środkowe poddrzewo.
    TST center() const {
        return empty() ? throw EmptyTreeException("TST::center() – Tree is empty.") : root->center;
    }

    // Zwraca prawe poddrzewo.
    TST right() const {
        return empty() ? throw EmptyTreeException("TST::right() – Tree is empty.") : root->right;
    }

    // Zwraca true, jeśli drzewo jest puste i false w przeciwnym przypadku.
    bool empty() const {
        return !root;
    }

    // Zwraca true, jeśli drzewo zawiera słowo str i false w przeciwnym przypadku.
    bool exist(const C *str) const {
        return *str == '\0' || empty() ? false :
               (*str < value() ? left().exist(str) :
                (*str == value() ? (*(str + 1) == '\0' ? word() : center().exist(str + 1)) :
                 right().exist(str)));
    }

    // Zwraca true, jeśli drzewo zawiera słowo str i false w przeciwnym przypadku.
    bool exist(const std::basic_string<C>& str) const {
        return exist(str.c_str());
    }

    // Wyszukuje najdłuższy wspólny prefiks słowa str i słów zawartych w danym
    // drzewie. Przykład: jeśli tst składa się ze słów "category", "functor"
    // oraz "theory" to tst.prefix("catamorphism") daje rezultat "cat".
    std::basic_string<C> prefix(const std::basic_string<C>& str) const {
        return str.empty() || empty() ? std::basic_string<C>() :
               (*str.begin() < value() ? left().prefix(str) :
                (*str.begin() > value() ? right().prefix(str) :
                 std::basic_string<C>(1, value()).append(center().prefix(std::basic_string(str, 1)))));
    }

    // Akumuluje wartości przechowywane w drzewie za pomocą funkcji functor.
    // W szczególnym przypadku pustego drzewa fold zwraca acc.
    template<typename Acc, class Functor>
    Acc fold(Acc acc, Functor functor) const {
        if (empty()) {
            return acc;
        }
        std::vector<TST> children{root->left, root->center, root->right};
        return functor(Detail::fold(children.begin(), children.end(), acc,
                                    [&](Acc acc, const TST& tst) { return tst.fold(acc, functor); }), *this);
    }

    // Zwraca liczbę wierzchołków w drzewie.
    size_t size() const {
        return this->fold(size_t{0}, [](size_t acc, const TST&) { return acc + 1; });
    }

private:

    // Struktura wierzchołka drzewa.
    struct Node {
        const C value;
        const std::shared_ptr<const Node> left;
        const std::shared_ptr<const Node> center;
        const std::shared_ptr<const Node> right;
        const bool word;

        Node(const C value, const std::shared_ptr<const Node>& left, const std::shared_ptr<const Node>& center,
             const std::shared_ptr<const Node>& right, const bool word)
                : value(value), left(left), center(center), right(right), word(word) {}
    };

    // Wskaźnik na korzeń drzewa.
    const std::shared_ptr<const Node> root;

    TST(const std::shared_ptr<const Node>& root) : root(root) {}

    TST(const C value, const TST& left, const TST& center, const TST& right, const bool word)
            : root(std::make_shared<Node>(value, left.root, center.root, right.root, word)) {}

    // Klasa wyjątku zgłaszanego w przypadku próby dostępu do składowych pustego drzewa.
    class EmptyTreeException : public std::logic_error {
    public:
        explicit EmptyTreeException(const char *what) : std::logic_error(what) {}
    };
};

#endif //TST_H