import os
from pathlib import Path
import subprocess
import filecmp

def get_objs():
    files = Path("./objects").glob('**/*')
    objs = [i for i in files if i.suffix == '.obj']
    return objs

print("Checking for your code...")
needed_files = ["BinaryHeap.cpp","MeshGraph.cpp"]
for i in needed_files:
    if not Path(f"./your_code/{i}").exists():
        print(f"Missing {i}")
        exit()

output_name = "test.exe"
print("Compiling...")
os.system(f"g++ -o {output_name} main.cpp")
objs = get_objs()

print("Cleaning up...")
while(os.listdir("./output")):
    for i in os.listdir("./output"):
        os.remove(Path(f"./output/{i}"))

print("Testing begins...")    
passed = 0
for i in range(len(objs)):
    if(Path(f"./output/object{i}.txt").exists()):
        os.remove(Path(f"./output/object{i}.txt"))
    subprocess.run(f"{output_name} >>" + str(Path(f"./output/object{i}.txt")) ,shell=True, input=bytes(str(i),encoding='utf-8'))
    truth = filecmp.cmp(Path(f"./answers/answer{i}.txt"),Path(f"./output/object{i}.txt"))
    if truth:
        print(f"Object {i+1}/{len(objs)} passed")
        os.remove(Path(f"./output/object{i}.txt"))
        passed += 1
    else:
        print(f"Object {i+1}/{len(objs)} failed")

print(f"{passed}/{len(objs)} passed")
print("Proposed score: ",passed/len(objs)*100)
os.system("rm test.exe")
if passed!=len(objs):
    print("You can find the wrong output files in the output folder")