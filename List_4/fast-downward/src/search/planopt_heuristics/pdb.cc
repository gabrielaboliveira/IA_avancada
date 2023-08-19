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

    /*ucs ao contrario nos indices dos estados
      usa rank(s) -> index
      usa unrank (index) -> s
      atualizar valor da distancia
    */

    /*Teste codigo
    python3 ./build.py
    python3 ./fast-downward/fast-downward.py ./castle/castle-8-5-1-cards.pddl --search "astar(planopt_pdb(pattern=[56, 57, 58, 59, 60, 61, 62, 63, 64]))"
    */

    int i = 0 ; //coloquei esse contador pra percorrer o distances, nao sei se esta certo
    int count = 0;
    std::unordered_set<int> operatorclosed; 


    while (!queue.empty()) { //a condicao tambem poderia ser enquanto o distances nao esta preenchido
      cout<<"======NOVO LOOP DA OPEN======"<<endl;
      QueueEntry entry = queue.top();
      queue.pop();
      
      int entry_distance = entry.first;
      int index = entry.second;

      if(distances[i]>entry_distance) {
        distances[i] = entry_distance;
        i++;

        TNFState state_old=projection.unrank_state(index);
        TNFState state_new=projection.unrank_state(index);

        cout<< "ESTADO " << state_old<< endl;
        //formato da classe operator: <entry<variable_id, precondition, effect>, cost, name> 
        //percorre os operadores
        for (const auto &entry : projected_task.operators){

          //cada opedor tem um componente que afeta uma variavel
          const auto &entries = entry.entries;
          int pre_conditions_test = 0; 

          //percorre as componentes que afetam cada variavel
          cout<< "OPERADOR INITIAL " << entry.name<< endl;
          for (const auto &tk : entries)
          {
            
            //usa o proprio variable id pra acessar a variavel dentro do state
            cout<<" VAR ID " << tk.variable_id<< " effect "<<tk.effect_value << " precond "<<tk.precondition_value <<"|| index " << index << endl;
            cout<<"  IF efeito " << tk.effect_value<< " = "<<state_old[tk.variable_id] << " stated em var id "<< endl;
            //Mais um teste pra ver se a variavel ja nao e igual a pre condicao, ai nao precisa inserir
            if (tk.effect_value==state_old[tk.variable_id] && state_new[tk.variable_id] != tk.precondition_value){
              state_new[tk.variable_id] = tk.precondition_value;
              pre_conditions_test++;
              }
          
          }
          cout<< "OPERADOR END" <<endl;
          
          
          QueueEntry state_queue = {(lookup_distance(state_new) + entry.cost), projection.rank_state(state_new)};
          cout<< "  estado gerado " << state_queue.second << endl;
          //insere o estado gerado na open
          if(pre_conditions_test>=1 && operatorclosed.find(state_queue.second)==operatorclosed.end()){
            operatorclosed.insert(state_queue.second); //coloquei um unordered set pra testar se ja nao foi inserido, nao sei se ta certo assim
            queue.push(state_queue);
            cout<<"-------novo estado inserido----------"<<endl;
            cout<<endl;
            //reseta o estado e vai para o prox operador
            state_new = state_old;
          }
          count++;

          //problema: ele gera o estado da forma certa aparentemente, mas parece estar usando sempre o mesmo operador
          
          //break de teste
          //  if(count==5){
          //    break;
          // }
        }
        cout<<endl;
      
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
