#ifndef canonical_h
#define canonical_h
#include <iostream>
#include<vector>
#include "string"
#include <algorithm>
#include <map>
#include <sstream>
#include<set>

using namespace std;


// string get_canonical_label(graph_type &g)
// {
//   vector< vector<string> > A;
//   vector< string> labels;
//   vector<string> deg;
//   get_graph_adjacency_format(g,A,labels,deg);
//   return myCanonicalLabel(A,labels,deg);

// }
// string get_canonical_label_print(graph_type &g)
// {
//   vector< vector<string> > A;
//   vector< string> labels;
//   vector<string> deg;
//   get_graph_adjacency_format(g,A,labels,deg);
//   return myCanonicalLabel(A,labels,deg);

// }
void print_vector_vector_string(vector<vector<string>>A){
        cout << "{";
    for (int i =0 ; i <A.size();i++){
        cout << "{";
        for (int j=0;j<A[0].size();j++)
                
                cout<< '"' <<A[i][j]<<'"' << ",";
        cout << "},";
    }
    cout << "}" << endl;
}
void print_vector_string(vector<string> &A){
        cout <<"{";
        for (int i = 0;i < A.size();i++){
                cout <<'"' <<A[i] << '"' << ",";
        }
        cout << "}" <<endl;
}
string myLabel(vector<vector<string>>&A, vector<string> &V){
   // replacee(A,"","0");
    string LabelOut ="";
    for (auto i : V)
    {LabelOut+=i;
     }

    vector< vector<string> >::iterator row;
    vector<string>::iterator col;
    int rows_counter =0 ;
    int size = A.size();
    for (int j =1 ; j <size;j++){
        for (int i =0; i<j;i++ ){
            LabelOut+=A[i][j];

        }
    }
    return LabelOut;
}

void myPartition(vector<vector<string>>&A, vector<string> &V, vector<string>&Deg,vector<int>&perm, vector<int>&index_array_for_perm){

    map<string, vector<int>, greater <string>> dict_hash;

    for (int i =0 ; i <A.size();i++){
        string myDeg = Deg[i];
        string myLabel = V[i];
        string key=myDeg+myLabel;
        vector<string> row = A[i];
        vector<string> neighbor_list;
        int col =0;
        for (;col<A[i].size();col++){
            if (A[i][col]!=""){
                string neighbor_code = A[i][col] + Deg[col] + V[col];
                neighbor_list.push_back(neighbor_code);
            }

        }
        sort(neighbor_list.begin(), neighbor_list.begin()+neighbor_list.size(),greater<string>());

        std::stringstream ss;
        for(size_t i = 0; i < neighbor_list.size(); ++i)
        {
            ss << neighbor_list[i];
        }
        std::string joined_neighbor_list = ss.str();
        key+= joined_neighbor_list;
        //cout<<"\n "<<key;

        if (dict_hash.find(key)== dict_hash.end()){
            dict_hash[key];

        }

        dict_hash[key].push_back(i);
    }
    map<string,vector<int>> :: iterator it;

    int len_array=0;
    for (it=dict_hash.begin() ; it!=dict_hash.end() ; it++){
        string key = (*it).first;
        vector<int> value_array = (*it).second;
        perm.insert(perm.end(), value_array.begin(), value_array.end());
        len_array+=value_array.size();
        index_array_for_perm.emplace_back(len_array-1);

        //cout << "\n" << (*it).first;
    }


   // std::transform( index_array_for_perm.begin(), index_array_for_perm.end(),
      //             index_array_for_perm.begin(), std::bind2nd( std::plus<int>(), -1 ) );



    //cout<<"\n";

}


vector<vector<int>> permutations(vector<int> array){
    vector<vector<int>> final;


    do {
        vector<int> temp;
        std::vector<int>::iterator it;
        for (it=array.begin(); it!=array.end(); ++it)
        {
            temp.push_back(*it);
        }
           // std::cout << ' ' << *it;

        final.push_back(temp);

    } while ( std::next_permutation(array.begin(),array.end()) );
    return final;
}


string myCanonicalLabel(vector<vector<string>>A, vector<string> V, vector<string>Deg ){

    int size = A.size();

    //not to be used below 2
    vector<int> indices1 ;
    vector<int> p ;

    myPartition(A,V,Deg,p, indices1);
    //cout<<"done";

    vector<string> Vtemp(size);
//    for (int i =0 ; i <x; i++){
//        Vtemp[i]= V[p[i]];
//
//    }
//    V = Vtemp;
    //cout<<"dione";

    //vector<vector<string>> vect1(x, y);
    //vector< vector<int> > v(x, vector<int> (y, 0));
    std::vector<std::vector<string> > A1(size, std::vector<string>(size));
    for (int i =0; i <size;i++){
        Vtemp[i]= V[p[i]];
        for(int j = 0; j<size;j++){
            A1[i][j] = A[p[i]][p[j]];
        }
    }
    V = Vtemp;

    A=A1;
    ///cout<<"";
 //   print(A1);

//    set<vector<int>> perm_set=permutations(temp);

    vector<vector<int>> mypermu;
    int temp=0;
    for (auto i : indices1){
        vector<vector<int>> mypermutemp1(mypermu);
        mypermu.clear();
        vector<int> range_temp_array;
        for (int t1 =temp; t1<i+1;t1++){
            range_temp_array.emplace_back(t1);
        }
        vector<vector<int>> mypermutemp2(permutations(range_temp_array));
        if (mypermutemp1.empty()){
            mypermu = mypermutemp2;
        }
        else {
                    std::vector<std::vector<int> >::iterator it1;
                    for (it1 = mypermutemp1.begin(); it1 != mypermutemp1.end(); it1++)
                    {
                        //std::vector<int> vec1 = (*it1);

                        std::vector<std::vector<int> >::iterator it2;
                        for (it2 = mypermutemp2.begin(); it2 != mypermutemp2.end(); it2++) {
                            vector<int>vec3(*it1);

                            //std::vector<int> vec2 = (*it2);
                           // std::vector<int> vec3(vec1);
                            vec3.insert(vec3.end(), (*it2).begin(), (*it2).end());
                            mypermu.emplace_back(vec3);
                            //cout<<"";
                        }
                    }

              }


        temp = i+1;

    }
    string CANcode="";
    string tempCAN="";

    std::vector<std::vector<int> >::iterator it;
    std::vector<std::vector<string> > A2(size, std::vector<string>(size));
    vector<string> Vcopy(size);

    for (it = mypermu.begin(); it != mypermu.end(); it++) {
        //cout<<"\n";

      //  vector<string> Vcopy(size);
        std::vector<int> p_vec = (*it);
//
//        for (int i =0 ; i <x; i++){
//            Vcopy[i]= V[p_vec[i]];
//
//        }

        for (int i =0; i <size;i++){
            Vcopy[i]= V[p_vec[i]];
            for(int j = 0; j<size;j++){
                A2[i][j] = A[p_vec[i]][p_vec[j]];
            }
        }
        CANcode=myLabel(A2,Vcopy);
        if(CANcode>tempCAN){
            tempCAN=CANcode;
        }

    }
    //cout<<" tempCAN "<< tempCAN;
    return tempCAN;
}

  // int graph_it = 0;
  // int ctp=0;
  // for (auto &g : fgraphs){
  //   if (num_edges(g) > 1 && num_edges(g) <= 5){
  //     cerr << num_edges(g)<< " " << endl;
  //     std::unordered_set<int> tp(tid_fgraphs[graph_it].begin(),tid_fgraphs[graph_it].end());
  //     feature_index[get_canonical_label(g)] = tp;
  //     //cerr << num_edges(g) << " " ;
  //     ctp += 1;
  //   }
  //   graph_it++;
  // }
  // cerr << " number of graphs " << graph_it << " " << ctp<< endl;
  // cerr << "Time spent in cononicalizing  , " << get_time_spent_seconds(start) << endl;

#endif
