import numpy as np
import scipy

import metrics

FEPS = 1e-8

NTESTS = 1000


def true_svd(x):
    u, s, vt = np.linalg.svd(x)
    return u, s, vt

def my_svd(x):
    return x, x, x

def check_svd(x, u, s, vt):
    m = x.shape[0]
    n = x.shape[1]
    k = min(m, n)
    if u.shape[0] != m or u.shape[1] != m:
        raise Exception('Invalid size for u')
    if vt.shape[0] != n or vt.shape[1] != n:
        raise Exception('Invalid size for v')
    if len(s) != k:
        raise Exception('Invalid size for s')

    d = metrics.tdist(u @ u.T, np.eye(m))
    if d >= FEPS:
        raise Exception('u @ u.t != I => dist = {}'.format(d))
    d = metrics.tdist(u.T @ u, np.eye(m))
    if d >= FEPS:
        raise Exception('u.t @ u != I => dist = {}'.format(d))

    d = metrics.tdist(vt.T @ vt, np.eye(n))
    if d >= FEPS:
        raise Exception('v @ v.t != I => dist = {}'.format(d))
    d = metrics.tdist(vt @ vt.T, np.eye(n))
    if d >= FEPS:
        raise Exception('v.t @ v != I => dist = {}'.format(d))

    s_mat = np.zeros((m, n))
    for i in range(k):
        s_mat[i][i] = s[i]
        
    d = metrics.tdist(u @ s_mat @ vt, x)
    if d >= FEPS:
        raise Exception('u @ s @ v.t != x => dist = {}'.format(d))



if __name__ == '__main__':


    for i in range(NTESTS):
        print('Test {}/{}'.format(i+1, NTESTS))

        m = np.random.randint(1, 10)
        n = np.random.randint(1, 10)
        x = np.random.randn(m, n) * 10
        
        #u, s, vt = true_svd(x)
        u, s, vt = my_svd(x)
        check_svd(x, u, s, vt)
    
