from __future__ import division

import pandas as pd
from pandas import Series,DataFrame
import numpy as np

#FCT table
class FCT_table:
    def __init__(self,id1,id2,count_positive=None,count_negative=None,\
                 if_trend=None,r_square=0,if_asso=None,rss=None,tss=None,\
                 mean=None,r_square_max=0,polyfit=None):
        self.id1=id1
        self.id2=id2
        self.count_positive=count_positive
        self.count_negative=count_negative
        self.if_trend=if_trend
        self.r_square=r_square
        self.if_asso=if_asso
        self.rss=rss
        self.tss=tss
        self.mean=mean
        self.r_square_max=r_square_max
        self.polyfit=polyfit
    def update_trend(self,count_positive,count_negative,if_trend):
        self.count_positive=count_positive
        self.count_negative=count_negative
        self.if_trend=if_trend
    def update_curvefit(self,r_square,if_asso,rss,tss,mean,polyfit):
        self.r_square=r_square
        self.if_asso=if_asso
        self.rss=rss
        self.tss=tss
        self.mean=mean
        self.polyfit=polyfit

class FSCFFR:    
    def __init__(self,data_file_name,threshold_trend,threshold_fit,feature_size,\
                 data_size,window_size=100,basic_window_size=1):
        self.data_file_name=data_file_name#文件名
        self.threshold_trend=threshold_trend#趋势相关阈值
        self.threshold_fit=threshold_fit#拟合优度阈值
        self.feature_size=feature_size#特征个数
        self.data_size=data_size
        self.windows_size=window_size#滑动窗口大小等于基本窗口个数乘以基本窗口大小
        self.basic_windows_size=basic_window_size#基本窗口大小
        
        self.windows=None#滑动窗口内容
        self.diff_windows=None#差分窗口值
        
        self.fct_tables=[]#fct表集合
        self.create_uninitial_fct()
        
        self.chunker=None#逐块读取文件
        
    def create_uninitial_fct(self):
        for i in range(1,self.feature_size):
            for j in range(i+1,self.feature_size+1):
                self.fct_tables.append(FCT_table(i,j))
    
    def print_fct(self):
        for fct in self.fct_tables:
            if fct.if_asso==1:
                print fct.id1,fct.id2,":",fct.count_positive,fct.count_negative,fct.if_trend,fct.r_square,fct.if_asso#,fct.rss,fct.tss        
        
    #读文件
    def readfile(self):
        self.chunker=pd.read_csv(self.data_file_name,\
                                 chunksize=self.data_size,\
                                 names=range(self.feature_size+1))
        self.windows=self.chunker.get_chunk(size=self.windows_size)
        self.windows=self.windows.drop([0],axis=1)
        self.diff_windows=self.windows.diff(periods=1).dropna()
        
    #更新滑动窗口内容
    def update_windows(self):
        try:
            tmp_frame=self.chunker.get_chunk(size=self.basic_windows_size)
        except StopIteration:
            return False
        tmp_frame=tmp_frame.drop([0],axis=1)#删除标签项        
        index=self.windows.index[:self.basic_windows_size]
        self.windows=self.windows.drop(index)#删除前一个基本窗口中的内容
        self.windows=self.windows.append(tmp_frame,ignore_index=True)
        self.diff_windows=self.windows.diff(periods=1).dropna()
        return True
    
    #判断属性i，j是否趋势相关
    def trend(self,i,j):
        series=self.diff_windows[i]*self.diff_windows[j]
        count_positive=series[series>0].size/self.windows_size
        count_negative=series[series<0].size/self.windows_size
        if_trend=0
        if count_positive>self.threshold_trend or count_negative>self.threshold_trend:
            if_trend=1
        for fct in self.fct_tables:
            if fct.id1==i and fct.id2==j:
                fct.update_trend(count_positive,count_negative,if_trend)
                break
        return if_trend
    
    #曲线拟合，判断趋势相关的属性i和j是否属性相关
    def curvefit(self,i,j):
        x=self.windows[i].values
        y=self.windows[j].values
        a,b,c=np.polyfit(x,y,deg=2)
        y_fit=a*x**2+b*x+c
        y_bar=np.mean(y)
        rss=sum(np.square(y-y_fit))
        tss=sum(np.square(y-y_bar))
        r_square=1-rss/tss
        if_asso=0
        if r_square>self.threshold_fit:
            if_asso=1
        for fct in self.fct_tables:
            if fct.id1==i and fct.id2==j:
                fct.update_curvefit(r_square,if_asso,rss,tss,y_bar,lambda x:a*x**2+b*x+c)
                break

    def run(self):
        for i in range(1,self.feature_size):
            for j in range(i+1,self.feature_size+1):
                if_trend = self.trend(i,j)
                if if_trend == 1:
                    self.curvefit(i, j)
    
    
if __name__=="__main__":
    data_file_name='/home/huaa/workspace/datas/letter/letter-recognition.data'
    f=FSCFFR(data_file_name,0.6,0.6,16,20000,\
             window_size=3600,basic_window_size=200)
    f.readfile()
    f.run()
    f.print_fct()
    iter_nums=0
    while f.update_windows():
        f.run()
        iter_nums+=1
        print iter_nums,":"
        f.print_fct()
    
        
    #for fct in f.fct_tables:
        #if fct.if_trend==1:
            #print fct.id1,fct.id2,fct.count_positive,fct.count_negative,fct.if_trend,fct.r_square,fct.if_asso,fct.rss,fct.tss