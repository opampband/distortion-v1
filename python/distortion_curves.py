import matplotlib.pyplot as plt
import math
import numpy as np

DISTORTION_FUNCTION_FIGURE = 1
APPLIED_FUNCTION_FIGURE = 2


def distortion_function_1(x):
    # assumes -1 <= x <= 1
    if x < -0.08905:
        return -(3 / 4) * (1 - (1 - (-x - 0.032847))**12 + (1 / 3) *
                           (-x - 0.032847)) + 0.01
    elif x < 0.320018:
        return -6.153 * x ** 2 + 3.9375 * x
    else:
        return 0.630035


def sigmoid_distortion_function(x, gain):
    return 0.6 * (gain * x) / math.sqrt(1 + (gain * x) ** 2)


def plot_distortion_function(function):
    x = np.arange(-1, 1, 1 / 1024)

    # Plot normal sine wave
    vfunc = np.vectorize(function)
    y = vfunc(x)
    plt.figure(DISTORTION_FUNCTION_FIGURE)
    plt.plot(y)


def plot_applied_function(function):
    x = np.arange(-4, 4, 1 / 1024)

    # Plot normal sine wave
    y = np.sin(x)
    plt.figure(APPLIED_FUNCTION_FIGURE)
    plt.plot(y)

    # Plot distorted sine wave on same plot
    vfunc = np.vectorize(lambda t: function(np.sin(t)))
    y = vfunc(x)
    plt.plot(y)


def main():
    plot_distortion_function(lambda t: sigmoid_distortion_function(t, 2))
    plot_applied_function(lambda t: sigmoid_distortion_function(t, 2))
    plt.show()


if __name__ == "__main__":
    main()
