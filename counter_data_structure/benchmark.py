import subprocess
import re
import matplotlib.pyplot as plt

threads = [1, 2, 4, 8]

def run(binary, n):
    out = subprocess.run([binary, str(n)], capture_output=True, text=True).stdout
    return float(re.search(r"time:\s*([\d.]+)", out).group(1))

precise = [run("./lock_counter", n) for n in threads]
approx = [run("./approx_counter", n) for n in threads]

plt.plot(threads, precise, marker="o", label="Precise")
plt.plot(threads, approx, marker="s", label="Approximate")
plt.xlabel("Threads")
plt.ylabel("Time (s)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig("benchmark.png")
plt.show()

for n, p, a in zip(threads, precise, approx):
    print(f"n={n}  precise={p:.4f}s  approx={a:.4f}s")
    