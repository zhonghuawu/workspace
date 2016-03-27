from __future__ import division

from svmutil import *
import sklearn 
from sklearn import svm
import numpy as np

from format_change2svm import letter2digit

def test_svm(data_file_name,position,end_position=None):
    y,x=svm_read_problem(data_file_name)
    m=svm_train(y[:position],x[:position])
    p_label,p_acc,p_val=svm_predict(y[position:end_position],x[position:end_position],m)

def test_svm_sklearn(data_file_name,position,label_position=-1,end_position=None):
    handler=open(data_file_name,'r')
    x=[]
    y=[]
    trans=letter2digit()
    for line in handler:
        datas=line.strip().split(',')
        label=datas[label_position]
        label=trans[label]
        y.append(label)
        del datas[label_position]
        features=map(eval,datas[3:])
        x.append(features)
    clf=svm.SVC()
    clf.fit(x[:position],y[:position])
    result=clf.predict(x[position:])
    result=np.array(result)-np.array(y[position:])
    total=result.size
    correctness=total-np.count_nonzero(result)
    accuracy=correctness/total
    return accuracy,correctness,total
    

if __name__ == "__main__":
    data_file_name_waveform='/home/huaa/workspace/datas/Waveform/waveform-+noise.data'
    data_file_name_letter='/home/huaa/workspace/datas/letter/letter-recognition.data'
    position=15000
    #test_svm(data_file_name,postion)
    accuracy,correctness,total=test_svm_sklearn(data_file_name_letter,
                                                position,label_position=0)
    print "accuracy=%g%% (%d/%d)"%(accuracy*100,correctness,total)