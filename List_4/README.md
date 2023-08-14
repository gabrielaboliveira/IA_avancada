# IA Avançada - Lista 4
## Exercício 2
Caminho dos arquivos: `fast-downward/src/search/planopt_heuristics`

### a) Implementação inicial

Completar o código com estado inicial, estado objetivo e operadores.

Arquivo: `projection.*`

\`\`\`bash
#!/bin/bash
python3 ./fast-downward.py --test-projections
\`\`\`

### b) Computando distâncias para estado abstrato

Completar a implementação computando todas as distâncias para o estado abstrato.

Arquivo: `pdb.*`

Teste nas partes (b) e (c).

### c) Experimento com heurísticas

1. Use a heurística "pdb" com o padrão padrão="greedy(1000)" para encontrar um padrão com no máximo 1000 estados abstratos para cada instância no diretório "castle".
2. Execute a implementação do *exercício (b)* usando a heurística "planopt pdb(pattern=P)".
3. Utilize o mesmo padrão P da implementação interna.
4. Compare as duas implementações e discuta o tempo de pré-processamento, tempo de busca e o número de estados expandidos excluindo a última camada F ("Expandido até o último salto").
5. Repita o experimento para 100000 estados abstratos e compare os resultados.

Usar o arquivo "run-ex-c" para testar:

\`\`\`bash
#!/bin/bash
python3 ./fast-downward.py ./castle/castle-8-5-1-cards.pddl --search "astar(planopt_pdb(pattern=[56, 57, 58, 59, 60, 61, 62, 63, 64]))"
\`\`\`

### d) Implementação nos métodos "build compatibility graph" e "compute heuristic"

Complete a implementação nos métodos "build compatibility graph" (construir o grafo de compatibilidade) e "compute heuristic" (calcular a heurística) para criar o grafo de compatibilidade para uma coleção de padrões dada e calcular o valor heurístico com base nos cliques máximos desse grafo.

Usar o arquivo "run-ex-e" para testar:

Caminho do arquivo: `canonical_pdbs.*`

### e) Utilize as coleções de padrões fornecidas com o script

1. Use as coleções de padrões fornecidas com o script, com no máximo 1000 estados abstratos, para resolver cada instância no diretório "nomystery-opt11-strips".
2. Execute a implementação do exercício (d) utilizando a heurística "planopt cpdbs(patterns=C)".
3. Utilize a mesma coleção de padrões C da implementação interna.
4. Compare as duas implementações e discuta o tempo total e o número de estados expandidos excluindo a última camada F ("Expandido até o último salto").
5. Compare os resultados ao usar uma única heurística de banco de dados de padrões com até 1000 estados abstratos, como no exercício (c).

Usar os comandos:

\`\`\`bash
python3 ./fast-downward.py ./nomystery-opt11-strips/p01.pddl --search "astar(pdb(pattern=greedy(1000)))"
python3 ./fast-downward.py ./nomystery-opt11-strips/p01.pddl --search "astar(planopt_cpdbs(patterns=[[0, 2, 3, 4]]))"
\`\`\`
