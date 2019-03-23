import numpy as np

def tdist(a, b):
    d = (a - b)
    return np.linalg.norm(d.reshape(-1))

def is_ltri(m):
    return np.allclose(m, np.tril(m))

def is_utri(m):
    return np.allclose(m, np.triu(m))

def is_udiag(m):
    return np.allclose(m, np.diag(np.diag(m)))
