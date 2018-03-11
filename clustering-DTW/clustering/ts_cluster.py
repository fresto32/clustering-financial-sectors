#Slightly adapted from: https://github.com/alexminnaar/time-series-classification-and-clustering

import numpy as np
import random
import matplotlib.pylab as plt

class ts_cluster(object):
    def __init__(self,num_clust):
        '''
        num_clust is the number of clusters for the k-means algorithm
        assignments holds the assignments of data points (indices) to clusters
        centroids holds the centroids of the clusters
        '''
        self.num_clust=num_clust
        self.assignments={}
        self.centroids=[]
        
    def k_means_clust(self,data,num_iter,w,progress=False):
        '''
        k-means clustering algorithm for time series data.  dynamic time warping Euclidean distance
         used as default similarity measure. 
        '''
        self.centroids=random.sample(data.tolist(),self.num_clust)
        
        for n in range(num_iter):
            if progress:
                print('iteration ' + str(n+1))
            #assign data points to clusters
            self.assignments={}
            #go through each list of data, starting at 0, determine which
            #piece of data is closest to which centroid. 
            for ind,i in enumerate(data):
                min_dist=float('inf')
                closest_clust=None
                for c_ind,j in enumerate(self.centroids):
                    if self.LB_Keogh(i,j,5)<min_dist:
                        cur_dist=self.DTWDistance(i,j,w)
                        if cur_dist<min_dist:
                            min_dist=cur_dist
                            closest_clust=c_ind
                if closest_clust in self.assignments:
                    self.assignments[closest_clust].append(ind)
                else:
                    self.assignments[closest_clust]=[ind]
        
            #recalculate centroids of clusters
            for key in self.assignments:
                # print("Key: " + str(key))
                # print("Ass: ")
                # print(self.assignments)
                clust_sum = 0
                for k in self.assignments[key]:
                    clust_sum=clust_sum+data[k]
                # print("Clust sum out of loop: ")
                # print(clust_sum)
                if len(self.assignments[key]) > 0:
                    self.centroids[key]=[m/len(self.assignments[key]) for m in clust_sum]

    def get_centroids(self):
        return self.centroids
        
    def get_assignments(self):
        return self.assignments
        
    def plot_centroids(self):
        for i in self.centroids:
            plt.plot(i)
        plt.show()
        
    def DTWDistance(self,s1,s2,w=None):
        '''
        Calculates dynamic time warping Euclidean distance between two
        sequences. Option to enforce locality constraint for window w.
        '''
        DTW={}
    
        if w:
            w = max(w, abs(len(s1)-len(s2)))
    
            for i in range(-1,len(s1)):
                for j in range(-1,len(s2)):
                    DTW[(i, j)] = float('inf')
            
        else:
            for i in range(len(s1)):
                DTW[(i, -1)] = float('inf')
            for i in range(len(s2)):
                DTW[(-1, i)] = float('inf')
        
        DTW[(-1, -1)] = 0
    
        for i in range(len(s1)):
            if w:
                for j in range(max(0, i-w), min(len(s2), i+w)):
                    dist= (s1[i]-s2[j])**2
                    DTW[(i, j)] = dist + min(DTW[(i-1, j)],DTW[(i, j-1)], DTW[(i-1, j-1)])
            else:
                for j in range(len(s2)):
                    dist= (s1[i]-s2[j])**2
                    DTW[(i, j)] = dist + min(DTW[(i-1, j)],DTW[(i, j-1)], DTW[(i-1, j-1)])
            
        return np.sqrt(DTW[len(s1)-1, len(s2)-1])
       
    def LB_Keogh(self,s1,s2,r):
        '''
        Calculates LB_Keough lower bound to dynamic time warping. Linear
        complexity compared to quadratic complexity of dtw.
        '''
        LB_sum=0
        for ind,i in enumerate(s1):
            
            lower_bound=min(s2[(ind-r if ind-r>=0 else 0):(ind+r)])
            upper_bound=max(s2[(ind-r if ind-r>=0 else 0):(ind+r)])
            
            if i>upper_bound:
                LB_sum=LB_sum+(i-upper_bound)**2
            elif i<lower_bound:
                LB_sum=LB_sum+(i-lower_bound)**2
        
        return np.sqrt(LB_sum)

data = np.genfromtxt('kmeans_format.csv', delimiter = "\t")
fout = open("to_json.json","w+")
to_file = ""
dict_ind_to_ticker = {'0': 'ABC', '1': 'AFI', '2': 'AMC', '3': 'AMP', '4': 'ANN', '5': 'ANZ', '6': 'ARG', '7': 'ASX', '8': 'BEN', '9': 'BHP', '10': 'BWP', '11': 'CAB', '12': 'CBA', '13': 'CCL', '14': 'COH', '15': 'CPU', '16': 'CTX', '17': 'DJW', '18': 'FLT', '19': 'FWD', '20': 'FXJ', '21': 'GUD', '22': 'HVN', '23': 'IAG', '24': 'IRE', '25': 'JHX', '26': 'MCR', '27': 'NCM', '28': 'ORI', '29': 'OSH', '30': 'PMC', '31': 'PPT', '32': 'QBE', '33': 'RHC', '34': 'RIC', '35': 'RIO', '36': 'RMD', '37': 'SGP', '38': 'SHL', '39': 'SOL', '40': 'STW', '41': 'TLS', '42': 'WBC', '43': 'WOW', '44': 'WPL'}

for i in range(2,21,2):
    to_file += "{\n\t\"dtw\": true,\n\t\"num_clusters\": " + str(i) + ",\n\t\"clusters\": "
    cluster_DTW = ts_cluster(i);
    # data, num iteration, w
    cluster_DTW.k_means_clust(data,10,50, False)
        
    #print(cluster_DTW.get_assignments())
    assignment_dict = cluster_DTW.get_assignments(); 
    to_file += "["
    k = 1
    for key in sorted(assignment_dict.keys()):
        to_file += "["
        n = 1
        for item in assignment_dict[key]:
            if type(item) == list:
                y = 0
             #   for val in item:
             #       to_file += str(item)
             #       if n != len(i) - 2:
             #           to_file += ", "
             #       y = y + 1
            else:
                to_file += "\"" + dict_ind_to_ticker[str(item)] + "\""
                if n != len(assignment_dict[key]):
                    to_file += ", "
                n = n + 1
        to_file += "]"
        if k != len(assignment_dict):
            to_file += ", "
        k = k + 1
        print(assignment_dict[key])
    to_file += "],"
    to_file += "\n\t\"centroid_values\": "
    to_file += "["
    t = 1
    for list_outter in cluster_DTW.get_centroids():
        to_file += "["
        m = 0
        for item in list_outter:
            to_file += str(item)
            if m != len(list_outter) - 1:
                to_file += ','
            m = m + 1
        to_file += "]"
        if t != len(cluster_DTW.get_centroids()):
            to_file += ","
        t = t + 1
    to_file += "]"
    to_file += "\n},"

fout.write(to_file)
        
#print(cluster_DTW.get_centroids())
#for i in cluster_DTW.centroids:
#    plt.plot(i)
#for i in cluster_DTW.assignments.values():
#    for j in i:
#        print(str(j))
#    print()

#plt.show()
