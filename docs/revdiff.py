import numpy as np

USE_CONV2D_MATMUL = True


def my_expand_dims3(x, size):
    y = np.empty((x.shape[0], size, x.shape[1]))
    for i in range(x.shape[0]):
        for j in range(size):
            for k in range(x.shape[1]):
                y[i, j, k] = x[i, k]
    return y

def my_conv2d_naive_val(X, K, n, f, y, x):
    res = 0
    for c in range(K.shape[1]):
        for i in range(K.shape[2]):
            for j in range(K.shape[3]):
                res += X[n, c, y+i, x+j] * K[f, c, i, j]
    return res

def my_conv2d_naive(X, K, sh, sw):
    if USE_CONV2D_MATMUL:
        return my_conv2d_matmul(X, K, sh, sw)
    h_y = int((X.shape[2] - K.shape[2]) / sh + 1)
    w_y = int((X.shape[3] - K.shape[3]) / sw + 1)
    
    Y = np.empty((X.shape[0], K.shape[0], h_y, w_y))
    
    for n in range(X.shape[0]):
        for f in range(K.shape[0]):
            for y in range(h_y):
                for x in range(w_y):
                    Y[n, f, y, x] = my_conv2d_naive_val(X, K, n, f, y*sh, x*sw)
                    
    return Y

def x2mat(X, K, sh, sw):
    hy = int((X.shape[2] - K.shape[2]) / sh + 1)
    wy = int((X.shape[3] - K.shape[3]) / sw + 1)
    
    mx = np.empty((X.shape[0], hy, wy,
                   X.shape[1] * K.shape[2] * K.shape[3]))
    
    for n in range(X.shape[0]):
        for i in range(hy):
            for j in range(wy):
                v = X[n, :, i*sh:i*sh+K.shape[2], j*sw:j*sw+K.shape[3]]
                mx[n, i, j] = v.reshape(-1)
     
    return mx.reshape(-1, mx.shape[3])

def my_conv2d_matmul(X, K, sh, sw):
    
    hy = int((X.shape[2] - K.shape[2]) / sh + 1)
    wy = int((X.shape[3] - K.shape[3]) / sw + 1)
    
    mX = x2mat(X, K, sh, sw)
    mK = K.reshape(K.shape[0], -1)
    mY = mK @ mX.T
    Y = mY.reshape(K.shape[0], X.shape[0], hy, wy)
    Y = np.transpose(Y, (1, 0, 2, 3))
    return Y

def my_pad_tensor(X, ptop, pbot, pleft, pright):
    Y = np.zeros((X.shape[0], X.shape[1], X.shape[2] + ptop + pbot,
                  X.shape[3] + pleft + pright))
    Y[:, :, ptop:ptop+X.shape[2], pleft:pleft+X.shape[3]] = X
    return Y

def my_conv2d_padded_naive(X, K, sh, sw, ph, pw):
    return my_conv2d_naive(my_pad_tensor(X, ph, ph, pw, pw), K, sh, sw)


def my_stride0(X, h, w):
    
    Y = np.zeros((X.shape[0], X.shape[1],
                 1 + (h + 1) * (X.shape[2] - 1), 
                 1 + (w + 1) * (X.shape[3] - 1)))
    
    for i1 in range(X.shape[0]):
        for i2 in range(X.shape[1]):
            for i3 in range(X.shape[2]):
                for i4 in range(X.shape[3]):
                    Y[i1, i2, i3 * (h+1), i4 * (w+1)] = X[i1, i2, i3, i4]
                
    return Y

def my_rot180(X):
    
    Y = np.empty(X.shape)
    for i1 in range(X.shape[0]):
        for i2 in range(X.shape[1]):
            for i3 in range(X.shape[2]):
                for i4 in range(X.shape[3]):
                    Y[i1, i2, i3, i4] = X[i1, i2, 
                                        X.shape[2] - i3 - 1,
                                        X.shape[3] - i4 - 1]
                    
    return Y


def my_conv2d_padded_dk_naive(X, dY, sh, sw, ph, pw):
    Xtr = np.transpose(my_pad_tensor(X, ph, ph, pw, pw), (1, 0, 2, 3))
    f_dY = np.transpose(dY, (1,0,2,3))
    f_dY = my_stride0(f_dY, sh - 1, sw - 1)
    o_dK = my_conv2d_naive(Xtr, f_dY, 1, 1)
    return np.transpose(o_dK, (1,0,2,3))

def my_conv2d_padded_dx_naive(K, dY, sh, sw, ph, pw):
    pdY = my_pad_tensor(my_stride0(dY, sh - 1, sw - 1), 
                     K.shape[2] - 1, K.shape[2] - 1,
                     K.shape[3] - 1, K.shape[3] - 1)
    K180 = np.transpose(my_rot180(K), (1, 0, 2, 3))
    dX_full = my_conv2d_naive(pdY, K180, 1, 1)
    dX = dX_full[:,:,ph:dX_full.shape[2]-ph, pw:dX_full.shape[3]-pw]
    return dX

def my_conv2d_bias_add(X, b):
    return np.transpose(np.transpose(X, (0, 3, 2, 1)) + b, (0, 3, 2, 1))

def my_maxpool(X, kh, kw, sh, sw):
    
    hy = int((X.shape[2] - kh) / sh) + 1
    wy = int((X.shape[3] - kw) / sw) + 1
    
    Y = np.empty((X.shape[0], X.shape[1], hy, wy))
    
    for n in range(X.shape[0]):
        for c in range(X.shape[1]):
            for i in range(hy):
                for j in range(wy):
                    Y[n, c, i, j] = np.max(X[n, c, i*sh:i*sh+kh,
                                             j*sw:j*sw+kw])
    return Y

def my_maxpool_dk(X, Y, dout, kh, kw, sh, sw):
    hy = int((X.shape[2] - kh) / sh) + 1
    wy = int((X.shape[3] - kw) / sw) + 1
    
    dX = np.zeros(X.shape)
    
    for n in range(Y.shape[0]):
        for c in range(Y.shape[1]):
            for i in range(Y.shape[2]):
                for j in range(Y.shape[3]):
                    m = Y[n, c, i, j]
                    for xi in range(i*sh,i*sh+kh):
                        for xj in range(j*sw,j*sw+kw):
                            if X[n, c, xi, xj] == m:
                                dX[n, c, xi, xj] += dout[n, c, i, j]
                                
    return dX







def to_node(x):
    if isinstance(x, (int, float)):
        x = np.array(x)
    if isinstance(x, (np.ndarray, np.generic)):
        return Val(x.astype(np.float32))
    elif isinstance(x, Node):
        return x
    else:
        raise Exception('to_node(x): x is of bad type')

def to_nodes(xs):
    return (to_node(x) for x in xs)
    
class Node:

    def __init__(self, shape, name, preds):
        self.shape = shape
        self.name = name
        self.preds = list(preds)
        self.succs = []
        self.grads = dict()
        self.value = None
        self.native = False
        for p in preds: p.succs.append(self)

    def size(self):
        res = 1
        for x in self.shape: res *= x
        return res

    '''
    Compute the value of the node
    '''
    def eval(self):
        if self.value is None:
            self.value = self.compute_native() if self.native else self.compute_value()
        return self.value

    '''
    Discard the value of the node. must be recomputed
    '''
    def discard(self):
        for s in self.succs:
            s.discard()
        self.value = None

    def compute_value(self):
        raise Exception('Node::compute_value() not implemented')

    def compute_native(self):
        raise Exception('Node::compute_native() not implemented')
    
    '''
    Check if x is in the succs nodes of self
    '''
    def is_ancestor_of(self, x):
        if self == x:
            return True
        for s in self.succs:
            if s.is_ancestor_of(x): return True
        return False

    '''
    Build and return a new node to compute the gradent
    @param pred - the predecessor to build gradient relative to it
    @param dout - the gradient for this node
    '''
    def get_grad(self, pred, dout):
        raise Exception('Node::get_grad(pred, dout) not implemnted')

    def fun_str(self):
        res = self.name + '('
        for i in range(len(self.preds)):
            if i > 0: res += ', '
            res += self.preds[i].fun_str()
        res += ')'
        return res
    
    def __add__(self, other):
        return build_vadd(self, other)
    def __sub__(self, other):
        return build_vsub(self, other)
    def __mul__(self, other):
        return build_vmul(self, other)
    def __truediv__(self, other):
        return build_vdiv(self, other)
    def __neg__(self):
        return build_vneg(self)
    def __radd__(other, self):
        return build_vadd(other, self)
    def __rsub__(self, other):
        return build_vsub(other, self)
    def __rmul__(self, other):
        return build_vmul(other, self)
    def __rtruediv__(self, other):
        return build_vdiv(other, self)
    

class Val(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'val', [])
        self.value = x

    def get_grad(self, pred, dout):
        return dout

    def fun_str(self):
        return str(self.shape)

    def update(self, new_val):
        self.discard()
        self.value = new_val

    def compute_value(self):
        raise Exception('Val::compte_value() should never be called: {}'.format(self.shape))
        

class Vadd(Node):

    def __init__(self, x, y):
        super().__init__(x.shape, 'vadd', [x, y])

    def compute_value(self):
        return self.preds[0].eval() + self.preds[1].eval()

    def get_grad(self, pred, dout):
        return dout

class Vsub(Node):

    def __init__(self, x, y):
        super().__init__(x.shape, 'sub', [x, y])

    def compute_value(self):
        return self.preds[0].eval() - self.preds[1].eval()

    def get_grad(self, pred, dout):
        if pred == self.preds[0]:
            return dout
        else:
            return - dout

class Vmul(Node):

    def __init__(self, x, y):
        super().__init__(x.shape, 'vmul', [x, y])

    def compute_value(self):
        return self.preds[0].eval() * self.preds[1].eval()

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return self.preds[1] * dout
        else:
            return self.preds[0] * dout

class Vdiv(Node):

    def __init__(self, x, y):
        super().__init__(x.shape, 'vdiv', [x, y])

    def eval(self):
        return self.preds[0].eval() / self.preds[1].eval()

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return dout / self.preds[1]
        else:
            return - dout * self.preds[0] / (self.preds[1] * self.preds[1])

class Vneg(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'neg', [x])

    def compute_value(self):
        return - self.preds[0].eval()

    def get_grad(self, pred, dout):
        return - dout

class Vsadd(Node):

    def __init__(self, x, y):
        super().__init__(y.shape, 'vsadd', [x, y])

    def compute_value(self):
        return self.preds[0].eval() + self.preds[1].eval()

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return op_sum(dout, axis=0)
        else:
            return dout
    
class Vsmul(Node):

    def __init__(self, x, y):
        super().__init__(y.shape, 'vsmul', [x, y])

    def compute_value(self):
        return self.preds[0].eval() * self.preds[1].eval()

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return build_dot_vv(dout, self.preds[1])
        else:
            return build_vsmul(self.preds[0], dout)

class Vsdiv(Node):

    def __init__(self, x, y):
        super().__init__(y.shape, 'vsdiv', [x, y])

    def compute_value(self):
        return self.preds[0].eval() / self.preds[1].eval()

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return build_dot_vv(dout, build_vsdiv(1, self.preds[1]))
        else:
            return build_vsmul(-self.preds[0], dout) / (pred * pred)

class Vexp(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vexp', [x])

    def compute_value(self):
        return np.exp(self.preds[0].eval())

    def get_grad(self, pred, dout):
        return dout * build_vexp(pred)

class Vlog(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vlog', [x])

    def compute_value(self):
        return np.log(self.preds[0].eval())

    def get_grad(self, pred, dout):
        return dout / pred

class Vsigmoid(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vsigmoid', [x])

    def compute_value(self):
        return  1 / (1 + np.exp(-self.preds[0].eval()))

    def get_grad(self, pred, dout):
        return dout * self * op_vsadd(1, -self)

class Vrelu(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vrelu', [x])

    def compute_value(self):
        x = self.preds[0].eval()
        return np.maximum(0, x)

    def get_grad(self, pred, dout):
        return dout * build_vrelu_prime(pred)

class VreluPrime(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vreluprime', [x])

    def compute_value(self):
        x = self.preds[0].eval()
        return (x > 0).astype(x.dtype)

    def get_grad(self, pred, dout):
        raise Exception('Vreluprime::get_grad not implemented')

class VleakyRelu(Node):

    def __init__(self, x, alpha):
        super().__init__(x.shape, 'vrelu', [x])
        self.alpha = alpha

    def compute_value(self):
        x = self.preds[0].eval()
        return np.maximum(0, x) + self.alpha * np.minimum(0, x)

    def get_grad(self, pred, dout):
        return dout * build_vleaky_relu_prime(pred, self.alpha)

class VleakyReluPrime(Node):

    def __init__(self, x, alpha):
        super().__init__(x.shape, 'vleakyreluprime', [x])
        self.alpha = alpha

    def compute_value(self):
        x = self.preds[0].eval()
        return (x > 0).astype(x.dtype) + self.alpha * (x <= 0).astype(x.dtype)

    def get_grad(self, pred, dout):
        raise Exception('VleakyReluPrime::get_grad not implemented')

class Velu(Node):

    def __init__(self, x, alpha):
        super().__init__(x.shape, 'velu', [x])
        self.alpha = alpha

    def compute_value(self):
        x = self.preds[0].eval()
        return np.maximum(0, x) + np.minimum(0, self.alpha * (np.exp(x) - 1))

    def get_grad(self, pred, dout):
        return dout * build_velu_prime(pred, self.alpha)

class VeluPrime(Node):

    def __init__(self, x, alpha):
        super().__init__(x.shape, 'veluprime', [x])
        self.alpha = alpha

    def compute_value(self):
        x = self.preds[0].eval()
        return (x > 0).astype(x.dtype) + self.alpha * np.exp(x) * (x <= 0).astype(x.dtype)

    def get_grad(self, pred, dout):
        raise Exception('VeluPrime::get_grad not implemented')
    
class Vsoftplus(Node):

    def __init__(self, x, beta = 1):
        super().__init__(x.shape, 'vsoftplus', [x])
        self.beta = beta

    def compute_value(self):
        x = self.preds[0].eval()
        return  (1 / self.beta) * np.log(1 + np.exp(self.beta * x))

    def get_grad(self, pred, dout):
        return dout * build_vsigmoid(build_vsmul(self.beta, pred))

class Vtanh(Node):

    def __init__(self, x, beta = 1):
        super().__init__(x.shape, 'vtanh', [x])
        self.beta = beta

    def compute_value(self):
        x = self.preds[0].eval()
        return np.tanh(x)

    def get_grad(self, pred, dout):
        return dout * build_vsadd(1, - self * self)

class Vsign(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'vsign', [x])

    def compute_value(self):
        return np.sign(self.preds[0].eval())

    def get_grad(self, pred, dout):
        raise Exception('DotMmt::Vsign not implemented')

class Softmax(Node):

    def __init__(self, x):
        super().__init__(x.shape, 'softmax', [x])

    def compute_value(self):
        x = self.preds[0].eval()
        x = x - np.max(x, axis=1, keepdims=True)
        x_e = np.exp(x)
        return x_e / np.sum(x_e, axis=1, keepdims=True)

    def get_grad(self, pred, dout):
        raise Exception('Softmax: get_grad not implemented')

'''
class Sum(Node):

    def __init__(self, x):
        super().__init__((), 'sum', [x])

    def compute_value(self):
        return np.sum(self.preds[0].eval())

    def get_grad(self, pred, dout):
        return build_vsmul(dout, np.ones(pred.shape))
'''

class Sum3(Node):

    def __init__(self, x):
        super().__init__((x.shape[0], x.shape[2]), 'sum3', [x])

    def compute_value(self):
        return np.sum(self.preds[0].eval(), axis=1)

    def get_grad(self, pred, dout):
        return build_expand_dims(dout, 1, pred.shape[1])

class ExpandDims(Node):

    def __init__(self, x, axis, size):
        new_shape = x.shape[:axis] + (size,) + x.shape[axis+1:]
        super().__init__(new_shape, 'expanddims', [x])

    def compute_value(self):
        return my_expand_dims3(self.preds[0].eval(), self.shape[1])

    def get_grad(self, pred, dout):
        raise Exception('ExpandDims::get_grad not implemented')

class DotVv(Node):

    def __init__(self, x, y):
        super().__init__((), 'dotvv', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval(), self.preds[1].eval())

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return build_vsmul(dout, self.preds[1])
        else:
            return build_vsmul(dout, self.preds[0])

class Outer(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[0], y.shape[0]), 'outer', [x, y])

    def compute_value(self):
        return np.outer(self.preds[0].eval(), self.preds[1].eval())

    def get_grad(self, pred, dout):
        raise Exception('Outer::get_grad() not implemented')
        if self.preds[0] == pred:
            return build_vsmul(dout, self.preds[1])
        else:
            return build_vsmul(dout, self.preds[0])
        
class DotMv(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[0],), 'dotmv', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval(), self.preds[1].eval())

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return build_outer(dout, self.preds[1])
        else:
            return build_dot_mtv(self.preds[0], dout)

class DotMtv(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[1],), 'dotmtv', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval().T, self.preds[1].eval())

    def get_grad(self, pred, dout):
        raise Exception('DotMtv::get_grad not implemented')

class DotMm(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[0], y.shape[1]), 'dotmm', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval(), self.preds[1].eval())

    def get_grad(self, pred, dout):
        if self.preds[0] == pred:
            return build_dot_mmt(dout, self.preds[1])
        else:
            return build_dot_mtm(self.preds[0], dout)

class DotMtm(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[1], y.shape[1]), 'dotmtm', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval().T, self.preds[1].eval())

    def get_grad(self, pred, dout):
        raise Exception('DotMtm::get_grad not implemented')

class DotMmt(Node):

    def __init__(self, x, y):
        super().__init__((x.shape[0], y.shape[0]), 'dotmmt', [x, y])

    def compute_value(self):
        return np.dot(self.preds[0].eval(), self.preds[1].eval().T)

    def get_grad(self, pred, dout):
        raise Exception('DotMmt::get_grad not implemented')

class Norm1(Node):

    def __init__(self, x):
        super().__init__((), 'norm1', [x])

    def compute_value(self):
        return np.linalg.norm(self.preds[0].eval(), ord=1)

    def get_grad(self, pred, dout):
        return build_vsmul(dout, build_vsign(pred))
        
    
class Reshape(Node):

    def __init__(self, x, shape):
        super().__init__(shape, 'reshape', [x])
        self.old_shape = x.shape

    def compute_value(self):
        return self.preds[0].eval().reshape(self.shape)

    def get_grad(self, pred, dout):
        return build_reshape(dout, self.old_shape)

class AddBias(Node):

    def __init__(self, x, y):
        super().__init__(x.shape, 'addbias', [x, y])

    def compute_value(self):
        return self.preds[0].eval() + self.preds[1].eval()

    def get_grad(self, pred, dout):
        if pred == self.preds[0]:
            return dout
        else:
            return op_sum(dout, axis=0)

class Conv2D(Node):

    def __init__(self, X, K, sh, sw, ph, pw):
        h_y = int((X.shape[2] - K.shape[2] + 2 * ph) / sh + 1)
        w_y = int((X.shape[3] - K.shape[3] + 2 * pw) / sw + 1)
        super().__init__((X.shape[0], K.shape[0], h_y, w_y), 'conv2d', [X, K])
        self.sh = sh
        self.sw = sw
        self.ph = ph
        self.pw = pw

    def compute_value(self):
        X = self.preds[0].eval()
        K = self.preds[1].eval()
        return my_conv2d_padded_naive(X, K, self.sh, self.sw, self.ph, self.pw)

    def get_grad(self, pred, dout):
        if pred == self.preds[0]:
            return build_conv2d_dx(self.preds[1], dout,
                                   self.sh, self.sw, self.ph, self.pw,
                                   self.preds[0].shape)
        else:
            return build_conv2d_dk(self.preds[0], dout,
                                   self.sh, self.sw, self.ph, self.pw,
                                   self.preds[1].shape)


class Conv2DDx(Node):

    def __init__(self, K, dY, sh, sw, ph, pw, x_shape):
        super().__init__(x_shape, 'conv2ddx', [K, dY])
        self.sh = sh
        self.sw = sw
        self.ph = ph
        self.pw = pw

    def compute_value(self):
        K = self.preds[0].eval()
        dY = self.preds[1].eval()
        return my_conv2d_padded_dx_naive(K, dY, self.sh, self.sw, self.ph, self.pw)

    def get_grad(self, pred, dout):
        K = self.preds[0]
        X = self.preds[1]
        if pred == self.preds[0]:
            return build_conv2d_dk(dout, X, self.sh, self.sw, self.ph, self.pw, K.shape) 
        else:
            return build_conv2d(dout, K, self.sh, self.sw, self.ph, self.pw)
        

class Conv2DDk(Node):

    def __init__(self, X, dY, sh, sw, ph, pw, k_shape):
        super().__init__(k_shape, 'conv2ddk', [X, dY])
        self.sh = sh
        self.sw = sw
        self.ph = ph
        self.pw = pw

    def compute_value(self):
        X = self.preds[0].eval()
        dY = self.preds[1].eval()
        return my_conv2d_padded_dk_naive(X, dY, self.sh, self.sw, self.ph, self.pw)

    def get_grad(self, pred, dout):
        raise Exception('Conv2DDk::get_grad() not implemented')

class Conv2DBiasAdd(Node):

    def __init__(self, X, b):
        super().__init__(X.shape, 'conv2dbiasadd', [X, b])

    def compute_value(self):
        X = self.preds[0].eval()
        b = self.preds[1].eval()
        return my_conv2d_bias_add(X, b)

    def get_grad(self, pred, dout):
        if pred == self.preds[0]:
            return dout
        else:
            return build_conv2d_bias_add_grad(dout)

class Conv2DBiasAddGrad(Node):

    def __init__(self, dout):
        super().__init__((dout.shape[1],), 'conv2dbiasaddgrad', [dout])

    def compute_value(self):
        dout = self.preds[0].eval()
        return np.sum(dout, axis=(0, 2, 3))

    def get_grad(self, pred, dout):
        raise Exception('Conv2dBiasAddGrad::get_grad not implemented')

class MaxPooling(Node):

    def __init__(self, X, kh, kw, sh, sw):
        hy = int((X.shape[2] - kh) / sh) + 1
        wy = int((X.shape[3] - kw) / sw) + 1
        super().__init__((X.shape[0], X.shape[1], hy, wy), 'maxpooling', [X])
        self.kh = kh
        self.kw = kw
        self.sh = sh
        self.sw = sw

    def compute_value(self):
        X = self.preds[0].eval()
        return my_maxpool(X, self.kh, self.kw, self.sh, self.sw)

    def get_grad(self, pred, dout):
        return build_max_pooling_grad(self, dout, self.kh, self.kw, self.sh, self.sw)

class MaxPoolingGrad(Node):

    def __init__(self, Y, dout, kh, kw, sh, sw):
        super().__init__((Y.preds[0].shape), 'maxpoolinggrad', [Y, dout])
        self.kh = kh
        self.kw = kw
        self.sh = sh
        self.sw = sw

    def compute_value(self):
        X = self.preds[0].preds[0].eval()
        Y = self.preds[0].eval()
        dout = self.preds[1].eval()
        return my_maxpool_dk(X, Y, dout, self.kh, self.kw, self.sh, self.sw)

    def get_grad(self, pred, dout):
        raise Exception('MaxPoolingGrad::get_grad not implemented')
    

class MAELoss(Node):

    def __init__(self, y_pred, y_true):
        super().__init__((), 'maeloss', [y_pred, y_true])

    def compute_value(self):
        y_pred = self.preds[0].eval()
        y_true = self.preds[1].eval()
        n = len(y_pred)
        return (1/1) * np.sum(np.abs(y_true - y_pred))

    def get_grad(self, pred, dout):
        y_pred = self.preds[0]
        y_true = self.preds[1]

        if pred == y_pred:
            return build_vsmul(dout, build_vsign(y_pred - y_true))
        else:
            return build_vsmul(dout, build_vsign(y_true - y_pred))

class MSELoss(Node):

    def __init__(self, y_pred, y_true):
        super().__init__((), 'mseloss', [y_pred, y_true])

    def compute_value(self):
        y_pred = self.preds[0].eval()
        y_true = self.preds[1].eval()
        n = 1 #len(y_pred)
        return (1/n) * np.sum((y_true - y_pred)**2)

    def get_grad(self, pred, dout):
        y_pred = self.preds[0]
        y_true = self.preds[1]
        n = 1 #y_pred.shape[0]

        if pred == y_pred:
            return build_vsmul((2 * n) * dout, y_pred - y_true)
        else:
            return build_vsmul((2 * n) * dout, y_true - y_pred)

class BCELoss(Node):

    def __init__(self, y_pred, y_true):
        super().__init__((), 'bceloss', [y_pred, y_true])

    def compute_value(self):
        y_pred = self.preds[0].eval()
        y_true = self.preds[1].eval()
        y_pred = 1 / (1 + np.exp(-y_pred))
        j = - np.sum(y_true * np.log(y_pred))
        return - np.sum(y_true * np.log(y_pred) + (1-y_true) * np.log(1-y_pred))

    def get_grad(self, pred, dout):
        y_pred = self.preds[0]
        y_true = self.preds[1]
        y_pred = build_vsigmoid(y_pred)
        
        if self.preds[0] == pred:
            return build_vsmul(dout, y_pred - y_true)
        else:
             raise Exception('BCELoss::get_grad not implemented for y_true')

class CrossEntropyLoss(Node):

    def __init__(self, y_pred, y_true):
        super().__init__((), 'crossentropyloss', [y_pred, y_true])

    def compute_value(self):
        x_e = np.exp(self.preds[0].eval())
        y_pred = x_e / np.sum(x_e, axis=1, keepdims=True)
        y_true = self.preds[1].eval()
        return - np.sum(y_true * np.log(y_pred))

    def get_grad(self, pred, dout):
        y_pred = self.preds[0]
        y_true = self.preds[1]
        y_pred = build_softmax(y_pred)
        
        if self.preds[0] == pred:
            return op_vsmul(dout, y_pred - y_true)
        else:
             raise Exception('CrossEntropyget_grad not implemented for y_true')
        
def build_val(x):
    return to_node(x)
    
def build_vadd(x, y):
    x, y = to_nodes((x, y))
    if x.shape != y.shape:
        raise Exception('vadd: operands must have the same shape')
    
    return Vadd(x, y)

def build_vsub(x, y):
    x, y = to_nodes((x, y))
    if x.shape != y.shape:
        raise Exception('vsub: operands must have the same shape')

    return Vsub(x, y)

def build_vmul(x, y):
    x, y = to_nodes((x, y))
    if x.shape != y.shape:
        raise Exception('vmul: operands must have the same shape')
    
    return Vmul(x, y)

def build_vdiv(x, y):
    x, y = to_nodes((x, y))
    if x.shape != y.shape:
        raise Exception('vdiv: operands must have the same shape')
    
    return Vdiv(x, y)

def build_vneg(x):
    x = to_node(x)
    return Vneg(x)

def build_vexp(x):
    x = to_node(x)
    return Vexp(x)

def build_vlog(x):
    x = to_node(x)
    return Vlog(x)

def build_vsigmoid(x):
    x = to_node(x)
    return Vsigmoid(x)

def build_vrelu(x):
    x = to_node(x)
    return Vrelu(x)

def build_vrelu_prime(x):
    x = to_node(x)
    return VreluPrime(x)

def build_vleaky_relu(x, alpha):
    x = to_node(x)
    return VleakyRelu(x, alpha)

def build_vleaky_relu_prime(x, alpha):
    x = to_node(x)
    return VleakyReluPrime(x, alpha)

def build_velu(x, alpha):
    x = to_node(x)
    return Velu(x, alpha)

def build_velu_prime(x, alpha):
    x = to_node(x)
    return VeluPrime(x, alpha)

def build_vsoftplus(x, beta):
    x = to_node(x)
    return Vsoftplus(x, beta)

def build_vtanh(x):
    x = to_node(x)
    return Vtanh(x)

def build_vsign(x):
    x = to_node(x)
    return Vsign(x)

def build_softmax(x):
    x = to_node(x)
    if len(x.shape) != 2:
        raise Exception('softmax: operand must be a matrix')
    return Softmax(x)

'''
def build_sum(x):
    x = to_node(x)
    if len(x.shape) != 1:
        raise Exception('sum: operand must be a vector')

    return Sum(x)
'''

def build_sum3(x):
    x = to_node(x)
    if len(x.shape) != 3:
        raise Exception('sum3: operand must be a 3-dim tensor')

    return Sum3(x)

def build_expand_dims(x, axis, size):
    x = to_node(x)
    size = int(size)
    if axis > len(x.shape) or size <= 0:
        raise Exception('expanddims: invalid axis or size')
    return ExpandDims(x, axis, size)

def build_dot_vv(x, y):
    x, y = to_nodes((x, y))
    if x.shape != y.shape or len(x.shape) != 1:
        raise Exception('dotvv: operands must be vectors of same size')

    return DotVv(x, y)

def build_outer(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 1 or len(y.shape) != 1:
        raise Exception('outer: operands must be vectors')

    return Outer(x, y)

def build_vsadd(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 0 or len(y.shape) != 1:
        raise Exception('vsadd: x must be a number and y a vector')

    return Vsadd(x, y)

def build_vsmul(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 0 or len(y.shape) != 1:
        raise Exception('vsmul: x must be a number and y a vector')

    return Vsmul(x, y)

def build_vsdiv(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 0 or len(y.shape) != 1:
        raise Exception('vsdiv: x must be a number and y a vector')

    return Vsdiv(x, y)

def build_dot_mv(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or len(y.shape) != 1 or x.shape[1] != y.shape[0]:
        raise Exception('dotmv: operantds must be of shape (m, n) and (n)')

    return DotMv(x, y)

def build_dot_mtv(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or len(y.shape) != 1 or x.shape[0] != y.shape[0]:
        raise Exception('dotmtv: operands must be of shape (m, n) and (n)')

    return DotMtv(x, y)

def build_dot_mm(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or len(y.shape) != 2 or x.shape[1] != y.shape[0]:
        raise Exception('dotmm: operands must be of shape (m, n) and (n, p)')

    return DotMm(x, y)

def build_dot_mtm(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or len(y.shape) != 2 or x.shape[0] != y.shape[0]:
        raise Exception('dotmtm: operands must be of shape (n, m) and (n, p)')

    return DotMtm(x, y)

def build_dot_mmt(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or len(y.shape) != 2 or x.shape[1] != y.shape[1]:
        raise Exception('dotmmt: operands must be of shape (m, n) and (p, n)')

    return DotMmt(x, y)

def build_norm1(x):
    x = to_node(x)
    if len(x.shape) != 1:
        raise Exception('norm1: operand must be a vector)')

    return Norm1(x)

def build_reshape(x, shape):
    def shape_len(s):
        res = 1
        for x in s: res *= x
        return res
    
    if -1 in shape:
        lshape = list(shape)
        i = lshape.index(-1)
        lshape[i] = 1
        cor_dim = int(x.size() / shape_len(lshape))
        lshape[i] = cor_dim
        shape = tuple(lshape)
    
    x = to_node(x)
    if shape_len(x.shape) != shape_len(shape):
        raise Exception('reshape: shape must be of same length that the operand shape')
    return Reshape(x, shape)

def build_add_bias(x, y):
    x, y = to_nodes((x, y))
    if len(y.shape) != 1 or x.shape[-1] != y.shape[0]:
        raise Exception('addbias: y must be a vector of same size than last dim of x')

    return AddBias(x, y)

def build_conv2d(X, K, sh, sw, ph, pw):
    X, K = to_nodes((X, K))
    if len(X.shape) != 4 or len(K.shape) != 4 or X.shape[1] != K.shape[1]:
        raise Exception('conv2: X and K must be valid 4D tensors')

    return Conv2D(X, K, sh, sw, ph, pw)

def build_conv2d_dx(K, dY, sh, sw, ph, pw, x_shape):
    K, dY = to_nodes((K, dY))
    if len(K.shape) != 4 or len(dY.shape) != 4:
        raise Exception('conv2dx: K and dY must be valid 4D tensors')

    return Conv2DDx(K, dY, sh, sw, ph, pw, x_shape)

def build_conv2d_dk(X, dY, sh, sw, ph, pw, k_shape):
    X, dY = to_nodes((X, dY))
    if len(X.shape) != 4 or len(dY.shape) != 4:
        raise Exception('conv2dk: X and dY must be valid 4D tensors')

    return Conv2DDk(X, dY, sh, sw, ph, pw, k_shape)

def build_conv2d_bias_add(X, b):
    X, b = to_nodes((X, b))
    if len(X.shape) != 4 or len(b.shape) != 1 or X.shape[1] != b.shape[0]:
        raise Exception('conv2_bias_add: X must be a 4D tensor and be a vector (nchans) ')

    return Conv2DBiasAdd(X, b)

def build_conv2d_bias_add_grad(dout):
    dout = to_node(dout)
    if len(dout.shape) != 4:
        raise Exception('conv2_bias_add_grad: dout must be a 4D tensor')

    return Conv2DBiasAddGrad(dout)

def build_max_pooling(X, kh, kw, sh, sw):
    X = to_node(X)
    if len(X.shape) != 4:
        raise Exception('max_pooling: X must be a 4D tensor')

    return MaxPooling(X, kh, kw, sh, sw)

def build_max_pooling_grad(Y, dout, kh, kw, sh, sw):
    Y, dout = to_nodes((Y, dout))
    if len(Y.shape) != 4 or len(dout.shape) != 4:
        raise Exception('max_pooling_grad: Y and dout must be a 4D tensors')

    return MaxPoolingGrad(Y, dout, kh, kw, sh, sw)
                      

def build_mae_loss(y_pred, y_true):
    y_pred, y_true = to_nodes((y_pred, y_true))
    if len(y_pred.shape) != 1 or y_pred.shape != y_true.shape:
        raise Exception('maeloss: operands must be vectors of same size')

    return MAELoss(y_pred, y_true)

def build_mse_loss(y_pred, y_true):
    y_pred, y_true = to_nodes((y_pred, y_true))
    if len(y_pred.shape) != 1 or y_pred.shape != y_true.shape:
        raise Exception('mseloss: operands must be vectors of same size')

    return MSELoss(y_pred, y_true)

def build_bce_loss(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 1 or x.shape != y.shape:
        raise Exception('bceloss: operands must be vectors of same size')

    return BCELoss(x, y)

def build_cross_entropy_loss(x, y):
    x, y = to_nodes((x, y))
    if len(x.shape) != 2 or x.shape != y.shape:
        raise Exception('bceloss: operands must be matrices of same size')

    return CrossEntropyLoss(x, y)


def build_node_grad(out, x):
    if len(out.shape) != 0:
        raise Exception('No grad of vector output')

    if x in out.grads:
        return out.grads[x]

    if x == out:
        res = build_val(1)
        out.grads[x] = res
        return res

    if not x.is_ancestor_of(out):
        res = build_val(np.zeros(x.shape))
        out.grads[x] = res
        return res

    res = None
    
    for y_i in list(x.succs):
        if not y_i.is_ancestor_of(out):
            continue
        
        dy_i = build_node_grad(out, y_i)
        grad_y_i = y_i.get_grad(x, dy_i)
        if res is None:
            res = grad_y_i
        else:
            res = build_vadd(res, grad_y_i)

    out.grads[x] = res
    return res
        


def prod(x):
    res = 1
    for v in x: res *= v
    return res

def op_sum(X, axis):
    shape3 = (prod(X.shape[:axis]), X.shape[axis], prod(X.shape[axis+1:]))
    final_shape = X.shape[:axis] + X.shape[axis+1:]

    t1 = build_reshape(X, shape3)
    t2 = build_sum3(t1)
    t3 = build_reshape(t2, final_shape)
    return t3


def op_vsadd(n, x):
    t1 = build_reshape(x, (prod(x.shape),))
    t2 = build_vsadd(n, t1)
    t3 = build_reshape(t2, x.shape)
    return t3

def op_vsmul(n, x):
    t1 = build_reshape(x, (prod(x.shape),))
    t2 = build_vsmul(n, t1)
    t3 = build_reshape(t2, x.shape)
    return t3

def op_conv2d_transpose(X, K, sh, sw, ph, pw):
    X, K = to_nodes((X, K))
    if len(X.shape) != 4 or len(K.shape) != 4:
        raise Exception('conv2_transpose: X and K must be valid 4D tensors')

    hy = (X.shape[2] - 1) * sh - 2 * ph + K.shape[2]
    wy = (X.shape[3] - 1) * sw - 2 * pw + K.shape[3]
    y_shape = (X.shape[0], K.shape[1], hy, wy)
    
    return build_conv2d_dx(K, X, sh, sw, ph, pw, y_shape)

def op_mse_loss(y_pred, y_true):
    y_pred = build_reshape(y_pred, (-1,))
    y_true = build_reshape(y_true, (-1,))
    return build_mse_loss(y_pred, y_true)


class Network:
    
    def __init__(self):
        self.params_ = []
        
    def __call__(self, x):
        return self.forward(x)
    
    def dense_layer(self, inp_size, out_size):
        
        W_arr = np.random.randn(inp_size, out_size)
        b_arr = np.zeros((out_size))
        
        W = build_val(W_arr)
        b = build_val(b_arr)
        
        self.params_.append(W)
        self.params_.append(b)
        
        def fun(X):
            X = build_dot_mm(X, W)
            X = build_add_bias(X, b)
            return X
        
        return fun

    def conv2d_layer(self, cx, cy, hk, wk, 
                     sh = 1, sw = 1, ph = 0, pw = 0):
        
        K_arr = np.random.randn(cy, cx, hk, wk)
        b_arr = np.random.randn(cy)
        
        K = build_val(K_arr)
        b = build_val(b_arr)
        self.params_.append(K)
        self.params_.append(b)
        
        def fun(X):
            X = build_conv2d(X, K, sh, sw, ph, pw)
            X = build_conv2d_bias_add(X, b)
            return X
        
        return fun

    def parameters(self):
        return list(self.params_)
