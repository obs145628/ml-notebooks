import metrics
import numpy as np
import torch
import revdiff as rd
import unittest
import utils

def get_grad(out, x):
    return rd.build_node_grad(out, x)

def val(x):
    return rd.build_val(x)

def get_arr_len(x):
    if isinstance(x, (np.ndarray, np.generic)):
        return x.size
    else:
        return 1

def mse(y_pred, y_true):
    diff = (y_true - y_pred)
    s = rd.op_sum(diff * diff, axis=0)
    return (1 / len(y_pred.shape)) * s
    
class RDTestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.ck_feps = 1e-6

    def ck_fequals(self, a, b, feps = None):
        if feps is None:
            feps = self.ck_feps
        dist = metrics.tdist(a, b) / get_arr_len(a)
        self.assertLess(dist, feps)
        

class BasicOps(RDTestCase):
    
    def test_fdw_val(self):
        x = rd.build_val(2)
        y = rd.build_val(3)
        self.ck_fequals(x.eval(), 2)
        self.ck_fequals(y.eval(), 3)

    def test_bwd_val(self):
        x = rd.build_val(2)
        y = rd.build_val(3)
        self.ck_fequals(get_grad(x, x).eval(), 1)
        self.ck_fequals(get_grad(x, y).eval(), 0)
        self.ck_fequals(get_grad(y, y).eval(), 1)
        self.ck_fequals(get_grad(y, x).eval(), 0)

    def test_fwd_vadd(self):
        x = np.array(2)
        y = np.array(3)
        self.ck_fequals((val(x) + val(y)).eval(), x + y)

        x = np.random.randn(3, 12, 7)
        y = np.random.randn(3, 12, 7)
        self.ck_fequals((val(x) + val(y)).eval(), x + y)

    def test_bwd_vadd(self):
        x = np.array(2)
        y = np.array(3)
        tx = val(x)
        ty = val(y)
        tz = tx + ty
        tlone = val(10)
        self.ck_fequals(get_grad(tz, tx).eval(), 1)
        self.ck_fequals(get_grad(tz, ty).eval(), 1)
        self.ck_fequals(get_grad(tz, tlone).eval(), 0)

    def test_fwd_vsub(self):
        x = np.array(2)
        y = np.array(3)
        self.ck_fequals((val(x) - val(y)).eval(), x - y)

        x = np.random.randn(3, 12, 7)
        y = np.random.randn(3, 12, 7)
        self.ck_fequals((val(x) - val(y)).eval(), x - y)

    def test_bwd_vsub(self):
        x = np.array(2)
        y = np.array(3)
        tx = val(x)
        ty = val(y)
        tz = tx - ty
        tlone = val(10)
        self.ck_fequals(get_grad(tz, tx).eval(), 1)
        self.ck_fequals(get_grad(tz, ty).eval(), -1)
        self.ck_fequals(get_grad(tz, tlone).eval(), 0)

    def test_fwd_vmul(self):
        x = np.array(2)
        y = np.array(3)
        self.ck_fequals((val(x) * val(y)).eval(), x * y)

        x = np.random.randn(3, 12, 7)
        y = np.random.randn(3, 12, 7)
        self.ck_fequals((val(x) * val(y)).eval(), x * y)

    def test_bwd_vmul(self):
        x = np.array(2)
        y = np.array(3)
        tx = val(x)
        ty = val(y)
        tz = tx * ty
        tlone = val(10)
        self.ck_fequals(get_grad(tz, tx).eval(), y)
        self.ck_fequals(get_grad(tz, ty).eval(), x)
        self.ck_fequals(get_grad(tz, tlone).eval(), 0)

    def test_fwd_vdiv(self):
        x = np.array(2)
        y = np.array(3)
        self.ck_fequals((val(x) / val(y)).eval(), x / y)

        x = np.random.randn(3, 12, 7)
        y = np.random.randn(3, 12, 7)
        self.ck_fequals((val(x) / val(y)).eval(), x / y, feps=1e-4)

    def test_bwd_vdiv(self):
        x = np.array(2)
        y = np.array(3)
        tx = val(x)
        ty = val(y)
        tz = tx / ty
        tlone = val(10)
        self.ck_fequals(get_grad(tz, tx).eval(), 1 / y)
        self.ck_fequals(get_grad(tz, ty).eval(), - x/(y**2))
        self.ck_fequals(get_grad(tz, tlone).eval(), 0)

    def test_fwd_dotvv(self):
        x = np.random.randn(7)
        y = np.random.randn(7)
        tx = val(x)
        ty = val(y)
        tz = rd.build_dot_vv(tx, ty)
        self.ck_fequals(tz.eval(), x @ y)

    def test_bwd_dotvv(self):
        x = np.random.randn(7)
        y = np.random.randn(7)
        tx = val(x)
        ty = val(y)
        tlone = val(10)
        tz = rd.build_dot_vv(tx, ty)
        self.ck_fequals(get_grad(tz, tx).eval(), y)
        self.ck_fequals(get_grad(tz, ty).eval(), x)
        self.ck_fequals(get_grad(tz, tlone).eval(), 0)

    def test_bwd_vsadd(self):

        x = np.random.randn()
        y = np.random.randn(12).astype(np.float32)

        tx = val(x)
        ty = val(y)
        tz = rd.build_vsadd(tx, ty)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dy = torch.tensor(y, requires_grad=True)
        dz = dx + dy
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, ty).eval(), dy.grad.data.numpy())

    def test_bwd_vsmul(self):

        x = np.random.randn()
        y = np.random.randn(12).astype(np.float32)

        tx = val(x)
        ty = val(y)
        tz = rd.build_vsmul(tx, ty)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dy = torch.tensor(y, requires_grad=True)
        dz = dx * dy
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, ty).eval(), dy.grad.data.numpy())

    def test_bwd_vsdiv(self):

        x = np.random.randn()
        y = np.random.rand(12).astype(np.float32) + 0.1

        tx = val(x)
        ty = val(y)
        tz = rd.build_vsdiv(tx, ty)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dy = torch.tensor(y, requires_grad=True)
        dz = dx / dy
        de = torch.dot(dz, dz)
        de.backward()

        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(te, ty).eval(), dy.grad.data.numpy())

    def test_bwd_vlog(self):

        x = np.random.rand(12).astype(np.float32) + 0.1

        tx = val(x)
        tz = rd.build_vlog(tx)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.log(dx)
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

class LinReg(RDTestCase):


    def test_mse(self):

        y_pred = np.random.randn(46)
        y_true = np.random.randn(46)

        dy_pred = val(y_pred)
        dy_true = val(y_true)
        dloss = mse(dy_pred, dy_true)
        
        ty_pred = torch.tensor(y_pred, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=True)
        criterion = torch.nn.MSELoss()
        tloss = criterion(ty_pred, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(dloss, dy_pred).eval(), ty_pred.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dy_true).eval(), ty_true.grad.data.numpy())


    def test_sgd_mse(self):
        X = np.random.randn(46, 7)
        w = np.random.randn(7)
        y_true = np.random.randn(46)

        dX = val(X)
        dw = val(w)
        dy_true = val(y_true)
        dy_pred = rd.build_dot_mv(dX, dw)
        dloss = mse(dy_pred, dy_true)

        tX = torch.tensor(X, requires_grad=True)
        tw = torch.tensor(w, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=True)
        ty_pred = torch.matmul(tX, tw)
        utils.save_grad(ty_pred)
        criterion = torch.nn.MSELoss()
        tloss = criterion(ty_pred, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-3)
        self.ck_fequals(get_grad(dloss, dy_pred).eval(),
                        utils.get_grad(ty_pred).data.numpy())
        self.ck_fequals(get_grad(dloss, dy_true).eval(), ty_true.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dw).eval(), tw.grad.data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy())

    def test_sgd_logreg_2(self):
        X = np.random.randn(46, 7).astype(np.float32)
        w = np.random.randn(7).astype(np.float32)
        y_true = np.random.randint(0, 2, (46)).astype(np.float32)

        dX = val(X)
        dw = val(w)
        dy_true = val(y_true)
        dy_out = rd.build_dot_mv(dX, dw)
        dy_pred = rd.build_vsigmoid(dy_out)
        dloss = rd.build_bce_loss(dy_out, dy_true)

        tX = torch.tensor(X, requires_grad=True)
        tw = torch.tensor(w, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=False)
        ty_out = torch.matmul(tX, tw)
        utils.save_grad(ty_out)
        ty_pred = torch.sigmoid(ty_out)
        criterion = torch.nn.BCEWithLogitsLoss(reduction='sum')
        tloss = criterion(ty_out, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-3)
        self.ck_fequals(dy_pred.eval(), ty_pred.data.numpy())
        self.ck_fequals(get_grad(dloss, dy_out).eval(),
                        utils.get_grad(ty_out).data.numpy())
        self.ck_fequals(get_grad(dloss, dw).eval(), tw.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy())

    def test_sgd_logreg_2_prim(self):
        X = np.random.randn(46, 7).astype(np.float32)
        w = np.random.randn(7).astype(np.float32)
        y_true = np.random.randint(0, 2, (46)).astype(np.float32)

        dX = val(X)
        dw = val(w)
        dy_true = val(y_true)
        dy_out = rd.build_dot_mv(dX, dw)
        dy_pred = rd.build_vsdiv(1, rd.build_vsadd(1, rd.build_vexp((-dy_out))))
        dloss = - rd.op_sum(dy_true * rd.build_vlog(dy_pred)
                               + (rd.build_vsadd(1, -dy_true))
                               * rd.build_vlog(rd.build_vsadd(1, -dy_pred)), axis=0) 

        tX = torch.tensor(X, requires_grad=True)
        tw = torch.tensor(w, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=False)
        ty_out = torch.matmul(tX, tw)
        utils.save_grad(ty_out)
        ty_pred = torch.sigmoid(ty_out)
        criterion = torch.nn.BCEWithLogitsLoss(reduction='sum')
        tloss = criterion(ty_out, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-2)
        self.ck_fequals(dy_pred.eval(), ty_pred.data.numpy())
        self.ck_fequals(get_grad(dloss, dy_out).eval(),
                        utils.get_grad(ty_out).data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(dloss, dw).eval(), tw.grad.data.numpy(), feps=1e-3)
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy(), feps=1e-4)



    def test_sgd_logreg_k(self):
        X = np.random.randn(46, 7).astype(np.float32)
        w = np.random.randn(7, 4).astype(np.float32)
        y_true = np.zeros((46, 4)).astype(np.float32)
        for i in range(y_true.shape[0]):
            y_true[i][np.random.randint(0, y_true.shape[1])] = 1

        dX = val(X)
        dw = val(w)
        dy_true = val(y_true)
        dy_out = rd.build_dot_mm(dX, dw)
        dy_pred = rd.build_softmax(dy_out)
        dloss = rd.build_cross_entropy_loss(dy_out, dy_true)

        tX = torch.tensor(X, requires_grad=True)
        tw = torch.tensor(w, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=False)
        ty_true = torch.argmax(ty_true, dim=1)
        ty_out = torch.matmul(tX, tw)
        ty_pred = torch.nn.functional.softmax(ty_out, dim=1)
        utils.save_grad(ty_out)
        criterion = torch.nn.CrossEntropyLoss(reduction='sum')
        tloss = criterion(ty_out, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-3)
        self.ck_fequals(dy_pred.eval(), ty_pred.data.numpy())
        self.ck_fequals(get_grad(dloss, dy_out).eval(),
                        utils.get_grad(ty_out).data.numpy())
        self.ck_fequals(get_grad(dloss, dw).eval(), tw.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy())


    def test_sgd_logreg_k_l1_l2(self):
        X = np.random.randn(46, 7).astype(np.float32)
        w = np.random.randn(7, 4).astype(np.float32)
        y_true = np.zeros((46, 4)).astype(np.float32)
        for i in range(y_true.shape[0]):
            y_true[i][np.random.randint(0, y_true.shape[1])] = 1

        alpha_l1 = 0.53
        alpha_l2 = 0.82

        dX = val(X)
        dw = val(w)
        dw_flat = rd.build_reshape(dw, (dw.shape[0] * dw.shape[1],))
        dy_true = val(y_true)
        dy_out = rd.build_dot_mm(dX, dw)
        dy_pred = rd.build_softmax(dy_out)
        dloss = rd.build_cross_entropy_loss(dy_out, dy_true)
        dloss = dloss + alpha_l1 * rd.build_norm1(dw_flat)
        dloss = dloss + alpha_l2 * rd.build_dot_vv(dw_flat, dw_flat)

        tX = torch.tensor(X, requires_grad=True)
        tw = torch.tensor(w, requires_grad=True)
        tw_flat = tw.view(-1)
        ty_true = torch.tensor(y_true, requires_grad=False)
        ty_true = torch.argmax(ty_true, dim=1)
        ty_out = torch.matmul(tX, tw)
        ty_pred = torch.nn.functional.softmax(ty_out, dim=1)
        utils.save_grad(ty_out)
        criterion = torch.nn.CrossEntropyLoss(reduction='sum')
        tloss = criterion(ty_out, ty_true)
        tloss = tloss + alpha_l1 * torch.norm(tw_flat, p=1) + alpha_l2 * torch.dot(tw_flat, tw_flat)
        
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-3)
        self.ck_fequals(dy_pred.eval(), ty_pred.data.numpy())
        self.ck_fequals(get_grad(dloss, dy_out).eval(),
                        utils.get_grad(ty_out).data.numpy())
        self.ck_fequals(get_grad(dloss, dw).eval(), tw.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy())


class MLP(RDTestCase):


    def test_layer_lin1(self):

        X = np.random.randn(46, 7)
        y_true = np.random.randn(46, 3)
        W = np.random.randn(7, 3)
        b = np.random.randn(3)    

        dX = val(X)
        dy_true = val(y_true)
        dW = val(W)
        db = val(b)
        dy_pred = rd.build_add_bias(rd.build_dot_mm(dX, dW), db)
        dloss = mse(rd.build_reshape(dy_pred, (y_true.size,)),
                    rd.build_reshape(dy_true, (y_true.size,)))

        
        tX = torch.tensor(X, requires_grad=True)
        ty_true = torch.tensor(y_true, requires_grad=True)
        tW = torch.tensor(W, requires_grad=True)
        tb = torch.tensor(b, requires_grad=True)
        ty_pred = torch.matmul(tX, tW) + tb
        criterion = torch.nn.MSELoss()
        tloss = criterion(ty_pred, ty_true)
        tloss.backward()

        self.ck_fequals(dloss.eval(), tloss.data.numpy(), feps=1e-3)
        self.ck_fequals(get_grad(dloss, dy_true).eval(), ty_true.grad.data.numpy())
        self.ck_fequals(get_grad(dloss, dW).eval(), tW.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(dloss, db).eval(), tb.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(dloss, dX).eval(), tX.grad.data.numpy())

    def test_act_relu(self):

        x = np.random.randn(43)
        
        tx = val(x)
        tz = rd.build_vrelu(tx)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.relu(dx)
        de = torch.dot(dz, dz)
        de.backward()


        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

    def test_act_softmax(self):

        x = np.random.randn(11, 7) 
        
        tx = val(x)
        tz = rd.build_softmax(tx)
        tz = rd.build_reshape(tz, (11 * 7,))
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.relu(dx).view(-1)
        de = torch.dot(dz, dz)
        de.backward()


        self.ck_fequals(tz.eval(), dz.data.numpy(), feps=1e-1)
        #self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

    def test_act_softplus(self):

        x = np.random.randn(23)
        
        tx = val(x)
        tz = rd.build_vsoftplus(tx, 0.7)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.nn.functional.softplus(dx, 0.7)
        de = torch.dot(dz, dz)
        de.backward()
        
        
        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

    def test_act_tanh(self):

        x = np.random.randn(23)
        
        tx = val(x)
        tz = rd.build_vtanh(tx)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.tanh(dx)
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

    def test_act_sigmoid(self):

        x = np.random.randn(23)
        
        tx = val(x)
        tz = rd.build_vsigmoid(tx)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.sigmoid(dx)
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())


    def test_act_leaky_relu(self):

        x = np.random.randn(43) * 4
        
        tx = val(x)
        tz = rd.build_vleaky_relu(tx, 0.05)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.nn.functional.leaky_relu(dx, 0.05)
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())

    def test_act_elu(self):

        x = np.random.randn(43) * 4
        
        tx = val(x)
        tz = rd.build_velu(tx, 0.05)
        te = rd.build_dot_vv(tz, tz)

        dx = torch.tensor(x, requires_grad=True)
        dz = torch.nn.functional.elu(dx, 0.05)
        de = torch.dot(dz, dz)
        de.backward()
        
        self.ck_fequals(tz.eval(), dz.data.numpy())
        self.ck_fequals(get_grad(te, tx).eval(), dx.grad.data.numpy())
        
    def test_mae_loss(self):

        y_true = np.random.randn(17)
        y_pred = np.random.randn(17) 
        
        ty_true = val(y_true)
        ty_pred = val(y_pred)
        te = rd.build_mae_loss(ty_pred, ty_true)

        dy_true = torch.tensor(y_true, requires_grad=True)
        dy_pred = torch.tensor(y_pred, requires_grad=True)
        criterion = torch.nn.L1Loss(reduction='elementwise_mean')
        de = criterion(dy_pred, dy_true)
        de.backward()

        self.ck_fequals(te.eval(), de.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, ty_pred).eval(), dy_pred.grad.data.numpy())
        self.ck_fequals(get_grad(te, ty_true).eval(), dy_true.grad.data.numpy())

    def test_mse_loss(self):
        y_true = np.random.randn(17)
        y_pred = np.random.randn(17) 
        
        ty_true = val(y_true)
        ty_pred = val(y_pred)
        te = rd.build_mse_loss(ty_pred, ty_true)

        dy_true = torch.tensor(y_true, requires_grad=True)
        dy_pred = torch.tensor(y_pred, requires_grad=True)
        criterion = torch.nn.MSELoss(reduction='elementwise_mean')
        de = criterion(dy_pred, dy_true)
        de.backward()

        self.ck_fequals(te.eval(), de.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, ty_pred).eval(), dy_pred.grad.data.numpy())
        self.ck_fequals(get_grad(te, ty_true).eval(), dy_true.grad.data.numpy())



class ConvNet(RDTestCase):


    def test_conv2d(self):

        X = np.random.randn(2, 3, 17, 23).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)
        b = np.random.randn(4).astype(np.float32) 

        tX = val(X)
        tK = val(K)
        tb = val(b)
        tY = rd.build_conv2d(tX, tK, 1, 1, 0, 0)
        tY = rd.build_conv2d_bias_add(tY, tb)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        db = torch.tensor(b, requires_grad=True)
        dY = torch.nn.functional.conv2d(dX, dK, bias=db, stride=(1, 1))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tb).eval(), db.grad.data.numpy(), feps=1e-3)

        
        

        X = np.random.randn(2, 3, 17, 24).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.build_conv2d(tX, tK, 3, 4, 0, 0)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv2d(dX, dK, stride=(3, 4))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())


        
        
        X = np.random.randn(2, 3, 17, 24).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.build_conv2d(tX, tK, 3, 4, 6, 8)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv2d(dX, dK, stride=(3, 4), padding=(6, 8))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())

        

        X = np.random.randn(2, 3, 16, 26).astype(np.float32)
        K = np.random.randn(4, 3, 6, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.build_conv2d(tX, tK, 3, 4, 7, 11)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv2d(dX, dK, stride=(3, 4), padding=(7, 11))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())



    def test_conv2d_transpose(self):

        X = np.random.randn(2, 4, 13, 16).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)
        b = np.random.randn(3).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tb = val(b)
        tY = rd.op_conv2d_transpose(tX, tK, 1, 1, 0, 0)
        tY = rd.build_conv2d_bias_add(tY, tb)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        db = torch.tensor(b, requires_grad=True)
        dY = torch.nn.functional.conv_transpose2d(dX, dK, bias=db, stride=(1, 1))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-4)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tb).eval(), db.grad.data.numpy(), feps=1e-3)

        X = np.random.randn(2, 4, 5, 5).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.op_conv2d_transpose(tX, tK, 3, 4, 0, 0)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv_transpose2d(dX, dK, stride=(3, 4))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy(), feps=1e-5)



        X = np.random.randn(2, 4, 5, 5).astype(np.float32)
        K = np.random.randn(4, 3, 5, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.op_conv2d_transpose(tX, tK, 3, 4, 6, 8)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv_transpose2d(dX, dK, stride=(3, 4), padding=(6, 8))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy(), feps=1e-5)




        X = np.random.randn(2, 4, 9, 11).astype(np.float32)
        K = np.random.randn(4, 3, 6, 8).astype(np.float32)

        tX = val(X)
        tK = val(K)
        tY = rd.op_conv2d_transpose(tX, tK, 3, 4, 7, 11)
        tYf = rd.build_reshape(tY, ((-1,)))
        te = rd.build_dot_vv(tYf, tYf)
        
        dX = torch.tensor(X, requires_grad=True)
        dK = torch.tensor(K, requires_grad=True)
        dY = torch.nn.functional.conv_transpose2d(dX, dK, stride=(3, 4), padding=(7, 11))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()
        
        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tK).eval(), dK.grad.data.numpy(), feps=1e-5)
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy(), feps=1e-5)

        
    def test_maxpooling(self):

        X = np.random.randn(1, 1, 6, 6)

        tX = val(X)
        tY = rd.build_max_pooling(tX, 2, 2, 2, 2)
        tYf = rd.build_reshape(tY, (-1,))
        te = rd.build_dot_vv(tYf, tYf)

        dX = torch.tensor(X, requires_grad=True)
        dY = torch.nn.functional.max_pool2d(dX, (2, 2), (2, 2))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()

        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())




        X = np.random.randn(1, 1, 5, 5)

        tX = val(X)
        tY = rd.build_max_pooling(tX, 2, 2, 1, 1)
        tYf = rd.build_reshape(tY, (-1,))
        te = rd.build_dot_vv(tYf, tYf)

        dX = torch.tensor(X, requires_grad=True)
        dY = torch.nn.functional.max_pool2d(dX, (2, 2), (1, 1))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()

        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())


        

        X = np.random.randn(2, 3, 9, 7)

        tX = val(X)
        tY = rd.build_max_pooling(tX, 3, 2, 2, 1)
        tYf = rd.build_reshape(tY, (-1,))
        te = rd.build_dot_vv(tYf, tYf)

        dX = torch.tensor(X, requires_grad=True)
        dY = torch.nn.functional.max_pool2d(dX, (3, 2), (2, 1))
        dYf = dY.view(-1)
        de = torch.dot(dYf, dYf)
        de.backward()

        self.ck_fequals(tY.eval(), dY.data.numpy())
        self.ck_fequals(get_grad(te, tX).eval(), dX.grad.data.numpy())
        

class ModelsBack(RDTestCase):


    def test_dense1(self):

        N = 17
        IN_SIZE = 23
        HIDDEN1_SIZE = 16
        HIDDEN2_SIZE = 9
        OUT_SIZE = 4
        LR = 0.001

        class TNet(torch.nn.Module):

            def __init__(self):
                super(TNet, self).__init__()
                self.l1 = torch.nn.Linear(IN_SIZE , HIDDEN1_SIZE)
                self.l2 = torch.nn.Linear(HIDDEN1_SIZE, HIDDEN2_SIZE)
                self.l3 = torch.nn.Linear(HIDDEN2_SIZE, OUT_SIZE)

            def forward(self, x):
                x = x.view(-1, IN_SIZE)
                x = torch.relu(self.l1(x))
                x = torch.relu(self.l2(x))
                y_logits = self.l3(x)
                return y_logits

        tnet = TNet()
        criterion = torch.nn.CrossEntropyLoss(reduction='sum')

        class DNet(rd.Network):
    
            def __init__(self):
                super().__init__()
                self.l1 = self.dense_layer(IN_SIZE , HIDDEN1_SIZE)
                self.l2 = self.dense_layer(HIDDEN1_SIZE, HIDDEN2_SIZE)
                self.l3 = self.dense_layer(HIDDEN2_SIZE, OUT_SIZE)
        
            def forward(self, x):
                x = rd.build_reshape(x, (-1, IN_SIZE))
                x = rd.build_vrelu(self.l1(x))
                x = rd.build_vrelu(self.l2(x))
                y_logits = self.l3(x)
                return y_logits
        
        dnet = DNet()
        
        X_sample = np.random.randn(N, IN_SIZE).astype(np.float32)
        y_sample = np.random.randint(0, OUT_SIZE, size=N)
    
        tparams = list(tnet.parameters())
        for i in range(len(tparams)):
            dnet.params_[i].update(tparams[i].data.numpy().T)
    
        tX = torch.tensor(X_sample)
        ty = torch.tensor(y_sample)
        ty_logits = tnet(tX)
        tloss = criterion(ty_logits, ty)
        tnet.zero_grad()
        tloss.backward()

        dX = rd.build_val(X_sample)
        dy = rd.build_val(utils.vec2one_hot(y_sample, OUT_SIZE))
        dy_logits = dnet(dX)
        dloss = rd.build_cross_entropy_loss(dy_logits, dy)


        self.ck_fequals(ty_logits.data.numpy(), dy_logits.eval())
        self.ck_fequals(tloss.data.numpy(), dloss.eval(), feps=1e-4)


        tparams = list(tnet.parameters())
        for i in range(len(tparams)):
            grad = rd.build_node_grad(dloss, dnet.params_[i]).eval()
            grad_sol = tparams[i].grad.data.numpy().T
            self.ck_fequals(grad, grad_sol)


    def test_conv1(self):

        F = torch.nn.functional

        class TNet(torch.nn.Module):

            def __init__(self):
                super(TNet, self).__init__()
                self.conv1 = torch.nn.Conv2d(1, 6, 5)
                self.conv2 = torch.nn.Conv2d(6, 16, 5)
                self.fc1 = torch.nn.Linear(16 * 5 * 5, 4)

            def forward(self, x):
                x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
                x = F.max_pool2d(F.relu(self.conv2(x)), (2, 2))
                x = x.view(x.shape[0], -1)
                x = self.fc1(x)
                return x


        tnet = TNet()
        criterion = torch.nn.MSELoss(reduction='sum')


        class DNet(rd.Network):

            def __init__(self):
                super().__init__()
                self.conv1 = self.conv2d_layer(1, 6, 5, 5)
                self.conv2 = self.conv2d_layer(6, 16, 5, 5)
                self.fc = self.dense_layer(16 * 5 * 5, 4)

            def forward(self, x):
                x = rd.build_vrelu(self.conv1(x))
                x = rd.build_max_pooling(x, 2, 2, 2, 2)
        
                x = rd.build_vrelu(self.conv2(x))
                x = rd.build_max_pooling(x, 2, 2, 2, 2)
        
                x = rd.build_reshape(x, (x.shape[0], -1))
                x = self.fc(x)
                return x


        dnet = DNet()

        X = np.random.randn(3, 1, 32, 32).astype(np.float32)
        y = np.random.randn(3, 4).astype(np.float32)
    
        tparams = list(tnet.parameters())
        for i in range(len(tparams)):
            if len(tparams[i].shape) == 2:
                dnet.params_[i].update(tparams[i].data.numpy().T)
            else:
                dnet.params_[i].update(tparams[i].data.numpy())
    
        tX = torch.tensor(X)
        ty = torch.tensor(y)
        ty_logits = tnet(tX)
        tloss = criterion(ty_logits, ty)
        tnet.zero_grad()
        tloss.backward()

        dX = rd.build_val(X)
        dy = rd.build_val(y)
        dy_logits = dnet(dX)
        dloss = rd.op_mse_loss(dy_logits, dy)


        self.ck_fequals(ty_logits.data.numpy(), dy_logits.eval())
        self.ck_fequals(tloss.data.numpy(), dloss.eval(), feps=1e-5)


        tparams = list(tnet.parameters())
        for i in range(len(tparams)):
            grad = rd.build_node_grad(dloss, dnet.params_[i]).eval()
    
            if len(tparams[i].shape) == 2:
                grad_sol = tparams[i].grad.data.numpy().T
            else:
                grad_sol = tparams[i].grad.data.numpy()
            self.ck_fequals(grad, grad_sol)
        
            
if __name__ == '__main__':
    unittest.main()
