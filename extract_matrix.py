import numpy as np

def read_tsp_coords(filename):
    with open(filename) as f:
        lines = f.readlines()
    n = 0
    coords = []
    in_coords = False
    for line in lines:
        line = line.strip()
        if line.startswith("DIMENSION"):
            n = int(line.split(":")[1])
        elif line.startswith("NODE_COORD_SECTION"):
            in_coords = True
            continue
        elif line.startswith("EOF"):
            break
        elif in_coords:
            parts = line.split()
            if len(parts) >= 3:
                coords.append((int(parts[1]), int(parts[2])))
    return n, coords

def compute_matrix(coords):
    n = len(coords)
    mat = np.zeros((n, n), dtype=int)
    for i in range(n):
        for j in range(n):
            if i == j:
                mat[i][j] = 0
            else:
                mat[i][j] = int(np.linalg.norm(np.array(coords[i]) - np.array(coords[j])))
    return mat

n, coords = read_tsp_coords("a280.tsp")
mat = compute_matrix(coords)

with open("input_a280.txt", "w") as f:
    f.write(f"{n}\n")
    for i in range(n):
        f.write(" ".join(str(int(round(mat[i][j]))) for j in range(n)) + "\n")