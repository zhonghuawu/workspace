from __future__ import division

import pandas as pd
from pandas import Series,DataFrame
import numpy as np

#FCT table
class fct_table:
    def __init__(self,id1,id2,count_positive=None,count_negative=None,\
                 if_trend=None,r_square=0,if_asso=None,rss=None,tss=None,\
                 mean=None,r_square_max=0,polyfit=None):
        self.id1=id1
        self.id2=id2
        self.count_positive=count_positive
        self.count_nagetive=count_negative
        self.if_trend=if_trend
        self.r_square=r_square
        self.if_asso=if_asso
        self.rss=rss
        self.tss=tss
        self.mean=mean
        self.r_square_max=r_square_max
        self.polyfit=polyfit
    def update_trend(self,count_positive,count_nagetive,if_trend):
        self.count_positive=count_positive
        self.connt_nagetive=count_nagetive
        self.if_trend=if_trend
    def update_curvefit(self,r_square,if_asso,rss,tss,mean,polyfit):
        self.r_square=r_square
        self.if_asso=if_asso
        self.rss=rss
        self.tss=tss
        self.mean=mean
        self.polyfit=polyfit

class fscffr:
    
    def __init__(self,data_file_name,threshold_trend,threshold_fit,feature_size,\
                 window_size=100,basic_window_size=1):
        self.data_file_name=data_file_name
        self.threshold_trend=threshold_trend#趋势相关阈值
        self.threshold_fit=threshold_fit#拟合优度阈值
        self.feature_size=feature_size#特征个数
        self.windows_size=window_size#滑动窗口大小等于基本窗口个数乘以基本窗口大小
        self.basic_windows_size=basic_window_size#基本窗口大小
        
        self.windows=None#滑动窗口内容
        self.diff_windows=None#差分窗口值
        
        self.readed_rows=[]#已经读入的行
        self.fct_tables=[]#fct表集合
        
    #读文件
    def readfile(self):
        self.windows=pd.read_csv(self.data_file_name,\
                                 nrows=self.windows_size,\
                                 names=range(self.feature_size+1))
        self.windows=self.windows.drop(0,axis=1)
        self.readed_rows.extend(range(self.windows_size))#记录读入的行
        
        self.diff_windows=self.windows.diff(periods=1)
        self.diff_windows=self.diff_windows.dropna()
        
    def __read_basic_windows(self):
        tmp_frame=pd.read_csv(self.data_file_name,nrows=self.basic_windows_size,\
                              names=range(self.feature_size+1),\
                              skiprows=self.readed_rows)
        tmp_frame=tmp_frame.drop(0,axis=1)
        start=self.readed_rows[-1]+1
        end=self.readed_rows[-1]+self.basic_windows_size+1
        self.readed_rows.extend(range(start,end))
        return tmp_frame
        
    #更新滑动窗口内容
    def update_windows(self):
        tmp_frame=self.__read_basic_windows()
        index=self.windows.index[:self.basic_windows_size]
        self.windows=self.windows.drop(index)#删除前一个基本窗口中的内容
        self.windows=self.windows.add(tmp_frame,fill_value=0)
        self.diff_windows=self.windows.diff(periods=1).dropna()
    
    #判断属性i，j是否趋势相关
    def trend(self,i,j):
        series=self.diff_windows[i]*self.diff_windows[j]
        count_positive=series[series>0].size/self.windows_size
        count_nagetive=series[series<0].size/self.windows_size
        if_trend=0
        if count_positive>self.threshold_trend or count_nagetive>self.threshold_trend:
            if_trend=1
        for fct in self.fct_tables:
            if fct.id1==i and fct.id2==j:
                fct.update_trend(count_positive,count_nagetive,if_trend)
        else:
            fct=fct_table(i,j,count_positive, count_nagetive, if_trend)
            self.fct_tables.append(fct)
        return if_trend
    
    #曲线拟合，判断趋势相关的属性i和j是否属性相关
    def curvefit(self,i,j):
        x=self.windows[i].values
        y=self.windows[j].values
        k,b=np.polyfit(x,y,1)
        _y=k*x+b
        mean=np.mean(y)
        rss=sum(np.square(y-_y))
        tss=sum(np.square(y-mean))
        r_square=1-rss/tss
        if_asso=0
        if r_square>self.threshold_fit:
            if_asso=1
        for fct in self.fct_tables:
            if fct.id1==i and fct.id2==j:
                fct.update_curvefit(r_square,if_asso,rss,tss,mean,lambda x:k*x+b)
       
        
    
    def run(self):
        for i in range(1,self.feature_size):
            for j in range(i+1,self.feature_size+1):
                if_trend = self.trend(i,j)
                if if_trend == 1:
                    self.curvefit(i, j)
    
    
if __name__=="__main__":
    data_file_name='/home/huaa/workspace/datas/letter/letter-recognition.data'
    f=fscffr(data_file_name,0.6,0.6,16,window_size=200,basic_window_size=40)
    f.readfile()
    f.run()
    i=0
    while 20000 not in f.readed_rows:
        f.update_windows()
        f.run()
    for fct in f.fct_tables:
        if fct.if_trend==1:
            print fct.id1,fct.id2,fct.count_positive,fct.count_nagetive,fct.if_trend,fct.r_square,fct.if_asso,fct.rss,fct.tss