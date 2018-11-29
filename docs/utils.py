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
