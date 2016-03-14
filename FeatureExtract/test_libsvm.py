from svmutil import *

def test_svm(data_file_name,position,end_position=None):
    y,x=svm_read_problem(data_file_name)
    m=svm_train(y[:position],x[:position])
    p_label,p_acc,p_val=svm_predict(y[position:end_position],x[position:end_position],m)

if __name__ == "__main__":
    data_file_name='/home/huaa/workspace/FeatureExtract/letter/letter-recognition-svm.data'
    postion=15000
    test_svm(data_file_name,16000)