import os
import subprocess
import filecmp

def get_objs():
    path = os.path.join(os.path.dirname(os.path.realpath(__file__)),"objects")
    files = os.listdir(path)
    objs = [i for i in files if i.endswith('.obj')]
    return objs


os.system("g++ -o answers.exe main.cpp")
objs = get_objs()

while(os.listdir(".\\answers")):
    os.remove(os.path.join(".\\answers",os.listdir(".\\answers")[0]))
    
passed = 0
for i in range(len(objs)):
    if(os.path.exists(f".\\answers\\answer{i}.txt")):
        os.remove(f".\\answers\\answer{i}.txt")
    subprocess.run(f"answers.exe >> .\\answers\\answer{i}.txt",shell=True, input=bytes(str(i),encoding='utf-8'))

os.system("del answers.exe")