#ifndef functions_h
#define functions_h


#include<iostream>
#include<algorithm>
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
#include<cstdio>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std::chrono; 
using namespace std;
using namespace boost;


/// For graph operations, some code/snippet taken from offical documentation of boost library ///
/// https://www.boost.org/doc/libs/1_63_0/libs/graph/doc/vf2_sub_graph_iso.html 
typedef property<edge_name_t, int> prop_edge;
typedef property<vertex_name_t, int, property<vertex_index_t, int> > prop_node;
typedef adjacency_list<vecS, vecS, undirectedS, prop_node, prop_edge> graph_type;
typedef property_map<graph_type, vertex_name_t>::type vertex_name_map_t;
typedef property_map_equivalent<vertex_name_map_t, vertex_name_map_t> vertex_comp_t;
typedef property_map<graph_type, edge_name_t>::type edge_name_map_t;
typedef property_map_equivalent<edge_name_map_t, edge_name_map_t> edge_comp_t;

template<typename Graph1, typename Graph2> struct vf2_callback{
    
    vf2_callback(Graph1 g, Graph2 f){
    }
    template <typename CorrespondenceMap1To2,
          typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1 g) const {
        return false;
    }
};

bool isIsoMorphic(graph_type G1, graph_type G2){

    vertex_comp_t vertex_comp =
        make_property_map_equivalent(get(vertex_name, G1), get(vertex_name, G2));
    edge_comp_t edge_comp =
        make_property_map_equivalent(get(edge_name, G1), get(edge_name, G2));
    vf2_callback<graph_type , graph_type> callback(G1,G2);
    return vf2_subgraph_mono(G1, G2, callback, vertex_order_by_mult(G1),
                       edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
}

void print_string_int_map(std::unordered_map<string,int> &map){
    for(auto &it: map){
        cerr << it.first << " " << it.second << endl;
  }
}

void print_string_int_int(std::unordered_map<int,int> &map){
    for(auto &it: map){
        cerr << it.first << " " << it.second << endl;
  }
}
vector<std::string> split(std::string &strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}


void get_graphs_from_standard_input(string filename, vector<graph_type> &graphs,std::unordered_map<string,int> &label_to_ct,std::unordered_map<int,int> &graph_index_to_id){
    ifstream inFile;
    inFile.open(filename); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data = split(str,'\n');

    int file_ct = 0;
    int ct_graph = 0;
    int ct_label = 1;
    while(file_ct < data.size()){

        string line = data[file_ct];
        if (line.size() > 0 && line[0] == '#'){

            graph_type G;
            int num_nodes = 0;
            int num_edges = 0;
            int graph_id = stoi(line.substr(1));

            file_ct++;
            num_nodes = stoi(data[file_ct]);
            for(int i =0;i < num_nodes;i++){
                file_ct++;
                string label = data[file_ct];
                if(label_to_ct.find(label) == label_to_ct.end()){
                label_to_ct[label] = ct_label;
                ct_label++;
                }
                add_vertex(prop_node(label_to_ct[label]), G);
            }

            file_ct++;
            num_edges = stoi(data[file_ct]);
            for(int i = 0;i < num_edges;i++){
                int start_node;
                int end_node;
                string edge_label;
                file_ct ++;
                istringstream line_stream(data[file_ct]);
                line_stream >> start_node >> end_node >> edge_label;
                if (start_node > end_node){
                int tmp = start_node;start_node = end_node; end_node =tmp;
                }

                if(label_to_ct.find(edge_label) == label_to_ct.end()){
                label_to_ct[edge_label] = ct_label;
                ct_label += 1;
                }

                add_edge(start_node, end_node, prop_edge(label_to_ct[edge_label]), G); 
            }

            graphs.emplace_back(G);
            graph_index_to_id[ct_graph] = graph_id;
            ct_graph++;

        }
        file_ct += 1;

        
    }
    return ;
}


void get_query_graphs_from_standard_input(string filename, vector<graph_type> &graphs,std::unordered_map<string,int> &label_to_ct,std::vector<int> &support){
    ifstream inFile;
    inFile.open(filename); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    //cerr << str << endl;
    vector<string> data = split(str,'\n');

    int file_ct = 0;
    int ct_graph = 0;
    int ct_label = label_to_ct.size()+1;
    while(file_ct < data.size()){

        string line = data[file_ct];
        if (line.size() > 0 && line[0] == '#'){

            graph_type G;
            int num_nodes = 0;
            int num_edges = 0;
            int graph_id = std::stoi(line.substr(1));
            file_ct++;
            num_nodes = stoi(data[file_ct]);
            for(int i =0;i < num_nodes;i++){
                file_ct++;
                string label = data[file_ct];
                if(label_to_ct.find(label) == label_to_ct.end()){
                label_to_ct[label] = ct_label;
                ct_label++;
                }
                add_vertex(prop_node(label_to_ct[label]), G);
            }

            file_ct++;
            if(data[file_ct][0] != '\n' && data[file_ct][0]!= '#'){
                num_edges = stoi(data[file_ct]);
                for(int i = 0;i < num_edges;i++){
                    int start_node;
                    int end_node;
                    string edge_label;
                    file_ct ++;
                    istringstream line_stream(data[file_ct]);
                    line_stream >> start_node >> end_node >> edge_label;
                    if (start_node > end_node){
                    int tmp = start_node;start_node = end_node; end_node =tmp;
                    }

                    if(label_to_ct.find(edge_label) == label_to_ct.end()){
                    label_to_ct[edge_label] = ct_label;
                    ct_label += 1;
                    }

                    add_edge(start_node, end_node, prop_edge(label_to_ct[edge_label]), G); 
                }
            }
            else
                file_ct = file_ct -1;

            graphs.emplace_back(G);
            support.push_back(graph_id);
            ct_graph++;

        }
        file_ct += 1;

        
    }
    return ;
}

void get_graphs_from_gspan(string filename, vector<graph_type> &graphs,vector< vector<int> > &tid_graphs,vector<int> &supports,int needed_graphs){
    ifstream inFile;
    inFile.open(filename); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data = split(str,'\n');

    int file_ct = 0;
    int ct_graph = 0;
    int tmp;
    while(file_ct < data.size()){
        string line = data[file_ct];
        if (line.size() > 0 && line[0] == 't'){

            graph_type G;

            int tid , support_ct;
            string str_tp;
            string tid_s,support_ct_s;
            istringstream line_stream(line);
            line_stream >> str_tp >> str_tp >> tid_s >> str_tp >> support_ct_s;
            tid = stoi(tid_s);
            support_ct = stoi(support_ct_s);
            supports.emplace_back(support_ct);
            file_ct++;
            int tp1,tp2;
            int tp3;
            while(data[file_ct][0]=='v'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2;
                add_vertex(prop_node(tp2), G);
                file_ct ++;
            }

            while(data[file_ct][0] =='e'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2 >> tp3;
                if(tp1 > tp2){
                    int tmp = tp1;tp1 = tp2;tp2=tmp;
                }
                add_edge(tp1, tp2, prop_edge(tp3), G); 
                file_ct++;
            }
            if(data[file_ct][0] =='x'){
                vector<int> tids;
                istringstream line_stream(data[file_ct].substr(1));
                while(line_stream >> tp1){
                    tids.push_back(tp1);
                }
                tid_graphs.emplace_back(tids);

            }



            graphs.emplace_back(G);
            ct_graph++;
            if(ct_graph > needed_graphs)
                break;
        }
        file_ct++;

        
    }
    return ;
}

void get_graphs_from_gaston(string filename, vector<graph_type> &graphs){
    ifstream inFile;
    inFile.open(filename); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data = split(str,'\n');
    int file_ct = 0;
    int ct_graph = 0;
    int tmp;
    while(file_ct < data.size()){
        string line = data[file_ct];
        //cerr << line << endl;
        if (line.size() > 0 && line[0] == 't'){
            graph_type G;

            int tid , support_ct;
            string str_tp;
            string tid_s,support_ct_s;
            //istringstream line_stream(line);
            //line_stream >> str_tp >> str_tp >> tid_s >> str_tp >> support_ct_s;
            //tid = stoi(tid_s);
            //support_ct = stoi(support_ct_s);
            //supports.emplace_back(support_ct);
            file_ct++;
            int tp1,tp2;
            int tp3;
            while(data[file_ct][0]=='v'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2;
                add_vertex(prop_node(tp2), G);
                file_ct ++;
            }

            while(file_ct < data.size() && data[file_ct][0] =='e'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2 >> tp3;
                if(tp1 > tp2){
                    int tmp = tp1;tp1 = tp2;tp2=tmp;
                }
                add_edge(tp1, tp2, prop_edge(tp3), G); 
                file_ct++;
            }



            graphs.emplace_back(G);
            ct_graph++;

        }
        file_ct++;

        
    }
    return ;
}


int get_time_spent_seconds(high_resolution_clock::time_point start){

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<seconds>(stop - start); 
    return duration.count();
}


string get_edge_hash(string &node1,string &node2, string &edge_label){
  string hash_key  = "";
  if(node1 < node2) 
    hash_key += node1+"#"+node2;
  else
    hash_key += node2+"#"+node1;
  return hash_key+"#"+edge_label;
}
void clean_candidate_set(std::unordered_set<int> &candidates,std::unordered_set<int> &feature_graphs){
    vector<int> removal_set;
    for(auto it:candidates){
      if(feature_graphs.find(it) == feature_graphs.end())
        removal_set.push_back(it);
    }
    for (auto it : removal_set){
      candidates.erase(it);
    }
}
void take_intersection_set(std::unordered_set<int> &s1,std::unordered_set<int> &s2){
    vector<int> removal_set;
    for(auto it: s1){
        if(s2.find(it) == s2.end())
            removal_set.emplace_back(it);
    }
    for(auto it:removal_set)
        s1.erase(it);
}






void get_graph_adjacency_format(graph_type &g,std::vector<vector<string> > &A,std::vector<string> &V,std:: vector<string> &Deg )
{
  std::vector<vector<string> > AA(num_vertices(g), vector<string>(num_vertices(g),"0"));
  vector<string> VV(num_vertices(g));
  vector<string> Degg(num_vertices(g));
  V = VV;Deg = Degg;A = AA;
  graph_type::vertex_iterator vertexIt, vertexEnd; 
  tie(vertexIt, vertexEnd) = vertices(g);
  for (; vertexIt != vertexEnd; ++vertexIt) {
    int ind = get(vertex_index,g)[*vertexIt];
    V[ind] = std::to_string(get(vertex_name,g)[*vertexIt]);
    Deg[ind] = std::to_string(degree(*vertexIt,g));
  }
  graph_type::edge_iterator it, end;
  for(tie(it, end) = edges(g); it != end; ++it){
    int start_node  = get(vertex_index, g)[source(*it, g)];
    int end_node = get(vertex_index, g)[target(*it, g)];
    if(start_node > end_node){
      int tp = start_node;start_node=end_node;end_node=start_node;
    }

    A[start_node][end_node] = std::to_string(get(edge_name,g)[*it]);
    A[end_node][start_node] = std::to_string(get(edge_name,g)[*it]);
  }

}


string get_string_from_vector(vector<int> &vec,string app){

    std::stringstream ss;
    for(size_t i = 0; i < vec.size(); ++i)
    {
    if(i != 0)
        ss << "\t";
    ss << app << vec[i];
    }
    std::string s = ss.str();
    return s;
}


void get_graphs_from_gspan_size_wise(string filename, vector< vector< graph_type> > &graphs,vector< vector< std::unordered_set<int>>> &tid_graphs){
    ifstream inFile;
    inFile.open(filename); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data = split(str,'\n');
    int max_size = graphs.size();
    cerr <<" maximum size to be stored " << max_size << endl;
    int file_ct = 0;
    int ct_graph = 0;
    int tmp;
    while(file_ct < data.size()){
        string line = data[file_ct];
        if (line.size() > 0 && line[0] == 't'){

            graph_type G;

            int tid , support_ct;
            string str_tp;
            string tid_s,support_ct_s;
            istringstream line_stream(line);
            line_stream >> str_tp >> str_tp >> tid_s >> str_tp >> support_ct_s;
            tid = stoi(tid_s);
            support_ct = stoi(support_ct_s);

            file_ct++;
            int tp1,tp2;
            int tp3;
            while(data[file_ct][0]=='v'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2;
                add_vertex(prop_node(tp2), G);
                file_ct ++;
            }

            while(data[file_ct][0] =='e'){
                istringstream line_stream(data[file_ct].substr(1));
                line_stream >> tp1 >> tp2 >> tp3;
                if(tp1 > tp2){
                    int tmp = tp1;tp1 = tp2;tp2=tmp;
                }
                add_edge(tp1, tp2, prop_edge(tp3), G); 
                file_ct++;
            }
            std::unordered_set<int> tids;

            if(data[file_ct][0] =='x'){
                istringstream line_stream(data[file_ct].substr(1));
                while(line_stream >> tp1){
                    tids.insert(tp1);
                }

            }

            int edge_ct = num_edges(G);
            if (edge_ct < max_size){
                graphs[edge_ct].push_back(G);
                tid_graphs[edge_ct].push_back(tids);
            }

            ct_graph++;

        }
        file_ct++;

        
    }
    return ;
}


#endif
