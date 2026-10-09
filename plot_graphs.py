import re
import sys
import statistics
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


def load_median(path):
    """Read lines like 'np=4 ... time=0.0123' and keep the median time per np."""
    runs = {}
    with open(path) as f:
        for line in f:
            m = re.search(r"np=(\d+).*time=([\d.]+)", line)
            if m:
                runs.setdefault(int(m.group(1)), []).append(float(m.group(2)))
    return {n: statistics.median(t) for n, t in sorted(runs.items())}


def make_graphs(name, path):
    med = load_median(path)
    procs = list(med.keys())
    times = list(med.values())
    speedup = [med[1] / t for t in times]

    print(f"\n{name}")
    print("np   median_time   speedup")
    for n, t, s in zip(procs, times, speedup):
        print(f"{n:<4} {t:<13.6f} {s:.2f}")

    plt.figure()
    plt.plot(procs, times, marker="o")
    plt.xlabel("Number of Processors")
    plt.ylabel("Time (seconds, median of runs)")
    plt.title(f"{name}: Time vs Number of Processors")
    plt.xticks(procs)
    plt.grid(True)
    plt.savefig(f"{name.lower()}_time.png", dpi=150)
    plt.close()

    plt.figure()
    plt.plot(procs, speedup, marker="o", label="Measured speedup")
    plt.plot(procs, procs, linestyle="--", label="Ideal (linear) speedup")
    plt.xlabel("Number of Processors")
    plt.ylabel("Speedup (T1 / Tn)")
    plt.title(f"{name}: Speedup")
    plt.xticks(procs)
    plt.legend()
    plt.grid(True)
    plt.savefig(f"{name.lower()}_speedup.png", dpi=150)
    plt.close()


if len(sys.argv) != 3:
    print("Usage: python3 plot_graphs.py <sum_times.txt> <pi_times.txt>")
    sys.exit(1)

make_graphs("Sum", sys.argv[1])
make_graphs("Pi", sys.argv[2])
print("\nSaved: sum_time.png, sum_speedup.png, pi_time.png, pi_speedup.png")
