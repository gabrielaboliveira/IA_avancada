#include "projection.h"

using namespace std;

namespace planopt_heuristics
{
    Projection::Projection(const TNFTask &task, const Pattern &pattern)
        : pattern(pattern)
    {
        /*
          Create variables and remember mapping between variables in the original
          and the projected task.

          In a task with 10 variables and a pattern {3,8,5}
          the projected task has 3 variables {0,1,2}.
          To keep track of which variable of the original task corresponds to
          which variable of the projected task, we use a vector that maps original
          variables to projected variables. In the example above, the mapping would
          be [-1, -1, -1, 0, -1, 2, -1, -1, 1, -1]
        */
       //inicializado com -1 -> nenhuma variavel foi mapeada
       //o tamanho do vetor e o tamanho da tarefa original
        vector<int> variable_mapping(task.variable_domains.size(), -1); 
        int var_id = 0; // variavel utilizada para atribuir index
        for (int pattern_var_id : pattern)
        {
            variable_mapping[pattern_var_id] = var_id; //atribui um index var_id para cada valor do pattern
            int domain_size = task.variable_domains[pattern_var_id]; // o tamanho do dominio e o tamanho da variavel original
            projected_task.variable_domains.push_back(domain_size); // ??????
            ++var_id;
        }

        /*
          Compute multipliers for ranking/unranking states.
        */
        int multiplier = 1;
        for (size_t i = 0; i < pattern.size(); ++i)
        {
            perfect_hash_multipliers.push_back(multiplier);
            multiplier *= projected_task.variable_domains[i];
        }

        /*
          Project initial state and goal state, and set
          projected_task.initial_state and projected_task.goal_state.
        */

        // TODO: add your code for exercise (a) here.
        // INICIO (a)

        for (int i = pattern.size() - 1; i >= 0; i--)
        {
            projected_task.initial_state.push_back(task.initial_state.at(i));
            projected_task.goal_state.push_back(task.goal_state.at(i));
        }

        /*
          Project operators and create the projected operators in
          projected_task.operators. Do not add operators that become no-ops after
          projection.
        */

        for (const TNFOperator &op : task.operators)
        {
            vector<TNFOperatorEntry> projected_entries;
            for (const TNFOperatorEntry entry : op.entries)
            {
                int var_id = variable_mapping[entry.variable_id];
                if (var_id != -1)
                {
                    projected_entries.emplace_back(var_id, entry.precondition_value, entry.effect_value);
                }
            }
        }

        // TODO: add your code for exercise (a) here.
        // FIM (a)
    }

    TNFState Projection::project_state(const TNFState &original_state) const
    {
        int num_abstract_variables = pattern.size();
        TNFState abstract_state(num_abstract_variables, -1);
        for (size_t var_id = 0; var_id < pattern.size(); ++var_id)
        {
            abstract_state[var_id] = original_state[pattern[var_id]];
        }
        return abstract_state;
    }
    //rank: recebe um estado e retorna o index pro mapping

    int Projection::rank_state(const TNFState &state) const
    {
        assert(state.size() == pattern.size());
        size_t index = 0;
        for (size_t i = 0; i < state.size(); ++i)
        {
            index += perfect_hash_multipliers[i] * state[i];
        }
        return index;
    }

//unrank recebe um index e retorna o estado pro mapping
    TNFState Projection::unrank_state(int index) const
    {
        vector<int> values(pattern.size());
        for (int i = pattern.size() - 1; i >= 0; --i)
        {
            values[i] = index / perfect_hash_multipliers[i];
            index -= values[i] * perfect_hash_multipliers[i];
        }
        assert(index == 0);
        return values;
    }
}
