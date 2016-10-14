import matplotlib.pyplot as plt

f = open("cpuUsageData",'r')

L = f.readlines()

for i in range(len(L)):
    L[i] = float(L[i].strip())

Lx = []

for i in range(len(L)):
    Lx.append(i*10)

plt.plot(Lx, L)
plt.show()
