import matplotlib.pyplot as plt
import numpy as np
import re

# ipc = [[0.33442, 0.306627, 0.308849],[0.380354, 0.129015, 0.379489], [0.0513314, 0.0328543, 0.0544968]]
# ipc = [[3955452/4838281, 600530/935914, 88508/383437],[3956282/4839791, 605063/936444, 60007/382923]]
# ipc_6 = [[0.180842, 0.180345, 0.207648], [0.175115, 0.12894, 0.193472], [0.179368, 0.159638, 0.204755]]
ipc_6 = [[[5986128/7362079, 5727602/7360369, 6167350/7363965], [5985920/7362417, 5739034/7360469, 6165609/7364287], [5985999/7362498, 5739099/7360580, 6165480/7364152]], 
         [[748267/1475955, 174923/1739629, 660310/1296292], [1376241/2752732, 1603838/3242445, 1198148/2397179], [762699/1475952, 167360/1721078, 670919/1298138]], 
         [[87778/816241, 1089730/1662597, 118281/728498], [489043/1918520, 1344809/3105472, 450203/1662167], [89630/812920, 952876/1647342, 119709/727387]]]
# ipc_8 = [0.180345, 0.12894, 0.159638]
# ipc_8 = [[5727602/7360369, 5739034/7360469, 5739099/7360580],[174923/1739629, 1603838/3242445, 167360/1721078],[1089730/1662597, 1344809/3105472, 952876/1647342]]
# ipc = np.asarray(ipc)
caches = ['L1D', 'L2C', 'LLC']
inc_policies = ['Inclusive', 'Exclusive', 'Non-Inclusive']
# inc_policies = ['half the default sizes on champsim','Blowing up LLC size','default sizes on champsim']
# opt = ['Vanilla', 'Optimized']
# opt = ['BFS-3', 'CC-14', 'PR-14']
opt = ['half the default sizes on champsim','Blowing up LLC size','default sizes on champsim']
colors = ['#283350', '#F93800', '#FFB500']

ipc_1 = np.asarray(ipc_6)

for i in range(3):
    fig, ax = plt.subplots()
    ipc = ipc_1[i]
    ax.set_xticks([i+0.25 for i in range(len(ipc[0]))])
    ax.set_xticklabels(inc_policies)
    # ax.set_xlabel('Replacement Policy')
    ax.set_ylabel('IPC')
    # ax.set_title('Optimizations on LFU')
    ax.set_title(f'LFU IN DIFFERENT CACHE SIZES ({caches[i]}-HITRATE)')
    ax.set_ylim([ipc.min() - 0.1,ipc.max() + 0.18])
    for j in range(len(ipc)):
        color = colors[j%len(colors)]
        ax.bar([k + j*0.25 for k in range(len(ipc[j]))], ipc[j], width=0.25, color=color, label=opt[j])
    legend = ax.legend(loc='best')
    # plt.show()
    plt.savefig(f'cache_sizes_{caches[i]}')

# plt.bar(range(len(ipc)), ipc, width=0.5)
# plt.ylabel('IPC')
# plt.ylim(ipc.min() - 0.05, ipc.max() + 0.05)
# plt.title('LFU ON DIFFERENT CACHE SIZES (IPC)')
# plt.savefig('cache_sizes_8')