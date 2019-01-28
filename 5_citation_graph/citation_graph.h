#ifndef CITATION_GRAPH_CITATION_GRAPH_H
#define CITATION_GRAPH_CITATION_GRAPH_H

#include <exception>
#include <vector>
#include <memory>
#include <set>
#include <map>

class PublicationNotFound : public std::exception {
    const char *what() const noexcept override {
        return "PublicationNotFound";
    }
};

class PublicationAlreadyCreated : public std::exception {
    const char *what() const noexcept override {
        return "PublicationAlreadyCreated";
    }
};

class TriedToRemoveRoot : public std::exception {
    const char *what() const noexcept override {
        return "TriedToRemoveRoot";
    }
};

template<class Publication>
class CitationGraph {

public:

    // Tworzy nowy graf. Tworzy także węzeł publikacji o identyfikatorze stem_id.
    explicit CitationGraph(const typename Publication::id_type& stem_id);

    // Nie można kopiować obiektów klasy CitationGraph.
    CitationGraph(CitationGraph<Publication>& other) = delete;

    CitationGraph<Publication>& operator=(CitationGraph<Publication>& other) = delete;

    // Konstruktor przenoszący i przenoszący operator przypisania.
    CitationGraph(CitationGraph<Publication>&& other) noexcept;

    CitationGraph<Publication>& operator=(CitationGraph<Publication>&& other) noexcept;

    // Destruktor.
    ~CitationGraph();

    // Zwraca identyfikator źródła. Metoda ta powinna być noexcept wtedy i tylko
    // wtedy, gdy metoda Publication::get_id jest noexcept.
    typename Publication::id_type get_root_id() const noexcept(noexcept(std::declval<Publication>().get_id()));

    // Zwraca listę identyfikatorów publikacji cytujących publikację o podanym
    // identyfikatorze. Zgłasza wyjątek PublicationNotFound, jeśli dana publikacja
    // nie istnieje.
    std::vector<typename Publication::id_type> get_children(const typename Publication::id_type& id) const;

    // Zwraca listę identyfikatorów publikacji cytowanych przez publikację o podanym
    // identyfikatorze. Zgłasza wyjątek PublicationNotFound, jeśli dana publikacja
    // nie istnieje.
    std::vector<typename Publication::id_type> get_parents(const typename Publication::id_type& id) const;

    // Sprawdza, czy publikacja o podanym identyfikatorze istnieje.
    bool exists(const typename Publication::id_type& id) const;

    // Zwraca referencję do obiektu reprezentującego publikację o podanym
    // identyfikatorze. Zgłasza wyjątek PublicationNotFound, jeśli żądana publikacja
    // nie istnieje.
    Publication& operator[](const typename Publication::id_type& id) const;

    // Tworzy węzeł reprezentujący nową publikację o identyfikatorze id cytującą
    // publikacje o podanym identyfikatorze parent_id lub podanych identyfikatorach
    // parent_ids. Zgłasza wyjątek PublicationAlreadyCreated, jeśli publikacja
    // o identyfikatorze id już istnieje. Zgłasza wyjątek PublicationNotFound, jeśli
    // któryś z wyspecyfikowanych poprzedników nie istnieje albo lista poprzedników jest pusta.
    void create(const typename Publication::id_type& id, const typename Publication::id_type& parent_id);

    void create(const typename Publication::id_type& id, const std::vector<typename Publication::id_type>& parent_ids);

    // Dodaje nową krawędź w grafie cytowań. Zgłasza wyjątek PublicationNotFound,
    // jeśli któraś z podanych publikacji nie istnieje.
    void add_citation(const typename Publication::id_type& child_id, const typename Publication::id_type& parent_id);

    // Usuwa publikację o podanym identyfikatorze. Zgłasza wyjątek
    // PublicationNotFound, jeśli żądana publikacja nie istnieje. Zgłasza wyjątek
    // TriedToRemoveRoot przy próbie usunięcia pierwotnej publikacji.
    // W wypadku rozspójnienia grafu, zachowujemy tylko spójną składową zawierającą źródło.
    void remove(const typename Publication::id_type& id);

private:

    struct Node;

    std::shared_ptr<Node> root;

    std::map<typename Publication::id_type, std::weak_ptr<Node>> map;

    struct Node {

        Node() = delete;

        Node(std::map<typename Publication::id_type, std::weak_ptr<Node>> *map,
             const typename Publication::id_type& id);

        ~Node();

        inline void addChild(const std::shared_ptr<Node>& child) {
            children.insert(std::shared_ptr<Node>(child));
        }

        inline void addParent(std::shared_ptr<Node> parent) {
            parents.insert(parent);
        }

        std::map<typename Publication::id_type, std::weak_ptr<Node>> *map;

        Publication publication;

        std::set<std::shared_ptr<Node>> children;

        std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> parents;

        typename std::map<typename Publication::id_type, std::weak_ptr<Node>>::iterator iterator;

    };

};

template<class Publication>
CitationGraph<Publication>::CitationGraph(const typename Publication::id_type& stem_id) {
    std::weak_ptr<Node> weak_ptr;
    root = std::make_shared<Node>(&map, stem_id);
    weak_ptr = root;

    auto pair = map.insert(std::make_pair(stem_id, weak_ptr));
    root->iterator = pair.first;
}

template<class Publication>
CitationGraph<Publication>::CitationGraph(CitationGraph<Publication>&& other) noexcept
        : root(std::move(other.root)), map(std::move(other.map)) {}

template<class Publication>
CitationGraph<Publication>& CitationGraph<Publication>::operator=(CitationGraph<Publication>&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    this->root = std::move(other.root);
    this->map = std::move(other.map);
    return *this;
}

template<class Publication>
CitationGraph<Publication>::~CitationGraph() {
    root.reset();
    map.clear();
}

template<class Publication>
typename Publication::id_type
CitationGraph<Publication>::get_root_id() const noexcept(noexcept(std::declval<Publication>().get_id())) {
    return root->publication.get_id();
}

template<class Publication>
std::vector<typename Publication::id_type>
CitationGraph<Publication>::get_children(const typename Publication::id_type& id) const {
    auto it = map.find(id);
    if (it == map.end()) {
        throw PublicationNotFound();
    }

    std::vector<typename Publication::id_type> res;
    for (auto& child : it->second.lock()->children) {
        res.push_back(child->publication.get_id());
    }

    return res;
}

template<class Publication>
std::vector<typename Publication::id_type>
CitationGraph<Publication>::get_parents(const typename Publication::id_type& id) const {
    auto it = map.find(id);

    if (it == map.end()) {
        throw PublicationNotFound();
    }

    std::vector<typename Publication::id_type> res;
    for (auto& parent : it->second.lock()->parents) {
        res.push_back(parent.lock()->publication.get_id());
    }

    return res;
}

template<class Publication>
bool CitationGraph<Publication>::exists(const typename Publication::id_type& id) const {
    return map.find(id) != map.end();
}

template<class Publication>
Publication& CitationGraph<Publication>::operator[](const typename Publication::id_type& id) const {
    auto it = map.find(id);

    if (it == map.end()) {
        throw PublicationNotFound();
    }

    return it->second.lock()->publication;
}

template<class Publication>
void CitationGraph<Publication>::create(const typename Publication::id_type& id,
                                        const typename Publication::id_type& parent_id) {
    create(id, std::vector<typename Publication::id_type>{parent_id});
}

template<class Publication>
void CitationGraph<Publication>::create(const typename Publication::id_type& id,
                                        const std::vector<typename Publication::id_type>& parent_ids) {
    auto it = map.find(id);
    if (it != map.end()) {
        throw PublicationAlreadyCreated();
    }

    if (parent_ids.empty()) {
        throw PublicationNotFound();
    }
    for (auto& parentId : parent_ids) {
        it = map.find(parentId);
        if (it == map.end()) {
            throw PublicationNotFound();
        }
    }

    std::weak_ptr<Node> weak_ptr;
    auto child = std::make_shared<Node>(&map, id);
    weak_ptr = child;

    std::vector<std::shared_ptr<Node>> shared_parents;

    // To jest const, ale może zgłosić wyjątek.
    for (auto& parentId : parent_ids) {
        auto parent = map.find(parentId)->second.lock();
        shared_parents.push_back(parent);
    }

    // To nie jest const, może zgłosić wyjątek.
    auto pair = map.insert(std::make_pair(id, weak_ptr));

    // To nie powinno zgłaszać wyjątków.
    for (auto& parent: shared_parents) {
        child->addParent(parent);
        parent->addChild(child);
    }

    child->iterator = pair.first;
}

template<class Publication>
void CitationGraph<Publication>::add_citation(const typename Publication::id_type& child_id,
                                              const typename Publication::id_type& parent_id) {
    auto it1 = map.find(child_id);
    auto it2 = map.find(parent_id);
    if (it1 == map.end() || it2 == map.end()) {
        throw PublicationNotFound();
    }

    auto child = it1->second.lock();
    auto parent = it2->second.lock();
    child->addParent(parent);
    parent->addChild(std::shared_ptr<Node>(child));
}

template<class Publication>
void CitationGraph<Publication>::remove(const typename Publication::id_type& id) {
    if (id == get_root_id()) {
        throw TriedToRemoveRoot();
    }

    auto it = map.find(id);
    if (it == map.end()) {
        throw PublicationNotFound();
    }

    auto shared_ptr = it->second.lock();
    for (auto& parent : shared_ptr->parents) {
        parent.lock()->children.erase(shared_ptr);
    }
}

template<class Publication>
CitationGraph<Publication>::Node::Node(std::map<typename Publication::id_type, std::weak_ptr<Node>> *map,
                                       const typename Publication::id_type& id)
        : map(map), publication(Publication{id}) {}

template<class Publication>
CitationGraph<Publication>::Node::~Node() {
    for (auto& child : children) {
        child->parents.erase(this->iterator->second);
    }
    children.clear();
    parents.clear();

    map->erase(iterator);
}

#endif //CITATION_GRAPH_CITATION_GRAPH_H
