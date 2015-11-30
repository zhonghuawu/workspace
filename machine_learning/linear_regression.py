from __future__ import division

import os.path
from matplotlib import pyplot as plt
import numpy as np


def read_file(filename):
    handler = open(filename,'r')
    years = []
    prices = []
    for line in handler:
        year,price = line.strip().split(' ')
        years.append(int(year))
        prices.append(float(price))
    handler.close()
    datas = zip(years,prices)
    return datas

def write_file(filename,datas):
    handler = open(filename,'w')
    for year,price in datas:
        handler.write("%s %s\n"%(year,price))
    handler.close()

def plotLine(datas):
    years = []
    prices = []
    for year,price in datas:
        years.append(float(year))
        prices.append(float(price))
    b1,b0 = calcParam_LMS(years,prices)
    x1,x2 = 2000,2013
    y1,y2 = b0+b1*x1,b0+b1*x2
 
    plt.subplot(121)
    plt.plot(years,prices,'b*')
    plt.plot([x1,x2],[y1,y2])
    x1,x2=2014,2015
    y1,y2=b0+b1*x1,b0+b1*x2
    plt.plot([x1,x2],[y1,y2],'r*')
    plt.xlabel('years')
    plt.xlim(1999,2016)
    plt.ylabel('prices')
    plt.ylim(0,15)
    plt.title('Least Mean Square')
    print "Least Mean Square algorithm predict result:"
    print "The house price of %d is estimated %f"%(x1,y1)
    print "The house price of %d is estimated %f"%(x2,y2)
   
    
    plt.subplot(122)
    b0,b1 = calcParam_GD(years,prices) 
    x1,x2 = 0,13
    y1,y2 = b0+b1*x1,b0+b1*x2
    plt.plot(years,prices,'b*')
    plt.plot([x1,x2],[y1,y2])
    x1,x2=14,15
    y1,y2=b0+b1*x1,b0+b1*x2
    plt.plot([x1,x2],[y1,y2],'r*')
    plt.xlabel('years')
    plt.xlim(1999,2016)
    plt.ylabel('prices')
    plt.ylim(0,15)
    plt.title('Gradient Descent') 
    print "Gradient Decent Algorithm predict result:"
    print "The house price of %d is estimated %f"%(x1,y1)
    print "The house price of %d is estimated %f"%(x2,y2)

    
    #plt.legend()
    plt.show()

def calcParam_LMS(years,prices):
    xmean = sum(years)/len(years)
    ymean = sum(prices)/len(prices)
    xdiff = map(lambda x:x-xmean,years)
    ydiff = map(lambda y:y-ymean,prices)
    ixx=sum(map(lambda x:x*x,xdiff))
    ixy=sum(map(lambda x,y:x*y,xdiff,ydiff))
    #print "ixx = %f\nixy = %f\n"%(ixx,ixy)
    b1 = ixy/ixx
    b0=ymean-b1*xmean
    print [b1,b0]
    return b1,b0

def calcParam_GD(years,prices):
    theta = np.ones(shape=(2,1))
    alpha = 0.0001
    lens=len(years)
    x=np.zeros(shape = (lens,2))
    for i in range(lens):
        x[i][0] = 1
        x[i][1] = years[i]-2000
    xTrans = x.transpose()
    y = np.matrix(prices)
    iterNums = 1000
    i = 0
    while(i<iterNums):
        hypothesis = np.dot(x,theta)
        loss = hypothesis-y
        gradient=np.dot(xTrans,loss.transpose())
        theta = theta-alpha*gradient
        i=i+1
    print theta
    return theta

if __name__ == "__main__":
    rf = 'linear_regression_data.txt'
    datas = read_file(rf)
    print datas
    plotLine(datas)
