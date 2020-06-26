import matplotlib.pyplot as plt
import subprocess
import sys
from timeit import timeit
from transform_input import get_data_in_fsg_format,get_data_in_gspan_gaston_format
import math
import os 
dir_path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
print("directory path", dir_path)
infile = sys.argv[1]
outfile = sys.argv[2]
support_thresholds = [5,20,25,50,90,95] #5,20,25,50,
#support_thresholds = [90,93,95,98,99] #5,20,25,50,
fsg_times = []
gspan_times = []
gaston_times = []

def run_system_call(code_type,support,filename):
    if code_type =="fsg":
        subprocess.run([dir_path+"/binaries/fsg","-s",str(support), filename])
    if code_type == "gspan":
        subprocess.run([dir_path+"/binaries/gSpan","-f",filename,"-s",str(support)])
    if code_type == "gaston":
        subprocess.run([dir_path+"/binaries/gaston",str(support),filename])


def measure_time_function(func, *args, **kwargs):
    def time_function():
        return func(*args, **kwargs)
    return time_function
### FSG time ###
print("capturing fsg")
get_data_in_fsg_format(infile,"temp_file_fsg")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call, "fsg",support,"temp_file_fsg")
    fsg_times.append(timeit(time_function, number=1))
### GSPAN time ###
print("capturing gspan")
num_graphs = get_data_in_gspan_gaston_format(infile,"temp_file_gspan","label_dict")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call, "gspan",(support*1.0/100),"temp_file_gspan")
    gspan_times.append(timeit(time_function, number=1))

### GASTON TIME ###
print("capturing gaston")

num_graphs = get_data_in_gspan_gaston_format(infile,"temp_file_gaston","label_dict")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call, "gaston",math.ceil(support*num_graphs*1.00/100),"temp_file_gaston")
    gaston_times.append(timeit(time_function, number=1))



plt.plot(support_thresholds,fsg_times,'-b',label = '-FSG')
plt.plot(support_thresholds,gspan_times,'-r',label = '-GSPAN')
plt.plot(support_thresholds,gaston_times,'-g',label = '-GASTON')
plt.xlabel('Support thresholds %')
plt.ylabel('Running time (seconds)')
plt.legend(loc='upper right')
plt.title('Comparision of thresholds vs running time for fsg,gspan,gaston')
plt.savefig(outfile)
