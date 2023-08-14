#include "pdb.h"

#include "../utils/logging.h"

#include <queue>
#include <limits>

using namespace std;

namespace planopt_heuristics
{

  /*
    An entry in the queue is a tuple (h, i) where h is the goal distance of state i.
    See comments below for details.
  */
  using QueueEntry = pair<int, int>;

  PatternDatabase::PatternDatabase(const TNFTask &task, const Pattern &pattern)
      : projection(task, pattern)
  {
    /*
      We want to compute goal distances for all abstract states in the
      projected task. To do so, we start by assuming every abstract state has
      an infinite goal distance and then do a backwards uniform cost search
      updating the goal distances of all encountered states.

      Instead of searching on the actual states, we use perfect hashing to
      run the search on the hash indices of states. To go from a state s to its
      index use rank(s) and to go from an index i to its state use unrank(i).
    */
    const TNFTask &projected_task = projection.get_projected_task();
    // o vetor distances é redimensionado para o tamanho da tarefa projetada
    // e todos os elementos são inicializados para o máximo: inf, para depois inicializar a busca e atualizar o valor
    distances.resize(projected_task.get_num_states(), numeric_limits<int>::max());

    /*
      Priority queues usually order entries so the largest entry is the first.
      By using the comparator greater<T> instead of the default less<T>, we
      change the ordering to sort the smallest element first.
    */
    // fila utilizada para o uniform cost search reverso
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    /*
      Note that we start with the goal state to turn the search into a regression.
      We also have to switch the role of precondition and effect in operators
      later on. This is sufficient to turn the search into a regression since
      the task is in TNF.
    */
    // insere ESTADO OBJETIVO: distancia zero e o indice de classificação
    queue.push({0, projection.rank_state(projected_task.goal_state)});

    // INICIO EXERICIO (b)
    /* TODO: add your code for exercise (b) here: Computar a distancia
    para todos os estados abstratos
    */
    // inicializa a closed
    unordered_set<int> closed;
    // insere o goal na closed
    closed.insert(closed.begin(), projection.rank_state(projected_task.goal_state));
    queue.push({lookup_distance(projected_task.initial_state), projection.rank_state(projected_task.initial_state)});
    closed.insert(closed.begin(), projection.rank_state(projected_task.initial_state));

    for (const auto entry : projected_task.operators)
    {
      for (const auto tk : entry.entries)
      {
        if (closed.find(projection.rank_state(projection.unrank_state(tk.precondition_value))) == closed.end())
        {
          closed.insert(closed.begin(), projection.rank_state(projection.unrank_state(tk.precondition_value)));
          queue.push({lookup_distance(projection.unrank_state(tk.precondition_value)), projection.rank_state(projection.unrank_state(tk.precondition_value))});
        }
      }
    }
  }

  int PatternDatabase::lookup_distance(const TNFState &original_state) const
  {
    TNFState abstract_state = projection.project_state(original_state);
    int index = projection.rank_state(abstract_state);
    return distances[index];
  }
  // FIM EXERICIO (b)

}
