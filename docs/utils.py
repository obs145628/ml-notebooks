import matplotlib.pyplot as plt
import numpy as np
import os
import torch
import torchvision
import torchvision.transforms as transforms


_grads = {}
def grad_hook(x):
    def hook(grad):
        _grads[x] = grad
    return hook
    


def save_grad(x):
    x.register_hook(grad_hook(x))

def get_grad(x):
    return _grads[x]


DATA_PATH = './data'

def load_mnist(batch_size):

    if not os.path.exists(DATA_PATH): os.mkdir(DATA_PATH)
    trans = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5,), (1.0,))])

    train_set = torchvision.datasets.MNIST(root=DATA_PATH, train=True, transform=trans, download=True)
    test_set = torchvision.datasets.MNIST(root=DATA_PATH, train=False, transform=trans, download=True)

    train_loader = torch.utils.data.DataLoader(
        dataset=train_set,
        batch_size=batch_size,
        shuffle=True)

    test_loader = torch.utils.data.DataLoader(
        dataset=test_set,
        batch_size=batch_size,
        shuffle = False)

    return train_loader, test_loader


def load_mnist_01(batch_size):

    if not os.path.exists(DATA_PATH): os.mkdir(DATA_PATH)
    trans = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5,), (1.0,))])

    train_set = torchvision.datasets.MNIST(root=DATA_PATH, train=True, transform=trans, download=True)
    test_set = torchvision.datasets.MNIST(root=DATA_PATH, train=False, transform=trans, download=True)

    class DatasetMNIST01:

        def __init__(self, mnist):
            self.mnist = mnist
            self.idxs = []
            for i in range(len(mnist)):
                c = int(mnist[i][1])
                if c <= 1: self.idxs.append(i)

        def __getitem__(self, i):
            return self.mnist[self.idxs[i]]

        def __len__(self):
            return len(self.idxs)
    
    train_loader = torch.utils.data.DataLoader(
        dataset=DatasetMNIST01(train_set),
        batch_size=batch_size,
        shuffle=True)

    test_loader = torch.utils.data.DataLoader(
        dataset=DatasetMNIST01(test_set),
        batch_size=batch_size,
        shuffle = False)

    return train_loader, test_loader


def get_class_output(model, loader, act = None):

    all_y_preds = np.empty((len(loader.dataset),)).astype(np.int64)
    all_y_true = np.empty((len(loader.dataset),)).astype(np.int64)
    idx = 0
    
    for batch_idx, (X, y) in enumerate(loader):
        y_logits = model(X)
        if act is not None:
            y_logits = act(y_logits)
        
        if len(y_logits.shape) == 1:
            y_preds = torch.round(y_logits)
        else:
            y_preds = torch.argmax(y_logits, dim=1)
        
        all_y_preds[idx:idx + len(X)] = y_preds.data.numpy()
        all_y_true[idx:idx + len(X)] = y.data.numpy()
        idx += len(X)
        
    return all_y_preds, all_y_true


def vec2one_hot(x, nclasses):
    res = np.zeros((len(x), nclasses))
    res[np.arange(len(x)), x] = 1
    return res



def make_meshgrid(x, y, h=.02):
    x_min, x_max = x.min() - 1, x.max() + 1
    y_min, y_max = y.min() - 1, y.max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
    return xx, yy


def plot_contours(clf, X, xtrans = None, **params):
    x_mesh, y_mesh = make_meshgrid(X[:, 0], X[:, 1])
    X = np.c_[x_mesh.ravel(), y_mesh.ravel()]
    Xe = X if xtrans is None else xtrans(X)
    y_pred = clf.predict(Xe)
    y_pred = y_pred.reshape(x_mesh.shape)
    plt.contourf(x_mesh, y_mesh, y_pred, **params)



def plot_preds(X, y, preds):
    n = len(X)
    
    markers = ['o' if preds[i] == 1 else '^' for i in range(n)]
    
    X0 = X[y == 0]
    preds0 = preds[y == 0]
    c0 = ['black' if preds0[i] == 0 else 'black' for i in range(len(X0))]
    plt.scatter(X0[:, 0], X0[:, 1], c=c0, marker='^')
    
    X1 = X[y == 1]
    preds1 = preds[y == 1]
    c1 = ['black' if preds1[i] == 1 else 'black' for i in range(len(X1))]
    plt.scatter(X1[:, 0], X1[:, 1], c=c1, marker='o')


def plot_reg(X, y, preds):
    plt.scatter(X[:,0], y, c='b')
    
    p = (X[:,0]).argsort()
    
    plt.plot(X[:,0][p], preds[p], c='r')



def plot_reg_contours(X, y, clf, npts=5000):
    min_x = np.min(X) - 1e-5 * np.std(X)
    max_x = np.max(X) + 1e-5 * np.std(X)
    xx = np.linspace(min_x, max_x, npts)
    yy = clf.predict(xx)
    plt.plot(xx, yy, c='r')

    plt.scatter(X, y, c='b')
    
