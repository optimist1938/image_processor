import numpy 
import numpy as np
import scipy.linalg as linalg
data_train = numpy.loadtxt('/Users/semencinman/Desktop/ЛР/train.txt', delimiter=',')
data_test = numpy.loadtxt('/Users/semencinman/Desktop/ЛР/test.txt', delimiter=',')
X_train = data_train[:,0]
y_train = data_train[:,1]
X_test = data_test[:,0]
Y_test = data_test[:,1]
n = X_train.size
disp = np.ones((n, 1))
X_train = X_train.reshape((n, 1)) #потом пофисксить
approx_matrix = np.concatenate((X_train, disp), axis=1)
hat_a = linalg.inv(approx_matrix.T @ approx_matrix) @ approx_matrix.T @ y_train
hat_a = hat_a.reshape((hat_a.size, 1))

print(f'{hat_a[0]}x + {hat_a[1]}b')
