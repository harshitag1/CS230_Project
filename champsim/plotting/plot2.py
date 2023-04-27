import matplotlib.pyplot as plt
import numpy as np
import re

filenames = ['lfru_opt2.txt']

for filename in filenames:
    with open(filename, 'r') as file:
        data = file.read()

    data = data.split('\n')
    ipc = []
    label = []

    for i in range(0,len(data),4):
        line = data[i].split(',')
        if i == 0:
            label.append('No Decay')
        else:
            # label.append(r'$1/' + line[0].strip() + r'$')
            label.append(r'$\frac{1}{' + line[0].strip() + r'}$')
        ipc.append(float(line[1].strip()))
    
    ipc = np.asarray(ipc)
    plt.bar(label, ipc, width=0.5)
    plt.xlabel('Decay')
    plt.ylabel('IPC')
    plt.ylim(ipc.min() - 0.00001, ipc.max() + 0.00001)
    title = filename.split('_')[0].upper()
    title = title + ' Decay'
    plt.title(title)
    plt.savefig(filename.split('.')[0])

    file.close()