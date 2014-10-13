
INF = float('inf')

# Young's tableu
class young_tableu:
    def __init__(self, m, n):
        self.m = m
        self.n = n
        self.y = [[INF]*n for i in xrange(m)]

    # just like min heap
    def youngify(self, i, j):
        # init:
        y = self.y
        m, n = self.m, self.n
        # begin:
        min_i = i
        min_j = j
        if i+1 < m and y[i][j] > y[i+1][j]:
            min_i, min_j = i+1, j
        if j+1 < n and y[min_i][min_j] > y[i][j+1]:
            min_i, min_j = i, j+1
        if min_i != i or min_j != j:
            #exchange(y[i][j], y[min_i][min_j])
            #this is swap!
            y[i][j], y[min_i][min_j] = y[min_i][min_j], y[i][j]
            self.youngify(min_i, min_j)
 
    def extract_min(self):
        x = self.y[0][0]
        self.y[0][0] = INF
        self.youngify(0, 0)
        return x

    def insert_(self, k):
        m, n = self.m, self.n   
        self.decrease_key(m-1, n-1, k)

    def decrease_key(self, i, j, k):
        y = self.y
        # begin:
        if y[i][j] <= k:
            print 'error'
            return 'error' # full?
        y[i][j] = k
        threashold = INF
        max_i, max_j = i, j
        while i > 0 or j > 0 and y[i][j] < threshold:
            # swap
            y[i][j], y[max_i][max_j] = y[max_i][max_j], y[i][j]
            i, j = max_i, max_j
            if i-1 >= 0 and y[i][j] < y[i-1][j]:
                max_i, max_j = i-1, j
            if j-1 >= 0 and y[max_i][max_j] < y[i][j-1]:
                max_i, max_j = i, j-1
            threshold = y[max_i][max_j]



    def insert(self, k):
        y = self.y
        m, n = self.m, self.n
        y[m-1][n-1] = k
        youngify_backward(y, m-1, n-1)

    def sort(self,a):
        l = self.m*self.n
        
        for i in xrange(l):
            self.insert(a[i])
        # copy back to a
        for i in xrange(l):
            a[i] = self.extract_min()

# static
def exchange(x, y):
    x, y = y, x

def youngify_backward(y, i, j):
    max_i, max_j = i, j
    if i-1 >= 0 and y[i][j] < y[i-1][j]:
        max_i, max_j = i-1, j
    if j-1 >= 0 and y[max_i][max_j] < y[i][j-1]:
        max_i, max_j = i, j-1
    if max_i != i or max_j != j:
        y[i][j], y[max_i][max_j] = y[max_i][max_j], y[i][j]
        youngify_backward(y, max_i, max_j)

def main():
    print 'young tableu:'
    t=young_tableu(3,3)
    #t.y[0]=[1,4,7]
    #t.y[1]=[2,5,8]
    #t.y[2]=[3,6,9]
    #a=[1,4,7,2,5,8,3,6,9]
    a=[1,3,5,7,9,8,6,4,2]
    t.sort(a)

    print 'sorted: ',a

if __name__ == "__main__":
    main()
