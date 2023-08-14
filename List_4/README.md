# IA_avancada
Lista 4
## Exercicio 2
<span style="color: red;"> Caminho arquivos: fast-downward/src/search/planopt_heuristics </span>
a) Completar implementação com estado inicial, estado objetivo e operadores.
<span style="color: red;"> Arquivo: projection.* </span>
<pre>
```bash
#!/bin/bash
python3 ./fast-downward.py --test-projections 
```
</pre>

b) Completar a implementação computando todas as distâncias para o estado abstrato
Arquivo: pdb.*
teste na letra (b) na (c)

c) Use a heurística "pdb" (padrão="greedy(1000)") para encontrar um bom padrão com no máximo 1000 estados abstratos para cada instância no diretório "castle". Em seguida, execute a sua implementação do *exercício (b)* usando a heurística "planopt pdb(pattern=P)". Para cada instância, utilize o mesmo padrão P utilizado pela implementação interna.
Compare as duas implementações e discuta o tempo de pré-processamento, o tempo de busca e o número de estados expandidos excluindo a última camada F (impresso como "Expandido até o último salto"). Repita o experimento para 100000 estados abstratos e compare os resultados.
**Usar o arquivo "run-ex-c" para testar**
<pre>
```bash
#!/bin/bash
python3 ./fast-downward.py ./castle/castle-8-5-1-cards.pddl --search "astar(planopt_pdb(pattern=[56, 57, 58, 59, 60, 61, 62, 63, 64]))"
```
</pre>
 
d) Complete a implementação nos métodos "build compatibility graph" (construir o grafo de compatibilidade) e "compute heuristic" (calcular a heurística) para criar o grafo de compatibilidade para uma coleção de patterns dada e para calcular o valor heurístico com base nos cliques máximos desse grafo.
**Usar o arquivo "run-ex-e" para testar**
Caminho: Arquivo: canonical_pdbs.*

e) Utilize as coleções de padrões fornecidas com o script, com no máximo 1000 estados abstratos, para resolver cada instância no diretório "nomystery-opt11-strips". Execute a sua implementação do exercício (d) utilizando a heurística "planopt cpdbs(patterns=C)". Para cada instância, utilize a mesma coleção de padrões C usada pela implementação interna.
Compare as duas implementações e discuta o tempo total e o número de estados expandidos excluindo a última camada F (impresso como "Expandido até o último salto"). Também compare os seus resultados ao usar uma única heurística de banco de dados de padrões com até 1000 estados abstratos, como no exercício (c).
<pre>
```bash
python3 ./fast-downward.py ./nomystery-opt11-strips/p01.pddl --search "astar(pdb(pattern=greedy(1000)))"
python3 ./fast-downward.py ./nomystery-opt11-strips/p01.pddl --search "astar(planopt_cpdbs(patterns=[[0, 2, 3, 4]]))"
 ```
</pre>
