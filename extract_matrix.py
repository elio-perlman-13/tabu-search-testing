import numpy as np
import os
import math

def euclidean(x1, y1, x2, y2):
    return int(round(math.hypot(x1 - x2, y1 - y2)))

def att(x1, y1, x2, y2):
    xd = x1 - x2
    yd = y1 - y2
    rij = math.sqrt((xd * xd + yd * yd) / 10.0)
    tij = int(round(rij))
    if tij < rij:
        dij = tij + 1
    else:
        dij = tij
    return dij

def geo_to_radians(coord):
    deg = int(coord)
    min = coord - deg
    return math.pi * (deg + 5.0 * min / 3.0) / 180.0

def geo(lat1, lon1, lat2, lon2):
    RRR = 6378.388
    lat1 = geo_to_radians(lat1)
    lon1 = geo_to_radians(lon1)
    lat2 = geo_to_radians(lat2)
    lon2 = geo_to_radians(lon2)
    q1 = math.cos(lon1 - lon2)
    q2 = math.cos(lat1 - lat2)
    q3 = math.cos(lat1 + lat2)
    dij = int(RRR * math.acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0)
    return dij

def read_tsplib(filename):
    with open(filename) as f:
        lines = f.readlines()
    lines = iter(lines)
    n = None
    ew_type = None
    ew_format = None
    coords = []
    matrix = []
    in_coords = False
    in_matrix = False
    for line in lines:
        line = line.strip()
        if line.startswith("DIMENSION"):
            n = int(line.split(":")[1].strip())
        elif line.startswith("EDGE_WEIGHT_TYPE"):
            ew_type = line.split(":")[1].strip()
        elif line.startswith("EDGE_WEIGHT_FORMAT"):
            ew_format = line.split(":")[1].strip()
        elif line.startswith("NODE_COORD_SECTION"):
            in_coords = True
            continue
        elif line.startswith("EDGE_WEIGHT_SECTION"):
            in_matrix = True
            continue
        elif line.startswith("EOF"):
            break
        elif in_coords:
            parts = line.split()
            if len(parts) >= 3:
                coords.append(tuple(map(float, parts[1:3])))
            if len(coords) == n:
                in_coords = False
        elif in_matrix:
            if ew_format == "FULL_MATRIX":
                row = []
                while len(row) < n:
                    row.extend([int(x) for x in line.split()])
                    if len(row) < n:
                        line = next(lines).strip()
                matrix.append(row)
                if len(matrix) == n:
                    in_matrix = False
            elif ew_format == "UPPER_ROW":
                # Flatten all numbers into a list
                numbers = []
                expected = n * (n - 1) // 2  # Only upper triangle, no diagonal
                while len(numbers) < expected:
                    parts = line.split()
                    if all(p.replace('-', '').isdigit() for p in parts):
                        numbers.extend([int(x) for x in parts])
                    if len(numbers) < expected:
                        line = next(lines).strip()
                # Reconstruct matrix
                matrix = [[0]*n for _ in range(n)]
                idx = 0
                for i in range(n):
                    for j in range(i+1, n):
                        matrix[i][j] = numbers[idx]
                        matrix[j][i] = numbers[idx]
                        idx += 1
                in_matrix = False
    # If coordinates, compute matrix
    if coords:
        matrix = [[0]*n for _ in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j:
                    matrix[i][j] = 0
                else:
                    if ew_type == "EUC_2D":
                        matrix[i][j] = euclidean(*coords[i], *coords[j])
                    elif ew_type == "ATT":
                        matrix[i][j] = att(*coords[i], *coords[j])
                    elif ew_type == "GEO":
                        matrix[i][j] = geo(*coords[i], *coords[j])
                    else:
                        raise ValueError(f"Unknown EDGE_WEIGHT_TYPE: {ew_type}")
    return n, matrix

input_folder = "tsp_data_folder"
output_folder = "txt_folder"
os.makedirs(output_folder, exist_ok=True)

for filename in os.listdir(input_folder):
    if filename.endswith(".tsp"):
        tsp_path = os.path.join(input_folder, filename)
        n, mat = read_tsplib(tsp_path)
        txt_filename = os.path.splitext(filename)[0] + ".txt"
        txt_path = os.path.join(output_folder, txt_filename)
        with open(txt_path, "w") as f:
            f.write(f"{n}\n")
            for i in range(n):
                f.write(" ".join(str(mat[i][j]) for j in range(n)) + "\n")