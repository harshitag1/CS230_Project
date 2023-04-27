import matplotlib.pyplot as plt
import numpy as np
import re

with open('policy_comparison.txt','r') as file:
    data = file.read()

data = data.split('\n')

ipc = []    
k = 0

for i in range(0,len(data),20):
    # print(data[i])
    l = []
    if k==3:
        break
    t = 0
    for j in range(i,i+20,4):
        if 'ship' not in data[j]:
            numbers = re.findall(r'\d+', data[j])
            # print(data[j])
            l.append(float('0.' + numbers[-1]))
            t += 1
    ipc.append(l)
    k += 1

# ipc.append(ipc[0])
# ipc.append(ipc[0])
ipc = np.asarray(ipc)

# print()
# print(ipc)
# print()

inc_policies = ['Inclusive', 'Exclusive', 'Non-Inclusive']
rep_policies = ['lru', 'lfu', 'fifo', 'random']
colors = ['#283350', '#F93800', '#FFB500']

fig, ax = plt.subplots()
ax.set_xticks([i+0.25 for i in range(len(ipc[0]))])
ax.set_xticklabels(rep_policies)
ax.set_xlabel('Replacement Policy')
ax.set_ylabel('IPC')
ax.set_title('IPC vs Replacement Policy (BFS)')
ax.set_ylim([0,ipc.max() + 0.1])
for j in range(len(ipc)):
    color = colors[j%len(colors)]
    ax.bar([k + j*0.25 for k in range(len(ipc[j]))], ipc[j], width=0.25, color=color, label=inc_policies[j])
legend = ax.legend(loc='upper left', bbox_to_anchor=(0, 1), ncol=len(inc_policies))
# plt.show()
plt.savefig('repp')

k = 0
hit_rate = []
miss_rate = []
for i in range(len(data)):
    if k == 3:
        break
    if 'random' in data[i]:
        l1 = []
        l2 = []
        for j in range(i+1, i+4):
            numbers = re.findall(r'\d+', data[j])
            # print(j)
            try:
                if(j%4 == 3):
                    l1.append(float(int(numbers[1])/int(numbers[0])))
                    l2.append(float(int(numbers[2])/int(numbers[0])))
                else:
                    l1.append(float(int(numbers[2])/int(numbers[1])))
                    l2.append(float(int(numbers[3])/int(numbers[1])))
            except:
                continue
        k += 1
        # print(l1)
        # print(l2)
        if len(l1) != 0:
            hit_rate.append(l1)
        if len(l2) != 0:
            miss_rate.append(l2)

# hit_rate.append(hit_rate[0])
# hit_rate.append(hit_rate[0])
# miss_rate.append(miss_rate[0])
# miss_rate.append(miss_rate[0])
hit_rate = np.asarray(hit_rate)
miss_rate = np.asarray(miss_rate)
caches = ['L1C', 'L2C', 'LLC']
# print(hit_rate)
# print(miss_rate)
# print()


fig, ax = plt.subplots()
ax.set_xticks([i+0.25 for i in range(len(hit_rate[0]))])
ax.set_xticklabels(caches)
ax.set_xlabel('Caches')
ax.set_ylabel('Hit Rate')
ax.set_title('Hit Rate vs Caches (BFS-RANDOM)')
ax.set_ylim([0,hit_rate.max() + 0.2])
for j in range(len(hit_rate)):
    color = colors[j%len(colors)]
    ax.bar([k + j*0.25 for k in range(len(hit_rate[j]))], hit_rate[j], width=0.25, color=color, label=inc_policies[j])
legend = ax.legend(loc='upper left', bbox_to_anchor=(0, 1), ncol=len(inc_policies))
# plt.show()
plt.savefig('hit_rate_random')

fig, ax = plt.subplots()
ax.set_xticks([i+0.25 for i in range(len(miss_rate[0]))])
ax.set_xticklabels(caches)
ax.set_xlabel('Caches')
ax.set_ylabel('Miss Rate')
ax.set_title('Miss Rate vs Caches (BFS-RANDOM)')
ax.set_ylim([0,miss_rate.max() + 0.2])
for j in range(len(miss_rate)):
    color = colors[j%len(colors)]
    ax.bar([k + j*0.25 for k in range(len(miss_rate[j]))], miss_rate[j], width=0.25, color=color, label=inc_policies[j])
legend = ax.legend(loc='upper left', bbox_to_anchor=(0, 1), ncol=len(inc_policies))
# plt.show()
plt.savefig('miss_rate_random')

