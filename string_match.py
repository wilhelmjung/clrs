# string matching algorithm.
import math
import time

def naive_string_matcher(t, p):
    n = len(t)
    m = len(p)
    for s in range(n - m):
        if p[0 : m] == t[s : s + m]: # p==t[s:s+m]
            print 'Pattern occurs with shift', s


# text, pattern, radix, prime
def rabin_karp_matcher(T, P, d, q):
    n = len(T)
    m = len(P)
    hit = 0
    h = int(math.pow(d, m - 1) % q)
    #print 'm = %s, n = %s, q = %s, h = %s'%(m, n, q, h)
    p = 0
    t = {}; t[0] = 0 # t0 = 0, NOTE: subscript for t can be dropped.
    for i in range(m): # preprocessing
        p    = (d * p    + ord(P[i])) % q
        t[0] = (d * t[0] + ord(T[i])) % q
    for s in range(n - m + 1): # matching # shift: [0..n-m]
        if p == t[s]:
            if P[0 : m] == T[s : s + m]:
                print 'Pattern occurs with shift', s
            else: # spurious hit
                print 'spurious hit @', s; hit = hit + 1
        if s < n - m: # s != n - m, compute t[1] to t[n-m-1]
            t[s + 1] = (d * (t[s] - ord(T[s]) * h) + ord(T[s + m])) % q
    #if hit > 0: print 'spurious hits count: %s for n = %s'%(hit, n)


# automaton machine method
def compute_transition_function(P, S): # sigma
    D = {} # delta function as a dictionary.
    m = len(P)
    for q in range(m + 1): # [0..m]
        for a in S:
            k = min(m + 1, q + 2)
            while True: # repeat
                k = k - 1
                if (P[:q]+a).endswith(P[:k]): # until Pk is a suffix of Pq+a
                    break
            # delta(q, a) = k
            D[q, a] = k
    return D


# Text, Delta, m of P
def finite_automaton_matcher(T, D, m):
    n = len(T)
    q = 0
    for i in range(n):
        q = D[q, T[i]]
        if q == m:
            print 'Pattern occurs with shift', i - m + 1


def fa_matcher(T, P):
    S = ' \n\t_0123456789abcdefghijklmnopqrstuvwxyz' # sigma
    D = compute_transition_function(P, S)
    #print 'Delta:',D
    m = len(P)
    finite_automaton_matcher(T, D, m)


# Knuth-Morris-Pratt string match algorithm.
def compute_prefix_function(P):
    m = len(P)
    P = '*' + P # subscript fix.
    pi = [0 for x in range(m + 1)] # a new array, subscript fix.
    pi[1] = 0 # q > k, 1 > 0, k: [0..q-1]
    k = 0
    for q in range(2, m + 1): # [2..m]
        while k > 0 and P[k + 1] != P[q]: # next char does not match
            k = pi[k]
        if P[k + 1] == P[q]: # next char matches
            k = k + 1
        pi[q] = k
    return pi


def kmp_matcher(T, P):
    n, m = len(T), len(P)
    pi = compute_prefix_function(P)
    #print 'PI:', pi[1:] # subscript fix.
    q = 0
    T = '*' + T # padding at subscript 0.
    P = '*' + P # padding at subscript 0.
    for i in range(1, n + 1):
        while q > 0 and P[q + 1] != T[i]:
            q = pi[q]
        if P[q + 1] == T[i]:
            q = q + 1
        if q == m:
            print 'Pattern occurs with shift', i - m
            q = pi[q] # look for the next match

if __name__ == "__main__":
  
    t = 'one twooxx three ooxx foobar'*10
    p = 'x three o'
    print 'The naive matcher:'
    naive_string_matcher(t, p)
    
    print 'The Rabin-Karp matcher:'
    rabin_karp_matcher(t, p, 256, 57)

    print 'FA matcher:'
    fa_matcher(t, p)

    print 'KMP matcher:'
    kmp_matcher(t, p)
