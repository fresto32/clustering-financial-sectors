The business cycle stipulates that some companies do better in some parts of the cycle than others. For instance, during a boom, firms that sell luxury goods outperform those that sell nescessity goods. Conversely, during an economic trough, firms that sell nescessity goods outperform those that sell luxury goods. So, can we cluster these different types of companies using dynamic time warping and the k-means algorithm?

This project collates the ASX 200 data and clusters the data using the k-means algorithm and dynamic time warping as seen in tajpereira.com/clustering_project.

The k-means algorithm is implemented by assigning each data point to a randomly assigned centroid according to euclidean distance to the mean centroid cluster. Subsequently, each data point is tested against all centroid means to determine if the data point is in fact in its closest centroid.

Dynamic time warping involves horizontally shifting a curve to minimize the euclidean distance from another curve. A fantastic tutorial on dynamic time warping is given by Alex Minnaar at https://github.com/alexminnaar/time-series-classification-and-clustering/blob/master/Time%20Series%20Classification%20and%20Clustering.ipynb. 

ASX 200 Data Source: https://www.asxhistoricaldata.com/archive/
