import numpy as np
import matplotlib.pyplot as plt

def city_distance_stats():
    ''' prints the expected value of distance between cities generated by
        different random procedures '''

    def elementwise_dists(points1, points2):
        return np.linalg.norm(points1 - points2, axis=1)

    def distance_stats(distances, info_str):
        mean = np.average(distances)
        std = np.std(distances)
        print '{}\n  mean: {:0.2f}\n  std:  {:0.2f}\n'.format(info_str, mean, std)

    distance_stats(elementwise_dists(np.random.randn(10000, 2), np.random.randn(10000, 2)),
                   'normal distrib. with mean 0, variance 1')

    D = 50
    distance_stats(elementwise_dists(np.random.rand(10000, 2)*D, np.random.rand(10000, 2)*D),
                   'uniform distribution [0,%d]' % D)

    def pairwise_wrapping_distances(points, L):
        N = len(points)
        assert(len(points[0]) == 2)

        pairwise_dists = np.empty( (N*(N-1)) / 2 )
        p_i = 0

        for i in xrange(len(points)):
            for j in xrange(i+1, len(points)):
                p1 = points[i]
                p2 = points[j]
                wraps = np.empty(4)
                wraps[0] = np.linalg.norm(p1 - p2)
                p2[0] -= L
                wraps[1] = np.linalg.norm(p1 - p2)
                p2[1] -= L
                wraps[2] = np.linalg.norm(p1 - p2)
                p2[0] += L
                wraps[3] = np.linalg.norm(p1 - p2)
                p2[1] += L

                pairwise_dists[p_i] = np.min(wraps)
                p_i += 1
        return pairwise_dists

    dists = pairwise_wrapping_distances(np.random.rand(100, 2)*100, 100)
    distance_stats(dists, 'uniform distribution [0,100] with wraparound')


city_distance_stats()
quit()

N = 1000
cities = np.random.randn(2, N)
plt.scatter(cities[0], cities[1])
plt.title('Normal distribution, mean=0, var=1, N=%d' % N)
plt.show()

cities = np.random.rand(2, N) * 2 - 1
plt.scatter(cities[0], cities[1])
plt.title('Uniform distribution, [1,2], N=%d' % N)
plt.show()
