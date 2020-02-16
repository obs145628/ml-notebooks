import numpy as np
import torch
from torch.utils.data import DataLoader

import datasets
from simpledataset import SimpleDataset


DS_PATH='~/.pythondata/mnist'
BATCH_SIZE = 64
IN_SIZE = 28*28
HIDDEN_SIZE = 50
OUT_SIZE = 10
LR=0.001
NEPOCHS = 10

### Prepare Data ###
X_train, y_train, X_test, y_test = datasets.load_mnist(DS_PATH)
X_train, X_test = X_train.reshape(len(X_train), -1), X_test.reshape(len(X_test), -1)
X_train, X_test = X_train / 255, X_test / 255
y_train, y_test = y_train.astype(np.long), y_test.astype(np.long)
train_dl = DataLoader(dataset=SimpleDataset(X_train, y_train), batch_size=BATCH_SIZE, shuffle=True)
test_dl = DataLoader(dataset=SimpleDataset(X_test, y_test), batch_size=BATCH_SIZE, shuffle=False)


### Prepare Network ###
class Net(torch.nn.Module):

    def __init__(self):
        super(Net, self).__init__()
        self.l1 = torch.nn.Linear(IN_SIZE , HIDDEN_SIZE)
        self.l2 = torch.nn.Linear(HIDDEN_SIZE, OUT_SIZE)

    def forward(self, x):
        x = x.view(-1, IN_SIZE)
        x = torch.relu(self.l1(x))
        y_logits = self.l2(x)
        return y_logits

net = Net()
criterion = torch.nn.CrossEntropyLoss(reduction='sum')
opti = torch.optim.SGD(net.parameters(), lr=LR)

### Train Network ###
for epoch in range(NEPOCHS):

    for batch_idx, (X, y) in enumerate(train_dl):    
        net.zero_grad()
        y_logits = net(X)
        loss = criterion(y_logits, y)
        loss.backward()
        opti.step()

    
    preds = torch.empty(len(train_dl.dataset))
    y = torch.empty(len(train_dl.dataset))
    loss = 0
    for batch_idx, (bX, by) in enumerate(train_dl): 
        y_logits = net(bX)
        bloss = criterion(y_logits, by)
        bpreds = torch.argmax(y_logits, dim=1)
        preds[batch_idx*BATCH_SIZE:batch_idx*BATCH_SIZE+len(bX)] = bpreds
        y[batch_idx*BATCH_SIZE:batch_idx*BATCH_SIZE+len(bX)] = by
        loss += bloss
        
    acc = y.eq(preds).sum().float() / len(y)
    print('Epoch {}: Loss = {}, Accuracy = {}'.format(epoch+1, 
                                                      loss.data,
                                                      acc))


### Evaluate Network ###
preds = torch.empty(len(test_dl.dataset))
y = torch.empty(len(test_dl.dataset))
loss = 0
for batch_idx, (bX, by) in enumerate(test_dl): 
    y_logits = net(bX)
    bloss = criterion(y_logits, by)
    bpreds = torch.argmax(y_logits, dim=1)
    preds[batch_idx*BATCH_SIZE:batch_idx*BATCH_SIZE+len(bX)] = bpreds
    y[batch_idx*BATCH_SIZE:batch_idx*BATCH_SIZE+len(bX)] = by
    loss += bloss

acc = y.eq(preds).sum().float() / len(y)
print('Test Accuracy = {}'.format(acc))
