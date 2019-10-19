import matplotlib.pyplot as plt
import numpy as np


def distortion_function_1(x):
    # assumes -1 <= x <= 1
    if x < -0.08905:
        return -(3 / 4) * (1 - (1 - (-x - 0.032847))**12 + (1 / 3) *
                           (-x - 0.032847)) + 0.01
    elif x < 0.320018:
        return -6.153 * x ** 2 + 3.9375 * x
    else:
        return 0.630035


def plot_function(function):
    x = np.arange(-4, 4, 1 / 1024)
    vfunc = np.vectorize(function)
    y = vfunc(x)
    print(y)
    plt.figure(1)
    plt.plot(y)


def main():
    plot_function(np.sin)
    plot_function(lambda t: distortion_function_1(np.sin(t)))
    plt.show()


if __name__ == "__main__":
    main()
