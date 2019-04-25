#!/usr/bin/python

import os, sys, time, json

files = []

name = ""

for x in os.walk(os.getcwd()):
    for y in x[2]:
        if name != "" and name + ".c" in y:
            files.append({ "file": str(x[0])  + "/"+ y, "folder": str(x[0]), "name": str(x[0].split("/")[-1])})
            break
        elif name == "" and ".c" in y and not "utilities" in x[0] and not "output" in x[0] and not ".csv" in y:
            files.append({ "file": str(x[0])  + "/"+ y, "folder": str(x[0]), "name": str(x[0].split("/")[-1])})

links = "utilities/polybench.c"
flags = "-O3 -lm -I utilities -I"

# for x in files:
#     print "runtime of " + x['name']
#     runtime = 0.0
#     tmpFlag = flags + x["folder"]
#     execute = "clang " + tmpFlag + " " + links + " " + x["file"] + " -o output.out"
#     start = time.time()
#     repeats = 1
#     for i in range(0, repeats):
#         os.system(execute)
#         os.system("./output.out  >> /dev/null")
#     end = time.time()
#     runtime = float(end - start) / repeats
#     x["base-time"] = runtime
#
# with open("backup.json", "w") as file:
#     file.write(json.dumps(files))
#
# print "#######################################################################################"
# print "#######################################################################################"
# print "#######################################################################################"
# print "BASE TIME DONE"
# print "#######################################################################################"
# print "#######################################################################################"
# print "#######################################################################################"


with open("backup.json", "r") as file:
    files = json.load(file)


if not os.path.exists("output"):
    os.makedirs("output")
for x in files:
    # if x["name"] != name:
    #     continue
    print x['name'] + " full simulation "
    if not os.path.exists("output/" + x["name"]):
        os.makedirs("output/" + x["name"] )

    tmpFlag = flags + x["folder"]
    tmpLinks = links

    execute = "../cSim -link-files=\"" + tmpLinks + "\" -flag=\"" + tmpFlag + "\" " + x["file"] + " > output/" + x["name"] +"/result-full"
    print execute
    start = time.time()
    os.system(execute)
    end = time.time()
    x["time"] = float(end-start)

with open("backup.json", "w") as file:
    file.write(json.dumps(files))
print "#######################################################################################"
print "#######################################################################################"
print "#######################################################################################"
print "BASE SIMULATION DONE"
print "#######################################################################################"
print "#######################################################################################"
print "#######################################################################################"

if os.path.isfile("output/time"):
    os.remove("output/time")

outfileTime = open("output/time", "a")
for x in files:
    if "base-time" in x:
        outfileTime.write(x["name"] + "," + str(x["base-time"]))

        if "time" in x:
            outfileTime.write(", " + str(x["time"]))
        outfileTime.write("\n");
outfileTime.close()
