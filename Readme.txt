COL 761 - Query SubGraph  : assignment - 2



Team consist of -
Sahil Manchanda - 2018CSZ8551
Raj Kamal - 2018CSZ8013
Shubham Gupta - 2019CSZ8470



compile.sh - running it generate executables which will be used by further programs !!!
plot.sh - running it will generate running time graph for FSG,Gspan and Gaston. 
./plot.sh <graphs dataset file> <plot output file>

index_and_query.sh -  this program first transform input file to gspan format file. Then gspan will be run on this file with support varied according to length of file. then ./index will called to generate index and answer the queries.




convert_input_to_gspan.cpp- will transform the input file to gspan format
functions.h - helper functions for various tasks
index.cpp - contains the structure for creating the index and then answering the query.

canonical.cpp - not used in final code but originally designed for canonical label generation.

plot_3.png - plot of fsg,gspan and gaston running times vs support. 


Observation - 
1. The running time increases with decrease in support .
2. The decrease in time when decreasing support is high (exponential) for gspan and fsg compared to Gaston(linear)
3. In most cases gspan is faster than fsg.
4. Gaston is very fast compared to gspan and fsg on lower support. 

Reasons:
1. At lower threshold, the frequent subgraphs mined are high( i.e more fragments) . This increases in an exponential fashion.
 

2. FSG uses a bfs startegy that creates candidate subgraphs of size k+1 using k size subgraphs and then prunes the infrequent ones.
On the other hand, gspan do not generate candidates due to construction of DFS Code tree and follow depth first based pattern generation like fp growth.  and thus do not require pruning.



Gaston performs the best as it proceeds mining paths and then mining subtrees and then mining subgraphs with cycles. 
The expensive subgraphs isomorphism testing occurs only in subgraph mining phase. The idea behind this is that for graphs, polynomial time algorithms are not available but for paths and trees, efficient isomorphism algorithms exist.
For graphs,Gaston algorithm generates cycles that are larger than previously generated graphs cycles.


Algorithm for current graph indexing and query searching approach - 
a) First we stores the edges which are occurring in graph db and corresponding graph ids.
b) Now we mine the frequent subgraphs using gspan.
c) Now we need to eliminate those frequent subgraphs which don't act as discriminator.
  We define such parameter gamma (https://sites.cs.ucsb.edu/~xyan/papers/sigmod04_gindex.pdf)
   gamma for feature subgraph X = 
	| set of intersection of graph ids contained by subgraph features(already in index) of X| / | graph ids contained by X|

	if this gamma == 1, then this feature is completely useless
	if gamma >> 1, then this feature is highly discriminative.
   We take gamma = 1.05 as we are doing this for 20% or higher support count of feature subgraphs.
d) On query time, we first prune candidate list by using 1 edge index and then check for the feature index, which graphs are isomorphic to query and further prune the candidate list.
e) finally, perform sub graph isomorphism test on all remaining graph patterns.


Team contribution- 
Everyone had a equal contribution. Everyone brainstormed the algorithm. Sahil and Shubham worked on c++ code implementation , Rajkamal worked parallely on python code using graph tool in case c++ code doesn't pan out and also implemented canonical labelling code.



Running issues - 
gSpan also runs some statements during its processing time , so please take care of that during evaluation. 


 

		


