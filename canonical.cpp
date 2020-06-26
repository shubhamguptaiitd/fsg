
#include <iostream>
#include<vector>
#include "string"
#include <algorithm>
#include <map>
#include <sstream>
#include<set>

using namespace std;

//string A[3][3] = {"","x","y","x","","x","y","x",""};
//string V[3]={"b","a","c"};
//vector<vector<string>>A{{"","x","y"},{"x","","x"},{"y","x",""}};
//vector<string>V{"b","a","c"};
//vector<string>Deg{"2","2","2"};

//vector<vector<string>>A{{"","x","y"},{"x","","x"},{"y","x",""}};
//vector<string>V{"b","a","c"};
// vector<vector<string>>A{{"","x","","","",""},{"x","","y","","y",""},{"","y","","y","y",""},{"","","y","","y","z"},{"","y","y","y","",""},{"","","","z","",""}};
// vector<string>V{"a","b","b","b","b","a"};
// vector<string>Deg{"1","3","3","3","3","1"};//
//
//vector<vector<string>>A{{"","x"},{"x",""}};
//vector<string>V{"b","a"};
//vector<string>Deg{"1","1"};//

void print(vector<vector<string>>A){
    for (int i =0 ; i <A.size();i++){
        cout<<"\n";
        for (int j=0;j<A[0].size();j++){
            if( A[i][j]=="")
                cout<<"0"<<" ";
            else
                cout<<A[i][j]<<" ";
            }
        }
    }
void replacee(vector<vector<string>> &vec, string oldd, string neww)
{
    for (vector<string> &v : vec) // reference to innver vector
    {
        replace(v.begin(), v.end(), oldd, neww); // standard library algorithm
    }


}

string myLabel(vector<vector<string>>&A, vector<string> &V){
    replacee(A,"","0");
    string LabelOut ="";
    for (auto i : V)
    {LabelOut+=i;
     }

    vector< vector<string> >::iterator row;
    vector<string>::iterator col;
    int rows_counter =0 ;
    int nrows = A.size();
    int ncols = A[0].size();
    for (int j =1 ; j <ncols;j++){
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
        index_array_for_perm.push_back(len_array);

        //cout << "\n" << (*it).first;
    }


    std::transform( index_array_for_perm.begin(), index_array_for_perm.end(),
                    index_array_for_perm.begin(), std::bind2nd( std::plus<int>(), -1 ) );




}


set<vector<int>> permutations(vector<int> array){
    set<vector<int>> final;


    do {
        vector<int> temp;
        std::vector<int>::iterator it;
        for (it=array.begin(); it!=array.end(); ++it)
        {
            temp.push_back(*it);
        }
           // std::cout << ' ' << *it;

        final.insert(temp);

    } while ( std::next_permutation(array.begin(),array.end()) );
    return final;
}


string myCanonicalLabel(vector<vector<string>>A, vector<string> V, vector<string>Deg ){

    int x = A.size();
    int y = A[0].size();

    //not to be used below 2
    vector<int> indices1 ;
    vector<int> p ;

    myPartition(A,V,Deg,p, indices1);

    vector<string> Vtemp(x);
    for (int i =0 ; i <x; i++){
        Vtemp[i]= V[p[i]];

    }
    V = Vtemp;

    //vector<vector<string>> vect1(x, y);
    //vector< vector<int> > v(x, vector<int> (y, 0));
    std::vector<std::vector<string> > A1(x, std::vector<string>(y));
    for (int i =0; i <x;i++){
        for(int j = 0; j<y;j++){
            A1[i][j] = A[p[i]][p[j]];
        }
    }
    A=A1;
    print(A1);

//    set<vector<int>> perm_set=permutations(temp);

    set<vector<int>> mypermu;
    int temp=0;
    for (auto i : indices1){
        set<vector<int>> mypermutemp1(mypermu);
        mypermu.clear();
        vector<int> range_temp_array;
        for (int t1 =temp; t1<i+1;t1++){
            range_temp_array.push_back(t1);
        }
        set<vector<int>> mypermutemp2(permutations(range_temp_array));
        if (mypermutemp1.empty()){
            mypermu = mypermutemp2;
        }
        else {
                    std::set<std::vector<int> >::iterator it1;
                    for (it1 = mypermutemp1.begin(); it1 != mypermutemp1.end(); it1++)
                    {
                        std::vector<int> vec1 = (*it1);

                        std::set<std::vector<int> >::iterator it2;
                        for (it2 = mypermutemp2.begin(); it2 != mypermutemp2.end(); it2++) {
                            std::vector<int> vec2 = (*it2);
                            std::vector<int> vec3(vec1);
                            vec3.insert(vec3.end(), vec2.begin(), vec2.end());
                            mypermu.insert(vec3);
                        }
                    }

              }


        temp = i+1;

    }
    string CANcode="";
    string tempCAN="";

    std::set<std::vector<int> >::iterator it;
    for (it = mypermu.begin(); it != mypermu.end(); it++) {

        vector<string> Vcopy(x);
        std::vector<int> p_vec = (*it);

        for (int i =0 ; i <x; i++){
            Vcopy[i]= V[p_vec[i]];

        }

        std::vector<std::vector<string> > A1(x, std::vector<string>(y));
        for (int i =0; i <x;i++){
            for(int j = 0; j<y;j++){
                A1[i][j] = A[p_vec[i]][p_vec[j]];
            }
        }
        CANcode=myLabel(A1,Vcopy);
        if(CANcode>tempCAN){
            tempCAN=CANcode;
        }

    }
    return tempCAN;
}

int main() {
    vector<vector<string>>A{{"","x","","","",""},{"x","","y","","y",""},{"","y","","y","y",""},{"","","y","","y","z"},{"","y","y","y","",""},{"","","","z","",""}};
    vector<string>V{"a","b","b","b","b","a"};
    vector<string>Deg{"1","3","3","3","3","1"};

    cout << myCanonicalLabel(A,V,Deg);
    return 0;
}
