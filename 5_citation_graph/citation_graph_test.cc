#if TEST_NUM <= 199 || TEST_NUM == 805 || TEST_NUM >= 900

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "citation_graph.h"

class Publication {
  public:
    using id_type = std::string;
    Publication(id_type const &_id) : id(_id) {
    }
    id_type get_id() const noexcept {
      return id;
    }
  private:
    id_type id;
};

#elif TEST_NUM == 803

#include <type_traits>
#include <utility>
#include "citation_graph.h"
  struct Publication {
      using id_type = int;
      Publication(id_type const &_id) { (void)_id; }
      id_type get_id() const {
	      throw 0;
      }
  };

#elif TEST_NUM == 804

#include <type_traits>
#include <utility>
#include "citation_graph.h"
  struct Publication {
      using id_type = int;
      Publication(id_type const &_id) { (void)_id; }
      id_type get_id() const noexcept {
        return 0;
      }
  };

#elif TEST_NUM <= 699

#include "citation_graph.h"
#include <cassert>
#include <exception>

// Wyjątek zgłaszany przez klasy Publication i Publication::id_type
class ExceptionSafetyTester : public std::exception {
  public:
     ExceptionSafetyTester() : std::exception() {
     }
};

// Pomocnicza klasa służąca do systematycznego zgłaszania wyjątków przy
// każdej nadarzającej się okazji
class Thrower {
  public:
    Thrower() : enabled(false), throw_value(0), throw_counter(0), thrown(false) {
    }
    void reset(long val) {
      throw_value = val;
      throw_counter = 0;
      thrown = false;
    }
    void enable(bool en) {
      enabled = en;
    }
    void execute() {
      if (enabled) {
        // "<=" zamiast "==" pozwala wykryć niektóre błędy w obsłudze wyjątków.
        if (throw_value <= throw_counter++) {
          thrown = true;
          throw ExceptionSafetyTester();
        }
      }
    }
  private:
    bool enabled;
    long throw_value;
    long throw_counter;
  public:
    bool thrown;
};

static Thrower thrower;

// Nasza bardzo wyjątkowa klasa
class Publication {
  public:
    class id_type {
      public:
        // Publication::id_type ma konstruktor bezargumentowy, konstruktor
        // kopiujący, operator przypisania.
        id_type() : id(0) {
          thrower.execute();
        }
        id_type(id_type const &that) : id(that.id) {
          thrower.execute();
        }
        id_type& operator=(id_type const& that) {
          thrower.execute();
          id = that.id;
          return *this;
        }

        // Konstruktor do tworzenia identyfikatorów
        id_type(int i) : id(i) {
          thrower.execute();
        }

        // Destruktor musi być no-throw.
        ~id_type() {
        }

        // Wartości typu Publication::id_type tworzą porządek liniowy i można je
        // porównywać za pomocą operatorów ==, !=, <=, >=, <, >.
        bool operator==(id_type const &that) const {
          thrower.execute();
          return id == that.id;
        }
        bool operator!=(id_type const &that) const {
          thrower.execute();
          return id != that.id;
        }
        bool operator<(id_type const &that) const {
          thrower.execute();
          return id < that.id;
        }
        bool operator<=(id_type const &that) const {
          thrower.execute();
          return id <= that.id;
        }
        bool operator>(id_type const &that) const {
          thrower.execute();
          return id > that.id;
        }
        bool operator>=(id_type const &that) const {
          thrower.execute();
          return id >= that.id;
        }

        // Gwarantujemy, że swap jest no-throw.
        void swap(id_type &that) {
          int tmp = id;
          id = that.id;
          that.id = tmp;
        }

      private:
        int id;
    };

  public:
    // Klasa Publication ma konstruktor przyjmujący argument typu Publication::id_type.
    Publication(id_type const &id) : vid(id) {
      thrower.execute();
    }

    // Destruktor musi być no-throw.
    ~Publication() {
    }

    // Klasa Publication ma metodę Publication::id_type get_id() const.
    id_type get_id() const {
      thrower.execute();
      return vid;
    }

    // Do testowania, czy obiekt faktycznie istnieje.
    void set(int v) {
      value = v;
    }
    int get() const {
      return value;
    }

  private:
    id_type vid;
    int value;

    Publication() = delete;
    Publication(Publication const &) = delete;
    Publication(Publication &&) = delete;
    void operator=(Publication) = delete;
};

// Gwarantujemy, że swap jest no-throw.
void swap(Publication::id_type &a, Publication::id_type &b) {
  a.swap(b);
}

// Poniższe byty są extern, aby kompilator nie ostrzegał o ich nieużyciu.

int root_id = 10777;

void RunTest(void (* BuildGraph)(CitationGraph<Publication> &),
             void (* ModifyGraph)(CitationGraph<Publication> &),
             void (* CheckBuiltGraph)(CitationGraph<Publication> &),
             void (* CheckModifiedGraph)(CitationGraph<Publication> &),
             bool enable_constructor_exceptions,
             bool enable_method_exceptions,
             bool enable_destruction_phase_exceptions,
             bool publication_already_created_exception_expected,
             bool publication_not_found_exception_expected,
             bool tried_to_remove_root_exception_expected) {
  for (long throw_value = 0; ; ++throw_value) {
    try {
      thrower.reset(throw_value);

      // Enable, aby testować odporność na wyjątki w konstruktorze.
      thrower.enable(enable_constructor_exceptions);
      CitationGraph<Publication> r(root_id);

      // Nie chcemy tu wyjątków.
      thrower.enable(false);
      BuildGraph(r);
      CheckBuiltGraph(r);

      try {
        // Enable, aby testować odporność na wyjątki w poszczególnych metodach.
        thrower.enable(enable_method_exceptions);
        ModifyGraph(r);

        // Nie chcemy wyjątków przy sprawdzaniu poprawności.
        thrower.enable(false);
        CheckModifiedGraph(r);
      }
      catch (ExceptionSafetyTester &) {
        // Nie chcemy wyjątków przy sprawdzaniu poprawności.
        thrower.enable(false);
        assert(thrower.thrown);
        thrower.thrown = false;
        CheckBuiltGraph(r);

        // Testuj, aż przestaną być zgłaszane wyjątki.
        continue;
      }
      catch (...) {
        // Zakładamy, że tylko ModifyGraph zgłasza wyjątki inne niż ExceptionSafetyTester.
        thrower.enable(false);
        CheckBuiltGraph(r);

        // Pozostałe przepuszczaj po sprawdzeniu.
        throw;
      }

      // Enable, aby testować no-throw destruktora.
      thrower.enable(enable_destruction_phase_exceptions);
    }
    catch (ExceptionSafetyTester &) {
      // Tu wchodzimy, gdy wyjątek został zgłoszony w konstruktorze lub destruktorze.
      assert(thrower.thrown);
      thrower.thrown = false;
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      continue;
    }
    catch (PublicationAlreadyCreated &) {
      assert(publication_already_created_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (PublicationNotFound &) {
      assert(publication_not_found_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (TriedToRemoveRoot &) {
      assert(tried_to_remove_root_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (...) {
      // Każdy inny wyjątek jest tu błędem.
      assert(false);
      return;
    }

    // Nie było wyjątku, a oczekiwaliśmy go.
    assert(!thrower.thrown);
    assert(!publication_already_created_exception_expected);
    assert(!publication_not_found_exception_expected);
    assert(!tried_to_remove_root_exception_expected);

    // Nie ma więcej wyjątków, więc test zakończył się pomyślnie.
    return;
  }
}

void NOP(CitationGraph<Publication> &) {
}

void CheckGraph0(CitationGraph<Publication>& graph) {
  assert(!graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_root_id() == root_id);
  assert(graph.get_children(root_id).size() == 0);
  assert(graph.get_parents(root_id).size() == 0);
}

void CheckGraph1(CitationGraph<Publication>& graph) {
  assert(graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_root_id() == root_id);
  assert(graph.get_parents(root_id).size() == 0);
  std::vector<Publication::id_type> children0 = graph.get_children(root_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  std::vector<Publication::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == root_id);
  assert(graph.get_children(1).size() == 0);
}

void CheckGraph12(CitationGraph<Publication>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_root_id() == root_id);
  assert(graph.get_parents(root_id).size() == 0);
  std::vector<Publication::id_type> children0 = graph.get_children(root_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  std::vector<Publication::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == root_id);
  std::vector<Publication::id_type> children1 = graph.get_children(1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  std::vector<Publication::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == 1);
  assert(graph.get_children(2).size() == 0);
}

void CheckGraph2(CitationGraph<Publication>& graph) {
  assert(!graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_root_id() == root_id);
  assert(graph.get_parents(root_id).size() == 0);
  std::vector<Publication::id_type> children0 = graph.get_children(root_id);
  assert(children0.size() == 1);
  assert(children0[0] == 2);

  std::vector<Publication::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == root_id);
  assert(graph.get_children(2).size() == 0);
}

void CheckMoveOps1(CitationGraph<Publication>& graph) {
  CheckGraph1(graph);
  CitationGraph<Publication> graph2(std::move(graph));
  CheckGraph1(graph2);
  graph = std::move(graph2);
  CheckGraph1(graph);
}

void CheckGraph123(CitationGraph<Publication>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_root_id() == root_id);
  assert(graph.get_parents(root_id).size() == 0);
  std::vector<Publication::id_type> children0 = graph.get_children(root_id);
  assert(children0.size() == 2);
  assert((children0[0] == 1 && children0[1] == 2) ||
         (children0[0] == 2 && children0[1] == 1));

  std::vector<Publication::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == root_id);
  std::vector<Publication::id_type> children1 = graph.get_children(1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  std::vector<Publication::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 2);
  assert((parents2[0] == 1 && parents2[1] == root_id) ||
         (parents2[0] == root_id && parents2[1] == 1));
  assert(graph.get_children(2).size() == 0);
}

void Create1(CitationGraph<Publication>& graph) {
  graph.create(1, root_id);
}

void Create2single(CitationGraph<Publication>& graph) {
  graph.create(2, 1);
}

void Create2vector(CitationGraph<Publication>& graph) {
  std::vector<Publication::id_type> parents;
  parents.push_back(1);
  parents.push_back(root_id);
  graph.create(2, parents);
}

void CreateEmpty(CitationGraph<Publication>& graph) {
  std::vector<Publication::id_type> parents;
  graph.create(2, parents);
}

void Graph2(CitationGraph<Publication>& graph) {
  Create1(graph);
  Create2single(graph);
}

void Graph3(CitationGraph<Publication>& graph) {
  Create1(graph);
  Create2vector(graph);
}

void Add_Citation20(CitationGraph<Publication>& graph) {
  graph.add_citation(2, root_id);
}

void Add_Citation25(CitationGraph<Publication>& graph) {
  graph.add_citation(2, 5);
}

void Remove0(CitationGraph<Publication>& graph) {
  graph.remove(root_id);
}

void Remove1(CitationGraph<Publication>& graph) {
  graph.remove(1);
}

void Remove2(CitationGraph<Publication>& graph) {
  graph.remove(2);
}

void Add_Citation20Remove2(CitationGraph<Publication>& graph) {
  Add_Citation20(graph);
  Remove2(graph);
}

void GetChildren(CitationGraph<Publication>& graph) {
  assert(graph.get_children(100).size() == 0);
}

void GetParents(CitationGraph<Publication>& graph) {
  assert(graph.get_parents(100).size() == 0);
}

void Operator(CitationGraph<Publication>& graph) {
  assert(graph[100].get_id() == 0);
}

void SimpleTest(CitationGraph<Publication>& graph) {
  graph.create(7, root_id);
  assert(graph.exists(root_id));
  assert(!graph.exists(6));
  assert(graph.exists(7));
  assert(!graph.exists(8));
  Publication &v0 = graph[root_id];
  Publication &v7 = graph[7];
  v0.set(66);
  v7.set(123);
  assert(v0.get() == 66);
  assert(v7.get() == 123);
}

void AdvancedTest(CitationGraph<Publication>& graph) {
  int i, n;

  for (n = 2; n <= 10; ++n) {
    graph.create(1, root_id);
    graph.create(2, root_id);
    for (i = 1; i <= n - 2; ++i) {
      graph.create(i + 2, i);
    }
    for (i = 1; i <= n - 3; ++i) {
      graph.add_citation(i + 3, i);
    }
    graph.remove(1);
    graph.remove(2);
    assert(graph.exists(root_id));
    for (i = 0; i <= n + 1; ++i) {
      assert(!graph.exists(i));
    }
  }
}

#elif TEST_NUM == 700

// Test, czy jest header guard.
#include "citation_graph.h"
#include "citation_graph.h"

#else

#include "citation_graph.h"
#include <cassert>
#include <exception>

#endif

int main() {

  // Test z treści zadania

  #if TEST_NUM == 100

  CitationGraph<Publication> gen("goto Considered Harmful");
  Publication::id_type const id1 = gen.get_root_id();
  assert(gen.exists(id1));
  assert(gen.get_parents(id1).size() == 0);
  gen.create("A", id1);
  gen.create("B", id1);
  assert(gen.get_children(id1).size() == 2);
  gen.create("C", "A");
  gen.add_citation("C", "B");
  assert(gen.get_parents("C").size() == 2);
  assert(gen.get_children("A").size() == 1);
  std::vector<Publication::id_type> parents;
  parents.push_back("A");
  parents.push_back("B");
  gen.create("D", parents);
  assert(gen.get_parents("D").size() == parents.size());
  assert(gen.get_children("A").size() == 2);
  assert("D" == gen["D"].get_id());
  gen.remove("A");
  assert(!gen.exists("A"));
  assert(gen.exists("B"));
  assert(gen.exists("C"));
  assert(gen.exists("D"));
  gen.remove("B");
  assert(!gen.exists("A"));
  assert(!gen.exists("B"));
  assert(!gen.exists("C"));
  assert(!gen.exists("D"));
  try {
    gen["E"];
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.create("E", "goto Considered Harmful");
    gen.create("E", "goto Considered Harmful");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.remove("goto Considered Harmful");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  #endif

  // Proste testy poprawności poszczególnych operacji

  #if TEST_NUM == 200
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 201
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 202
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 203
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 204
  RunTest(Graph2, Add_Citation20, CheckGraph12, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 205
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 206
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 207
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 208
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 209
  RunTest(Graph3, Add_Citation20, CheckGraph123, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 210
  RunTest(Graph3, Add_Citation20Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 211
  RunTest(NOP, Create1, CheckGraph0, CheckMoveOps1,
          false, false, false, false, false, false);
  #endif

  // Testy poprawności zgłaszania wyjątków

  #if TEST_NUM == 300
  RunTest(Create1, Create1, CheckGraph1, CheckGraph1,
          false, false, false, true, false, false); // PublicationAlreadyCreated
  #endif

  #if TEST_NUM == 301
  RunTest(Graph2, Create2vector, CheckGraph12, CheckGraph12,
          false, false, false, true, false, false); // PublicationAlreadyCreated
  #endif

  #if TEST_NUM == 302
  RunTest(NOP, Create2single, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 303
  RunTest(NOP, Create2vector, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 304
  RunTest(NOP, Add_Citation20, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 305
  RunTest(NOP, Remove0, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true); // TriedToRemoveRoot
  #endif

  #if TEST_NUM == 306
  RunTest(Graph3, Remove0, CheckGraph123, CheckGraph123,
          false, false, false, false, false, true); // TriedToRemoveRoot
  #endif

  #if TEST_NUM == 307
  RunTest(NOP, GetChildren, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 308
  RunTest(NOP, GetParents, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 309
  RunTest(NOP, Operator, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 310
  RunTest(Graph2, Add_Citation25, CheckGraph12, CheckGraph12,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 311
  RunTest(Create1, Remove2, CheckGraph1, CheckGraph1,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  #if TEST_NUM == 312
  RunTest(NOP, CreateEmpty, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // PublicationNotFound
  #endif

  // Testy no-throw destruktora

  #if TEST_NUM == 400
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 401
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 402
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 403
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 404
  RunTest(Graph2, Add_Citation20, CheckGraph12, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 405
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 406
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 407
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 408
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 409
  RunTest(Graph3, Add_Citation20, CheckGraph123, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  // Testy odporności na wyjątki

  #if TEST_NUM == 500
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          true, false, false, false, false, false);
  #endif

  #if TEST_NUM == 501
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 502
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 503
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 504
  RunTest(Graph2, Add_Citation20, CheckGraph12, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 505
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 506
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 507
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 508
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 509
  RunTest(Graph3, Add_Citation20, CheckGraph123, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  // Różne testy

  #if TEST_NUM == 600
  RunTest(NOP, SimpleTest, NOP, NOP,
          true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 601
  RunTest(NOP, SimpleTest, NOP, NOP,
          false, true, true, false, false, false);
  #endif

  #if TEST_NUM == 602
  RunTest(NOP, AdvancedTest, NOP, NOP,
          true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 603
  RunTest(NOP, AdvancedTest, NOP, NOP,
          false, true, true, false, false, false);
  #endif

  // Testy, czy wyjątki dziedziczą po std::exception.

  #if TEST_NUM == 800
  try {
    throw PublicationAlreadyCreated();
  }
  catch (std::exception &e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 801
  try {
    throw PublicationNotFound();
  }
  catch (std::exception &e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 802
  try {
    throw TriedToRemoveRoot();
  }
  catch (std::exception &e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  // Test specyfikacji nothrow
  #if TEST_NUM == 803
  static_assert(!std::is_nothrow_invocable<decltype(&CitationGraph<Publication>::get_root_id), CitationGraph<Publication> *>::value);
  static_assert(!std::is_nothrow_invocable<decltype(&Publication::get_id), Publication *>::value);

  #endif

  #if TEST_NUM == 804
  static_assert(std::is_nothrow_invocable<decltype(&CitationGraph<Publication>::get_root_id), CitationGraph<Publication> *>::value);
  static_assert(std::is_nothrow_invocable<decltype(&Publication::get_id), Publication *>::value);
  #endif

  #if TEST_NUM == 805
  static_assert(std::is_nothrow_move_constructible<CitationGraph<Publication>>::value);
  #endif

  // Próba użycia konstruktora kopiującego lub operatora przypisania dla
  // obiektów klasy CitationGraph powinna zakończyć się błędem kompilacji.

  #if TEST_NUM == 900
  // Musi być jeden test, który zawsze powinien się skompilować,
  // aby niekompilujący się program nie dostał punktu.
  #endif

  #if TEST_NUM == 901
  CitationGraph<Publication> r1("ABC");
  CitationGraph<Publication> r2(r1);
  #endif

  #if TEST_NUM == 902
  CitationGraph<Publication> r1("A");
  CitationGraph<Publication> r2("B");
  r1 = r2;
  #endif

}
