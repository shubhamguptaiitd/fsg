import sys
file1 = sys.argv[1]
file2 = sys.argv[2]
import json
def get_data_in_fsg_format(infile,outfile):
    fin = open(infile,"r")
    fout = open(outfile,"w")
    EOF = False
    item = fin.readline()
    while not EOF:
        if item[0] == '#':
            fout.write("t " + item.strip() + "\n")
            num_nodes = int(fin.readline().split()[0])
            for i in range(num_nodes):
                fout.write("v" + " " + str(i) + " "+  fin.readline().split()[0] + "\n")
            num_edges = int(fin.readline().split()[0])
            for i in range(num_edges):
                fout.write("u" + " " + " ".join(fin.readline().split()) + "\n")
        item = fin.readline()
        if not item:
            fin.close()
            fout.close()
            return

def get_data_in_gspan_gaston_format(infile,outfile,label_dict_file):
    fin = open(infile,"r")
    fout = open(outfile,"w")
    EOF = False
    item = fin.readline()
    label_dict = {}
    num_graphs = 0
    ct = 0
    while not EOF:
        if item[0] == '#':
            num_graphs += 1
            fout.write("t"+ item.strip() + "\n")
            num_nodes = int(fin.readline().split()[0])
            for i in range(num_nodes):
                label = fin.readline().split()[0]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("v" + " " + str(i) + " "+ str(label)  + "\n")
            num_edges = int(fin.readline().split()[0])
            for i in range(num_edges):
                line = fin.readline().split()
                start_node = line[0]
                end_node = line[1]
                label = line[2]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("e" + " " + " ".join([start_node,end_node,str(label)]) + "\n")
        item = fin.readline()
        if not item:
            json.dump(label_dict,open(label_dict_file,"w"))
            fin.close()
            fout.close()
            return num_graphs

            


if __name__=="__main__":
    #get_data_in_fsg_format(file1,file2)
    get_data_in_gspan_gaston_format(file1,file2,"label_dict.json")
