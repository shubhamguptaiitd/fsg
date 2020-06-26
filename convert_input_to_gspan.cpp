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
#include<iostream>
using namespace std;
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
int main(int argc, char** argv){

        ios_base::sync_with_stdio(false);
        string in_file = argv[1];
        string out_file = argv[2];
        ifstream inFile;
        inFile.open(in_file); 
        stringstream strStream;
        strStream << inFile.rdbuf(); 
        string str = strStream.str(); 

        vector<string> data = split(str,'\n');

        ofstream ofile(out_file);  
        unordered_map<string, int> label_to_ct;
        int ct_label  = 1;
        int file_ct  = 0;
        while(file_ct < data.size()){

                string line = data[file_ct];
                if (line.size() > 0 && line[0] == '#'){
                        int num_nodes = 0;
                        int num_edges = 0;
                        string graph_id = line.substr(1);
                        ofile << "t # " << graph_id << "\n";
                        file_ct++;
                        num_nodes = stoi(data[file_ct]);
                        for(int i =0;i < num_nodes;i++){
                                file_ct++;
                                string label = data[file_ct];
                                if(label_to_ct.find(label) == label_to_ct.end()){
                                label_to_ct[label] = ct_label;
                                ct_label++;
                                }
                                ofile << "v " << i << " "  << label_to_ct[label] << "\n";
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
                                ofile << "e " << start_node <<" " << end_node << " " << label_to_ct[edge_label] << "\n";
                        }
                }
                file_ct += 1;
        }
}