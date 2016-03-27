
def letter2digit():
    letters='ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    trans={'':0}
    i=1
    for letter in letters:
        trans[letter]=i
        i+=1
    return trans

def change_format2svm(filename_r,filename_w):
    handler_r=open(filename_r,'r')
    handler_w=open(filename_w,'w')
    labels=set([])
    #trans=letter2digit()
    for line in handler_r:
        datas=line.strip().split(',')
        #label=trans[datas[0]]
        label=datas[-1]
        labels.add(label)
        handler_w.write('%s '%label)
        index=1
        for data in datas[:-1]:
            handler_w.write('%d:%s '%(index,data))
            index+=1
        handler_w.write('\n')
    handler_r.close()
    handler_w.close()
    return labels
def count_label(filename):
    handler=open(filename,'r')
    labels=set([])
    for line in handler:
        label=line.strip().split(' ')[0]
        labels.add(label)
    labels=list(labels)
    labels.sort()
    return labels
    
if __name__ == "__main__":
    path_r='/home/huaa/workspace/datas/Waveform/waveform-+noise.data'
    path_w='/home/huaa/workspace/datas/Waveform/waveform-+noise-svm.data'
    change_format2svm(path_r,path_w)
    #labels=count_label(path_w)
    #print labels