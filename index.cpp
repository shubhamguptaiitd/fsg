#include <iostream>
#include <fstream>
#include <string>  
#include <vector> 
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include<cmath>
#include<string>
#include<sstream>
#include <chrono> 
#include "functions.h"
///#include "canonical.h"

using namespace std::chrono; 
using namespace boost;
using namespace std;


void prune_using_1_edge_features(vector<graph_type> &graphs,graph_type &g,std::unordered_map<string,std::unordered_set<int> > &feature_index,std::unordered_set<int> &candidate_set,vector<graph_type> &subgraphs)
{
  graph_type::edge_iterator it, end;
  std::unordered_set<string> tmp;
  for(tie(it, end) = edges(g); it != end; ++it){

        string label =  std::to_string(get(edge_name,g)[*it]);
        string start_node = std::to_string(get(vertex_name, g)[source(*it, g)]);
        string end_node = std::to_string(get(vertex_name, g)[target(*it, g)]);
        string index_key  = get_edge_hash(start_node,end_node,label);
        if( tmp.find(index_key) == tmp.end()){
          tmp.insert(index_key);
          if(feature_index.find(index_key)== feature_index.end()){
            std::unordered_set<int> dummy; /// This edge is not found ///
            candidate_set = dummy;
            return;
          }
          else{
            std::unordered_set<int> feature_graphs  = feature_index[index_key];
            clean_candidate_set(candidate_set,feature_graphs);
          }
        }


    }
    return;

}

void prune_using_discriminative_graphs(graph_type &g,std::unordered_set<int> &candidate_set,vector< vector< graph_type> > &feature_graphs,
  vector< vector< std::unordered_set<int>>> &tids_feature_graphs)
{
  for(int i = 2;i < feature_graphs.size();i++){
    if(i <= num_edges(g)){
      for(int j = 0; j < feature_graphs[i].size();j++){
        if(isIsoMorphic(feature_graphs[i][j],g))
          take_intersection_set(candidate_set,tids_feature_graphs[i][j]);
      }
    }
  }
}

vector<int> search_query_in_graphs(graph_type &g, vector<graph_type> &graphs,std::unordered_map<string,std::unordered_set<int> > &feature_index,std::unordered_map<int,int> &graph_index_to_id,
                  vector< vector< graph_type> > &feature_graphs,vector< vector< std::unordered_set<int>>> &tids_feature_graphs,bool flag=true)
{
  std::unordered_set<int> candidate_graphs_id;
  for(int i= 0; i < graphs.size();i++)
    candidate_graphs_id.insert(i);
  vector<graph_type> subgraphs;
  prune_using_1_edge_features(graphs,g,feature_index,candidate_graphs_id,subgraphs);
  if (candidate_graphs_id.size() > 1000){
    cerr << "pruning further using graph index" << endl;
    prune_using_discriminative_graphs(g,candidate_graphs_id,feature_graphs,tids_feature_graphs);
  }
  vector<int> found_in_vector;
  for (auto &it : candidate_graphs_id){
    if (isIsoMorphic(g,graphs[it]))
      found_in_vector.push_back(it);
  }
  if(flag){
    for(int i = 0; i < found_in_vector.size();i++)
      found_in_vector[i] = graph_index_to_id[found_in_vector[i]];
  }
  return found_in_vector;
}



float get_gamma(graph_type &g,std::unordered_set<int> &tids,vector< vector< graph_type> > &feature_graphs,vector< vector< std::unordered_set<int>>> &tids_feature_graphs){
  int size_g = num_edges(g);
  std::unordered_set<int> candidates;
  for(int i = 1; i <=size_g;i++){ // looping across sizes ///
    for(int j = 0; j< feature_graphs[i].size();j++) // looping across all graphs of size i
    {
      if (isIsoMorphic(feature_graphs[i][j],g)){
        if(candidates.size() == 0)
          candidates = tids_feature_graphs[i][j];
        else
          take_intersection_set(candidates,tids_feature_graphs[i][j]);
      }
    }
  }
  
  return candidates.size()*1.00/tids.size();

}
int main(int argc, char** argv){


  string filename = argv[1];
  auto start = high_resolution_clock::now(); 
  std::unordered_map<string,int> label_mp;
  std::unordered_map<int,int> graph_index_to_id;
  vector<graph_type> graphs;
  cerr <<"reading graph database " << endl;
  get_graphs_from_standard_input(filename, graphs,label_mp,graph_index_to_id);

  cerr <<  "Length of graph, " << graphs.size() << endl;
  std::unordered_map<string, std::unordered_set<int> > feature_index;

  cerr << "creating 1 edge feature map " << endl;
  // Create 1 edge hash map //
  graph_type::edge_iterator it, end;
  int graph_ct= 0;
  int max_size = 15;
  vector< vector< graph_type> > feature_graphs(max_size);
  vector< vector< std::unordered_set<int>>> tids_of_feature_graphs(max_size);
  for(auto &g : graphs){
      for(tie(it, end) = edges(g); it != end; ++it){
        
        string label =  std::to_string(get(edge_name,g)[*it]);
        string start_node = std::to_string(get(vertex_name, g)[source(*it, g)]);
        string end_node = std::to_string(get(vertex_name, g)[target(*it, g)]);
        string index_key  = get_edge_hash(start_node,end_node,label);
        if(feature_index.find(index_key)!= feature_index.end())
          feature_index[index_key].insert(graph_ct);
        else{
          feature_index[index_key] = std::unordered_set<int>({graph_ct});
          graph_type g;
          add_vertex(prop_node(stoi(start_node)), g);
          add_vertex(prop_node(stoi(end_node)),g);
          add_edge(0, 1, prop_edge(stoi(label)), g);
          feature_graphs[1].push_back(g);
        }
    //       if (feature_graphs[1].size() == 0){
    //         vector<graph_type> tp;
    //         feature_graphs[1] = tp;

      }
    graph_ct += 1;
  }
  //cerr << " here 1" << endl;
  for(int i = 0 ; i < feature_graphs[1].size();i++){
    graph_type g = feature_graphs[1][i];
    for(tie(it, end) = edges(g); it != end; ++it){
      string label =  std::to_string(get(edge_name,g)[*it]);
      string start_node = std::to_string(get(vertex_name, g)[source(*it, g)]);
      string end_node = std::to_string(get(vertex_name, g)[target(*it, g)]);
      string index_key  = get_edge_hash(start_node,end_node,label);
      //cerr << index_key << feature_index[index_key].size() << endl;
      tids_of_feature_graphs[1].push_back(feature_index[index_key]);
    }

  }

  string gspan_filename = argv[2];
  cerr << "creating frequent feature maps " << endl;
  vector< vector< graph_type> > gspan_feature_graphs(max_size);
  vector< vector< std::unordered_set<int>>> tids_of_gspan_graphs(max_size);

  cerr << "    Reading frequent dataset from " << gspan_filename << endl;
  
  get_graphs_from_gspan_size_wise(gspan_filename,gspan_feature_graphs,tids_of_gspan_graphs);
  cerr << "Time spent , " << get_time_spent_seconds(start) << endl;

  cerr << "Creating discriminative features "<< endl;

  // for(int i = 0; i < 50;i++){
  //   cerr << "Frequent graphs of size " << i << " are " << gspan_feature_graphs[i].size() << " and their frequencies " ;
  //   for(int j = 0;j< gspan_feature_graphs[i].size();j++)
  //     cerr << tids_of_gspan_graphs[i][j].size() << " ";
  //   cerr << endl;
  // }
  int kept= 0;
  int actual = 0;
  float gamma_cut = 1.05000000;
  if(graphs.size() <= 50000)
    gamma_cut = 1.1000000;
  if(graphs.size() <= 40000)
    gamma_cut = 1.200000;
  if(graphs.size() < 30000)
    gamma_cut = 1.300000;
  cerr << "gamma_Cut is " << gamma_cut << endl;
  for(int ct  = 2;ct < max_size;ct++){
    int lkept = 0;
    actual += gspan_feature_graphs[ct].size();
    for(int j = 0; j< gspan_feature_graphs[ct].size();j++ ){
      graph_type g = gspan_feature_graphs[ct][j];
      if(kept <= 500){
      float gamma = get_gamma(g,tids_of_gspan_graphs[ct][j],feature_graphs,tids_of_feature_graphs);

      if( gamma >= gamma_cut){
        kept++;
        lkept++;
        feature_graphs[ct].push_back(g);
        tids_of_feature_graphs[ct].push_back(tids_of_gspan_graphs[ct][j]);
      }
      }
    }
    //cerr <<  "candidates : " << gspan_feature_graphs[ct].size() << " kept  " << lkept << endl;
  }

  
  cerr << "size of feature index  " << kept << " original " << actual << endl;
  cerr << "Time spent , " << get_time_spent_seconds(start) << endl;



  cout << "Indexing complete";
  std::cout.flush();



//########################################################################################//
  string query_file, query_output_file;
  cin >> query_file >> query_output_file;
  start = high_resolution_clock::now(); 
  cerr << "starting query processing " << endl;
  

  std::vector<int> query_supports;
  vector<graph_type> qgraphs;
  cerr <<"reading graph database from " << query_file << endl;
  get_query_graphs_from_standard_input(query_file, qgraphs,label_mp,query_supports);


  ofstream ofout(query_output_file);  
  cerr << "size of query  graphs " << qgraphs.size() << endl;
  for(int i = 0; i < qgraphs.size();i++){

    //cerr << "processing query " << i << endl;
    graph_type g = qgraphs[i];
    //cerr << "size of graph "<< num_edges(g)<< endl;
    vector<int> found_in = search_query_in_graphs(g,graphs,feature_index,graph_index_to_id,feature_graphs,
                                                  tids_of_feature_graphs,true);
    
    if (found_in.size() > 0)
      ofout << get_string_from_vector(found_in,"#") << endl;
    else
      ofout << endl;
    cerr << "found in ," << found_in.size() << endl;
    cerr << "actual ," << query_supports[i];
    cerr << "Time spent , " << get_time_spent_seconds(start) << endl;
  }
  cerr << "Time spent , " << get_time_spent_seconds(start) << endl;
  ofout.close();

  return 0;

}
