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


t3, p3 = load("ex3/pi_times.txt")
t6, p6 = load("ex6/pi_any_times.txt")

print("np | pi (Ex 3)  pi (Ex 6)  | median time (Ex 3)  median time (Ex 6)")
for n in t3:
    print(f"{n:<2} | {p3[n]:<10} {p6[n]:<10} | {t3[n]:<19.6f} {t6[n]:.6f}")
