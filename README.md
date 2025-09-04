# Project Title

Practicing tabu search on TSP

## File Structure

- `csp_tabu.cpp` — Tabu Search for TSP
- `extract_matrix.py` — Extracts distance matrices from .tsp files
- `input.txt` — Example input file for a single TSP instance
- `txt_folder/` — Folder containing converted TSP instance files in .txt format and distance matrix
- `tsp_data_folder/` — Folder containing original .tsp files in coordinates form
- `tabu_results.txt` — Aggregated results from Tabu Search runs

## Method
Neighbor selection: randomized, 2-opt
Criteria: neighbor_cost < best_cost

## Result

- with `MAX_ITERATIONS` = 1000, `TABU_TENURE` = 10 and `NEIGHBOR_SAMPLE_SIZE` = 200

| Instance      | Tabu Best Cost | Best Known Solution | Accuracy (%) | Elapsed Time (s) |
|---------------|----------------|---------------------|--------------|------------------|
 a280.txt       | 2979           | 2579                | 86.0         | 3.04             |
| bayg29.txt*   | 1376           | 1610                | 85.5         | 1.03             |
| bays29.txt*   | 1625           | 2020                | 80.4         | 0.99             |
| brazil58.txt* | 17671          | 25395               | 69.6         | 1.31             |
| nrw1379.txt   | 68930          | 56638               | 82.0         | 10.70            |
| pr1002.txt    | 316814         | 259045              | 81.6         | 8.36             |

 - with `MAX_ITERATIONS` = 5000, `TABU_TENURE` = 15 and `NEIGHBOR_SAMPLE_SIZE` = 500

| Instance      | Tabu Best Cost | Best Known Solution | Accuracy (%) | Elapsed Time (s) |
|---------------|----------------|---------------------|--------------|------------------|
| a280.txt      | 2875           | 2579                | 89.0         | 33.43            |
| bayg29.txt*   | 1398           | 1610                | 86.8         | 12.27            |
| bays29.txt*   | 1734           | 2020                | 85.9         | 12.25            |
| brazil58.txt* | 15431          | 25395               | 60.8         | 14.63            |
| nrw1379.txt   | 68256          | 56638               | 82.0         | 122.551          |
| pr1002.txt    | 314778         | 259045              | 82.0         | 91.40            |