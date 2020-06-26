import sys
graph_db = sys.argv[1]
gspan_out = sys.argv[2]
query_file = sys.argv[3]
result_file = sys.argv[4]
import json

def get_label_dict(infile):
        fin = open(infile,"r")
        item = fin.readline()
        label_dict = {}
        num_graphs = 0
        ct = 1
        EOF = False
        while not EOF:
            if item[0] == '#':
                num_graphs += 1
                num_nodes = int(fin.readline().split()[0])
                for i in range(num_nodes):
                    label = fin.readline().split()[0]
                    if label in label_dict:
                        label  = label_dict[label]
                    else:
                        label_dict[label] =ct
                        label = ct
                        ct += 1
                num_edges = int(fin.readline().split()[0])
                for i in range(num_edges):
                    line = fin.readline().split()
                    label = line[2]
                    if label in label_dict:
                        label  = label_dict[label]
                    else:
                        label_dict[label] =ct
                        label = ct
                        ct += 1
            item = fin.readline()
            if not item:
                fin.close()
                return label_dict 

def create_query_and_result_file(gspan_out,query_file,result_file,label_dict):
    fin = open(gspan_out,"r")
    foquery = open(query_file,"w")
    fresult = open(result_file,"w")
    data = fin.read().splitlines()
    ct_graph = 0
    file_index = 0
    write_mod = 9998
    while file_index < len(data):
        item = data[file_index]
        if len(item)> 0 and item[0] == 't':
            support  = item.split("*")[-1].split()[-1]
            #print (item)
            ## Number of nodes
            file_index += 1
            num_nodes = 0 
            node_labels = []
            while data[file_index][0] == 'v':
                num_nodes += 1
                node_labels.append(label_dict[int(data[file_index].split()[-1])])
                file_index += 1


            ### Num of edges
            edges = []
            edge_labels = []
            while data[file_index][0] == 'e':
                line = data[file_index].split()
                edges.append((int(line[1]),int(line[2])))
                edge_labels.append(label_dict[int(line[3])])
                file_index+=1

            ### Write edges and labels ####
            if ct_graph%write_mod == 0:
                foquery.write("#"+support+"\n")
                foquery.write(str(len(node_labels)) + "\n")
                for i in range(0,len(node_labels)):
                    foquery.write(node_labels[i]+"\n")
                if len(edge_labels) > 0:
                    foquery.write(str(len(edge_labels))+"\n")
                    for i in range(0,len(edge_labels)):
                        foquery.write(str(edges[i][0]) + " " + str(edges[i][1]) + " " + edge_labels[i] + "\n")
                    
                
                tid = []
                if data[file_index][0] == 'x':
                    tid = [item for item in data[file_index].split()[1:]]
                    fresult.write("\t".join(tid) + "\n")
            file_index +=1 ### this will skill empty line and reach to new line containing new t

        ct_graph+= 1
        print ct_graph 
        if ct_graph > 1000000:
            break
            # if ct_graph%10000 == 0:
            #     print("completed, ",ct_graph)
            #     print("it took", time.time() - start, "seconds.")
        
        file_index += 1
    return 



if __name__ == "__main__":
    print "creating dict"
    label_dict = get_label_dict(graph_db)
    label_dict = {value:key for key,value in label_dict.iteritems()}
    print label_dict
    print "creating queries"
    create_query_and_result_file(gspan_out,query_file,result_file,label_dict)