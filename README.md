# sp26project2
Based on your timing results, here's what to write:

Task 4A: contains() Average Time (ms)
Container	n=1000	n=2000	n=4000	n=8000
vector	0.000031	0.000030	0.000030	0.000030
list	0.000030	0.000030	0.000032	0.000031
unordered_set	0.000117	0.000117	0.000127	0.000175
AVL tree	0.007462	0.068151	0.029878	0.071031

Task 4B Test 1: query() by Name Average Time (ms)
Container	n=1000	n=2000	n=4000	n=8000
vector	0.059786	0.124750	0.250561	0.814647
list	0.079926	0.125677	0.300761	1.693232
unordered_set	0.059466	0.126228	0.271015	1.055166
AVL tree	0.044551	0.121930	0.212892	0.715169

Task 4B Test 2: query() by Weight Average Time (ms)
Container	n=1000	n=2000	n=4000	n=8000
vector	0.007317	0.013310	0.029279	0.058046
list	0.008178	0.015755	0.928396	0.094229
unordered_set	0.008406	0.017113	0.037603	0.125747
AVL tree	0.000848	0.001953	0.003663	0.008004

Analysis paragraph:

The results were largely expected, with one surprise. For contains(), the AVL tree was the slowest by far — this is because our implementation must search every node regardless of ordering (since ordering is by Comparator, not name), making it O(n). Counterintuitively, the hash table was slightly slower than vector and list for contains() at small n, likely due to hash computation overhead on short runs. For query() by name, all containers scaled roughly linearly since name is the ordering property for the AVL tree but the range queries still touched many nodes. The biggest difference appeared in query() by weight, where the AVL tree was 7–15x faster than every other container — this is because weight is the tree's ordering property, allowing the helper to prune entire subtrees outside the range. The hash table was worst for weight queries since it has no ordering and must scan everything. In practice, a hash-based inventory is the right choice when the primary operation is single-item lookup by name. A tree-based inventory is superior when range queries are frequent, especially when the ordering property matches the query dimension.
