import math


def gradient_descent(f, x, lr=0.01, tol=1e-9, max_iter=100_000, h=1e-6):
    """Return (x, f(x)) at the local minimum reached from x, or None if it never converges."""
    for _ in range(max_iter):
        grad = (f(x + h) - f(x - h)) / (2 * h)   # numerical derivative (central difference)
        step = lr * grad
        x -= step
        if abs(step) < tol:                      # converged
            return x, f(x)
    return None                                  # no convergence: likely unbounded below


if __name__ == "__main__":
    expr = input("f(x) = ")                      # e.g. x**2 - 4*x + 3  or  sin(x) + 0.1*x**2
    x0 = float(input("Starting x = "))

    f = lambda x: eval(expr, {**vars(math), "x": x})   # math functions (sin, exp, ...) allowed
    result = gradient_descent(f, x0)
    if result:
        print(f"Local minima: x = {result[0]:.6f}, f(x) = {result[1]:.6f}")
    else:
        print("No local minima found (function may be unbounded below).")