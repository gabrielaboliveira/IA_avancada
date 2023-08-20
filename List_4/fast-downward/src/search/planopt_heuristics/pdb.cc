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

bool can_apply_operator(TNFOperator op, TNFState state){
  //Can only apply the operator on state S to get to state S+1 if ALL values of S+1 were in the effects of the operator
  //TNF then all the variables in effect and in precondition!
  for(const TNFOperatorEntry &entry : op.entries){
    if(entry.effect_value != state[entry.variable_id]){
      return false;
    }
  }
  return true;
}


TNFState get_predecessor(TNFOperator op, TNFState state){
  TNFState predecessor_state = state;

  for(const TNFOperatorEntry &entry : op.entries){
    predecessor_state[entry.variable_id] = entry.precondition_value;
  }

  return predecessor_state;
}

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

    // TODO: add your code for exercise (b) here.

    while (!queue.empty()) {
      QueueEntry entry = queue.top();
      queue.pop();
      
      int current_cost = entry.first;
      int current_pdb_index = entry.second;

      //distances funciona como uma closed, se o valor e maior, nao foi expandido
      if(distances[current_pdb_index] > current_cost){

        //atualiza a distancia
        distances[current_pdb_index] = current_cost;

        TNFState current_state = projection.unrank_state(current_pdb_index);

        //gerando os novos estados
        for (const auto &op : projected_task.operators){

          //Confere se o operador e aplicavel no estado atual
          if(can_apply_operator(op, current_state)){

            //Gera o predecessor, sucessor na reversa
            TNFState predecessor_state = get_predecessor(op, current_state);
            int predecessor_pdb_index = projection.rank_state(predecessor_state);
            int predecessor_cost = current_cost + op.cost;

            //insere novo estado na open
            if(distances[predecessor_pdb_index] > predecessor_cost){

              queue.push({predecessor_cost, predecessor_pdb_index});
            }
          }
      }
    }
  }
}

int PatternDatabase::lookup_distance(const TNFState &original_state) const {

    TNFState abstract_state = projection.project_state(original_state);
    
    int index = projection.rank_state(abstract_state);
    return distances[index];

}

}
