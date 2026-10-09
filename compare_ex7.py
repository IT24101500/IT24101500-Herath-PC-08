import re
import statistics


def load(path):
    times, pis = {}, {}
    with open(path) as f:
        for line in f:
            m = re.search(r"np=(\d+) pi=([\d.]+) time=([\d.]+)", line)
            if m:
                n = int(m.group(1))
                times.setdefault(n, []).append(float(m.group(3)))
                pis[n] = m.group(2)
    return {n: statistics.median(t) for n, t in sorted(times.items())}, pis


t6, p6 = load("ex6/pi_any_times.txt")
t7, p7 = load("ex7/pi_bsend_times.txt")

print("np | pi (Ex 6)  pi (Ex 7)  | median time (Ex 6)  median time (Ex 7)")
for n in t6:
    print(f"{n:<2} | {p6[n]:<10} {p7[n]:<10} | {t6[n]:<19.6f} {t7[n]:.6f}")
