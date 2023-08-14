# IA_avancada
Lista 4
## Exercicio 2
a) Completar implementação com estado inicial, estado objetivo e operadores
caminho fast-downward/src/search/planopt_heuristics/projection.*
<pre>
```bash
#!/bin/bash
python3 ./fast-downward.py --test-projections.
```
</pre>

b) Completar a implementação computando todas as distâncias para o estado abstrato
Caminho fast-downward/src/search/planopt_heuristics/pdb.*
teste na letra (b) na (c)

c) Use a heurística "pdb" (padrão="greedy(1000)") para encontrar um bom padrão com no máximo 1000 estados abstratos para cada instância no diretório "castle". Em seguida, execute a sua implementação do *exercício (b)* usando a heurística "planopt pdb(pattern=P)". Para cada instância, utilize o mesmo padrão P utilizado pela implementação interna.
Compare as duas implementações e discuta o tempo de pré-processamento, o tempo de busca e o número de estados expandidos excluindo a última camada F (impresso como "Expandido até o último salto"). Repita o experimento para 100000 estados abstratos e compare os resultados.
**Usar o arquivo "run-ex-c" para testar
<pre>
```bash
#!/bin/bash
python3 ./fast-downward.py --test-projections.
```
</pre>
