from __future__ import division

import pandas as pd
from pandas import DataFrame,Series
import numpy as np
from scipy import stats
from sklearn import linear_model
from itertools import chain,combinations

#return a subset iterator of iterable object
def powerset(iterale):
    xs=list(iterale)
    return chain.from_iterable(combinations(xs,n) for n in range(len(xs)+1))

class OSFS:
    def __init__(self,filename):
        df=pd.read_csv(filename)
        labels=df['class']
        self.labels=Series((labels=='ALL')+1,dtype=int)
        self.features=df[['f'+str(i) for i in range(1,7130)]]
        self.__col = 0#store current feature vector index
        self.BCF = set([])
        
    def get_labels(self):
        return self.labels
    
    def get_feature(self):
        tmp = self.features.icol(self.__col)
        self.__col+=1
        return tmp
    def run(self):
        i=0
        while(True):
            added = 0
            X=self.get_feature()
            if self.__col == 10:
                return
            C=self.get_labels()
            if Dep_ZTest(C, X, DataFrame([]), 0):
                added = 1
                self.BCF.add('f'+str(self.__col))
            if added!=0:
                if len(self.BCF) == 1:
                    continue
                l=len(self.BCF)
                for _ in range(l):
                    flag = 0
                    Y_index = self.BCF.pop()
                    BCF_Y = self.BCF-set([Y_index])
                    for S_index in powerset(BCF_Y):
                        if len(S_index)==0:
                            continue
                        else:
                            S=self.features[list(S_index)]
                        Y=self.features[Y_index]
                        if Ind_ZTest(C, Y, S, len(S)):
                            flag=1
                            break
                    if flag == 0:
                        self.BCF.add(Y_index)
            print i,":",self.BCF
            i+=1
            #print self.BCF
#calculate partial correlation ceofficient of X and C
#given set S ,N is length of sample
def partial_correlation(C,X,S,N):
    S = DataFrame(S)
    C = Series(C)
    X = Series(X)
    #linear fit between S and C
    linreg = linear_model.LinearRegression()
    linreg.fit(S,C)
    coef_c=linreg.coef_
    intercept_c = linreg.intercept_
    R_c=C-(S.mul(coef_c,axis=1).sum()+intercept_c)#residuals of C and S
    
    #linear fit between S and X
    linreg.fit(S,X)
    coef_x = linreg.coef_
    intercept_x = linreg.intercept_
    R_x=X-(S.mul(coef_x,axis=1).sum()+intercept_x)#residuals for X and S
    numerator = N*(R_c*R_x).sum() - R_c.sum()*R_x.sum()
    denominator=np.sqrt(N*np.square(R_c).sum()-np.square(R_c.sum()))*\
        np.sqrt(N*np.square(R_x).sum()-np.square(R_x.sum()))
    rou = numerator*1.0/denominator
    return rou

def fisher_z_test(C,X,S,M):
    if S.empty:
        return C.corr(X)
    rou = partial_correlation(C, X, S, M)
    print "rou=",rou
    z = 1.0/2.0*np.sqrt(M-len(S.columns)-3)*np.log((1+rou)/(1-rou))
    return z

def Dep_ZTest(C,X,S,M,alpha=0.05):
    z0=1.64
    if alpha == 0.01:
        z0=2.33
    z = fisher_z_test(C,X,S,M)
    print "z=",z
    if S.empty:
        if np.abs(z) >= alpha:
            return True
        else:
            return False
    if z >= z0:
        return True
    else :
        return False
def Ind_ZTest(C,X,S,M,alpha=0.05):
    return ~Dep_ZTest(C, X, S, M, alpha)

def Dep_GTest(C,X,S,M,alpha=0.05):
    C=np.array(C)
    X=np.array(X)
    g,p,dof,expected = stats.chi2_contingency(np.array([X,C]))#,lambda_='log-likelihood')
    if (p<=alpha):
        return true
    return false
def Ind_GTest(C,X,alpha=0.05):
    return ~Dep(C,X,alpha)
    
if __name__=="__main__":
    filename = '/home/huaa/workspace/datas/leukemia.csv'
    osfs = OSFS(filename)
    osfs.run()
    #print osfs.get_feature().values[:3]
    #print osfs.get_feature().values[:3]
    #print osfs.get_labes().values[:3]
    #X=osfs.get_feature().values
    #C=osfs.get_labels()
    #print C.value_counts()
    #print Dep(C,X)