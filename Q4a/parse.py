import os, time

f1 = open('contextSwitchesNum', 'a')

f2 = open('cpuUsageData', 'a')

for i in range (1440):

    os.system('perf stat -o tempdtella -p 9413 sleep 5') 

    time.sleep(10)
    
    f = open('tempdtella','r')
    
    lines = f.readlines()    
    
    f1.write(lines[6].split(' ')[15] + '\n')    
    
    f2.write(lines[5].split('#')[1].split(' ')[4] + '\n')
