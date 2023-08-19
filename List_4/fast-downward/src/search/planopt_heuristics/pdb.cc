#include "pdb.h"

#include "../utils/logging.h"

#include <queue>
#include <limits> 

using namespace std;

namespace planopt_heuristics {

/*
  An entry in the queue is a tuple (h, i) where h is the goal distance of state i.
  See comments below for details.
*/
using QueueEntry = pair<int, int>;

PatternDatabase::PatternDatabase(const TNFTask &task, const Pattern &pattern)
    : projection(task, pattern) {
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
    distances.resize(projected_task.get_num_states(), numeric_limits<int>::max());

    /*
      Priority queues usually order entries so the largest entry is the first.
      By using the comparator greater<T> instead of the default less<T>, we
      change the ordering to sort the smallest element first.
    */
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    /*
      Note that we start with the goal state to turn the search into a regression.
      We also have to switch the role of precondition and effect in operators
      later on. This is sufficient to turn the search into a regression since
      the task is in TNF.
    */
    queue.push({0, projection.rank_state(projected_task.goal_state)});
    // cout<<"teste "<<projection.rank_state(projected_task.goal_state)<<endl;
    // TODO: add your code for exercise (b) here.

    /*o que precisamos fazer:
      uniform cost search ao contrario nos indices dos estados
      usa rank(s) -> index
      usa unrank (index) -> s
      atualizar valor da distancia para computar o pdb
    */

    /*Teste codigo
    python3 ./build.py
    python3 ./fast-downward/fast-downward.py ./castle/castle-8-5-1-cards.pddl --search "astar(planopt_pdb(pattern=[56, 57, 58, 59, 60, 61, 62, 63, 64]))"
    */

    int i = 0 ; //coloquei esse contador pra percorrer o distances, nao sei se esta certo

    while (!queue.empty()) { //a condicao tambem poderia ser enquanto o distances nao esta preenchido

      QueueEntry entry = queue.top();
      queue.pop();
      
      int entry_distance = entry.first;
      int index = entry.second;

      if(distances[i]>entry_distance) {
      distances[i] = entry_distance;
      i++;

      TNFState state_old=projection.unrank_state(index);
      TNFState state_new=projection.unrank_state(index);

      //formato da classe operator: <entry<variable_id, precondition, effect>, cost, name> 
      //percorre os operadores
      for (const auto &entry : projected_task.operators){
      //cada opedor tem um componente que afeta uma variavel
      const auto &entries = entry.entries;
      //percorre as componentes que afetam cada variavel
      for (const auto &tk : entries)
      {
        cout<<"effect value "<< projection.unrank_state(tk.effect_value) << "- index " << index << "- var id " << tk.variable_id<< endl;
        //abstract_state[var_id] := original_state[pattern[var_id]];

        //percorre o estado
        for (size_t j = 0; j < state_new.size(); ++j) {
        //cout<<"state old"<<state_new<< "- state old j"<< j << "- var id"<<tk.variable_id<<endl;
          //se a variavel for a mesma altera o estado com a pre condicao, ja que a busca e reversa
          //pelo que foi falado em aula, precisa conferir a condicao do efeito
          if (j==tk.variable_id && tk.effect_value==state_new[j]){
          state_new[j] = tk.precondition_value;
          }
        
        }
      
      }
      //PROBLEMA: esta gerando sempre o mesmo estado?
      cout<<"novo estado gerado "<< state_new <<  "- rank state " << lookup_distance(state_new)<< endl;
      //insere o estado gerado na open
      queue.push({lookup_distance(state_new), projection.rank_state(state_new)});
      cout<<"novo estado inserido"<<endl;
      //reseta o estado e vai para o prox operador
      state_new = state_old;
    }



    }
    }

}

// cout<<"teste"<<endl;
// queue.push({lookup_distance(projection.unrank_state(tk.precondition_value)), tk.precondition_value});

int PatternDatabase::lookup_distance(const TNFState &original_state) const {

    TNFState abstract_state = projection.project_state(original_state);
    
    int index = projection.rank_state(abstract_state);
    return distances[index];

}

}
