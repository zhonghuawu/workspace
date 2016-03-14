import numpy as np
import matplotlib.pyplot as plt

def readData(filename):
    handler=open(filename,'r')
    datas=[]
    labels=[]
    for line in handler:
        data=line.strip().split(",")[1:]
        for i in range(len(data)):
            data[i]=float(data[i]) if data[i]!='?' else 0
        #data=map(lambda x:int(x) if x!='?' else x=0,data)
        labels.append(data[-1])
        data=np.array(data[:-1])
        datas.append(data)
    return datas,labels

def relief(datas,labels):
    normalIndex=[]
    unNormalIndex=[]
    lens=len(datas)
    for i in range(lens):
        if labels[i]!=2:
            normalIndex.append(i)
        else:
            unNormalIndex.append(i)
    M=lens/3
    W=np.zeros(len(datas[0]))
    for m in range(M):
        R=np.random.randint(lens)
        if R in normalIndex:
            normalIndex.remove(R)
        elif R in unNormalIndex:
            unNormalIndex.remove(R)
        sample=datas[R]
        distIndex1=normalIndex[0]
        distance1=np.linalg.norm(sample-datas[distIndex1])
        for i in normalIndex[1:]:
            dist=np.linalg.norm(sample-datas[i])
            if dist<distance1:
                distance1=dist
                distIndex1=i
        distIndex2=unNormalIndex[0]
        distance2=np.linalg.norm(sample-datas[distIndex2])
        for i in unNormalIndex[1:]:
            dist=np.linalg.norm(sample-datas[i])
            if dist<distance2:
                distance2=dist
                distIndex2=i
        
        W=W+datas[distIndex1]/M-datas[distIndex2]/M
    return W

if __name__ == '__main__':
    fn='breast-cancer-wisconsin.data'
    datas,labels=readData(fn)
    plt.figure()
    for i in range(20):
        W=relief(datas, labels)
        plt.plot(W)
    plt.show()
    print W