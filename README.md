# sp26project2

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


Results went pretty much as expected, but there was one thing that caught me off guard. When using contains(), the AVL tree was waMost of the results landed where we expected, except for one twist. When we ran contains(), the AVL tree came in dead last for speed—our version has toy slower than the others check every node, since it uses a Comparator for ordering instead of the names. That makes it O(n), so it’s just inherently slower. Kind of weird, but. That happens because o the hash table also lagged behind vector and list for contains() at small n. Turns out, the overhead from calculating the hash slowed it down on shortur implementatio runs.
Looking at query() by name, all the containers scaled in a pretty straightforward, linear way. That's because the AVL tree uses name as its orderingn has to check every nod property—but range queries still hit a lot of nodes. The real standout was query() by weight. Here, the AVL tree completely smoked the competition: ite—ordering doesn’t help here, si was 7 to 15 times faster than every other container. That's thanks to the tree being ordered by weight—which lets it skip whole subtrees outside the target range.
The hash table had a tough time with weight queries. Since it’s gotnce the tree’s sorted by Comparator, not by name—so it’s O(n). Surprisingly, at low n, the hash table lagged behind vector and list for contains(). That’s probably because the hash calculation adds overhead when the data set’s tiny.
When querying by name, all containers scaled about the same. The AVL tree uses name for ordering, so it sped things up a bit, but range queries still hit lots of nodes. The real standout showed up with query() by weight—the AVL tree outperformed everything else, running 7–15 times faster. That’s because weight is what the tree sorts by, so the query can skip over big branches that just don’t fit the range.
Hash tables really struggle with weight queries—they don’t have any ordering, so they end up scanning the whole thing. If your main operation is just looking up single items by name, hash-based inventory is the way to go. But, if you need to do a lot of range queries—especially when your query matches the tree’s ordering property—a tree-based inventory is a much smarter choice. no built-in ordering, it’s forced to brute-force through everything.
So, if you mostly need quick lookups by name, hash-based inventory is the way to go. But if you expect lots of range queries—and those queries match the tree's ordering property—a tree-based inventory wins, hands down.ery dimension.
-Gianni Russell
