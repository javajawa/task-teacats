import sys


buffer: list[tuple[str, int, str, str]] = []

with open(sys.argv[1]) as f:
    for line in f:
        line = line.rstrip("\n\r")

        if line == "THIS SHOULD ALWAYS APPEAR":
            continue

        if line.startswith("20"):
            mode = 1
            date, host, ident, data = line.split(" ", 3)
        else:
            mode = 0
            date = line[0:15]
            host, ident, data = line[16:].split(" ", 2)

        ident = ident.strip(":")
        if "[" in ident:
            ident, _ = ident.split("[", 1)

        if ident == "named":
            data = data[25:]

        buffer.append((ident, mode, data, line))

with open(sys.argv[1] + ".out", "w") as f:
    f.write("THIS SHOULD ALWAYS APPEAR\n")
    for _, _, _, line in sorted(buffer):
        f.write(line)
        f.write("\n")
