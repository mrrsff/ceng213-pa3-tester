import os
import subprocess
import filecmp

def get_objs():
    path = os.path.join(os.path.dirname(os.path.realpath(__file__)),"objects")
    files = os.listdir(path)
    objs = [i for i in files if i.endswith('.obj')]
    return objs

print("Checking for your code...")
needed_files = ["BinaryHeap.cpp","MeshGraph.cpp"]
for i in needed_files:
    if not os.path.exists(f".\\your_code\\{i}"):
        print(f"Missing {i}")
        exit()

print("Compiling...")
os.system("g++ -o test.exe main.cpp")
objs = get_objs()

print("Cleaning up...")
while(os.listdir(".\\output")):
    os.remove(os.path.join(".\\output",os.listdir(".\\output")[0]))

print("Testing begins...")    
passed = 0
for i in range(len(objs)):
    if(os.path.exists(f".\\output\\object{i}.txt")):
        os.remove(f".\\output\\object{i}.txt")
    subprocess.run(f"test.exe >> .\\output\\object{i}.txt",shell=True, input=bytes(str(i),encoding='utf-8'))
    
    truth = filecmp.cmp(f".\\answers\\answer{i}.txt",f".\\output\\object{i}.txt")
    if truth:
        print(f"Object {i+1}/{len(objs)} passed")
        os.remove(f".\\output\\object{i}.txt")
        passed += 1
    else:
        print(f"Object {i+1}/{len(objs)} failed")

print(f"{passed}/{len(objs)} passed")
print("Proposed score: ",passed/len(objs)*100)
os.system("rm test.exe")
if passed!=len(objs):
    print("You can find the wrong output files in the output folder")