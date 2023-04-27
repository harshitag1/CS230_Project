import matplotlib.pyplot as plt
import numpy as np
import re

filenames = ['lfru_opt1.txt']

for filename in filenames:
    with open(filename, 'r') as file:
        data = file.read()

    data = data.split('\n')
    ipc = []
    label = []

    for i in range(0,len(data),4):
        line = data[i].split(',')
        label.append(line[0].strip())
        ipc.append(float(line[1].strip()))
    
    ipc = np.asarray(ipc)
    plt.bar(label, ipc)
    plt.xlabel('Threshold')
    plt.ylabel('IPC')
    plt.ylim(ipc.min() - 0.0001, ipc.max() + 0.0001)
    title = filename.split('_')[0].upper()
    title = title + ' Threshold'
    plt.title(title)
    plt.savefig(filename.split('.')[0])

    file.close()