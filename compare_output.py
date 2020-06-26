import sys
file1 = sys.argv[1]
file2 = sys.argv[2]

f1 = open(file1,"r")
f2 = open(file2,"r")
d1 = f1.read().splitlines()
d2 = f2.read().splitlines()
print len(d1), len(d2)
if len(d1) != len(d2):
    print "not same lenght"
for i in range(len(d1)):
    t1 = d1[i].split()
    if "#" in t1[0]:
        t1 = [item.split("#")[-1] for item in t1]
    t2 = d2[i].split()
    if set(t1) != set(t2):
        print "diference"
        break
print "same "

    