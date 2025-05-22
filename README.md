# 💽 Disk Scheduling Algorithm Simulator  
**Simulation and Fairness Analysis of FIFO, SSTF, SCAN, and C-SCAN**

This project simulates common **disk scheduling algorithms** to compare their efficiency and fairness. It processes random disk track requests and analyzes the number of tracks traversed as well as the delays caused to requests when they are handled out of FIFO order.

---

## 📚 Project Context

Disk heads must move between tracks to serve I/O requests. The **order in which requests are processed** impacts performance and fairness. This project models four major disk scheduling strategies:

- **FIFO** – First-In-First-Out
- **SSTF** – Shortest Seek Time First
- **SCAN** – Elevator Algorithm
- **C-SCAN** – Circular SCAN

The simulation evaluates:
- Total tracks traversed
- Longest and average delay per request
- Delay histograms across track ranges

---

## 🧪 Simulation Model

- Disk with 100 tracks numbered 0–99
- Random sequence of up to 1000 requests
- Configurable via command-line: number of requests, random seed
- Initial head position: **Track 50**
- Output: Track movement, fairness stats, and histograms

---

## 📊 Sample Output & Explanation

```
Generating 25 random track requests with seed 2
Testing with 25 track requests

=== Disk Scheduling Algorithm Performance ===
Initial head position: 50
FIFO: 769 tracks traversed
SSTF: 108 tracks traversed
SCAN: 144 tracks traversed
C-SCAN: 97 tracks traversed

=== Fairness Analysis (compared to FIFO) ===

SSTF Fairness:
Longest delay: 21 requests
Requests delayed: 12 out of 25 (48.00%)
Average delay for delayed requests: 6.83 requests
Requests serviced early: 11 out of 25 (44.00%)
Average early service: 7.45 requests

Detail for first 10 requests:
Track | Entry Order | Service Order | Delay
------|-------------|--------------|------
90    | 0           | 0            | 0    
19    | 1           | 22           | 21   
88    | 2           | 1            | -1   
75    | 3           | 6            | 3    
61    | 4           | 13           | 9    
98    | 5           | 3            | -2   
64    | 6           | 12           | 6    
77    | 7           | 5            | -2   
45    | 8           | 17           | 9    
27    | 9           | 21           | 12   

SSTF Delay Histogram:
Track Range | Avg Delay | Max Delay | Histogram (each █ = approx. 1 unit of delay)
------------|-----------|-----------|-----------------------------------
 0 -  9     |      4.00 |         4 | ███████████████
10 - 19     |     10.50 |        21 | ████████████████████████████████████████
20 - 29     |     12.00 |        12 | ██████████████████████████████████████████████
30 - 39     |      3.00 |         5 | ███████████
40 - 49     |      5.25 |         9 | ████████████████████
50 - 59     |     -2.00 |         0 | 
60 - 69     |     -3.80 |         9 | 
70 - 79     |     -5.80 |         3 | 
80 - 89     |     -1.00 |         0 | 
90 - 99     |     -4.33 |         0 | 

SCAN Fairness:
Longest delay: 21 requests
Requests delayed: 14 out of 25 (56.00%)
Average delay for delayed requests: 7.21 requests
Requests serviced early: 9 out of 25 (36.00%)
Average early service: 11.22 requests

Detail for first 10 requests:
Track | Entry Order | Service Order | Delay
------|-------------|--------------|------
90    | 0           | 12           | 12   
19    | 1           | 22           | 21   
88    | 2           | 11           | 9    
75    | 3           | 8            | 5    
61    | 4           | 1            | -3   
98    | 5           | 14           | 9    
64    | 6           | 2            | -4   
77    | 7           | 9            | 2    
45    | 8           | 17           | 9    
27    | 9           | 21           | 12   

SCAN Delay Histogram:
Track Range | Avg Delay | Max Delay | Histogram (each █ = approx. 1 unit of delay)
------------|-----------|-----------|-----------------------------------
 0 -  9     |      4.00 |         4 | ███████████████
10 - 19     |     10.50 |        21 | ████████████████████████████████████████
20 - 29     |     12.00 |        12 | ██████████████████████████████████████████████
30 - 39     |      3.00 |         5 | ███████████
40 - 49     |      5.25 |         9 | ████████████████████
50 - 59     |    -16.00 |         0 | 
60 - 69     |    -11.80 |         0 | 
70 - 79     |     -3.80 |         5 | 
80 - 89     |      9.00 |         9 | ██████████████████████████████████
90 - 99     |      7.00 |        12 | ██████████████████████████

C-SCAN Fairness:
Longest delay: 16 requests
Requests delayed: 12 out of 25 (48.00%)
Average delay for delayed requests: 9.42 requests
Requests serviced early: 11 out of 25 (44.00%)
Average early service: 10.27 requests

Detail for first 10 requests:
Track | Entry Order | Service Order | Delay
------|-------------|--------------|------
90    | 0           | 12           | 12   
19    | 1           | 17           | 16   
88    | 2           | 11           | 9    
75    | 3           | 8            | 5    
61    | 4           | 1            | -3   
98    | 5           | 14           | 9    
64    | 6           | 2            | -4   
77    | 7           | 9            | 2    
45    | 8           | 22           | 14   
27    | 9           | 18           | 9    

C-SCAN Delay Histogram:
Track Range | Avg Delay | Max Delay | Histogram (each █ = approx. 1 unit of delay)
------------|-----------|-----------|-----------------------------------
 0 -  9     |     -5.00 |         0 | 
10 - 19     |      4.50 |        16 | ██████████████████
20 - 29     |      9.00 |         9 | █████████████████████████████████████
30 - 39     |      3.00 |         6 | ████████████
40 - 49     |     11.25 |        14 | ██████████████████████████████████████████████
50 - 59     |    -16.00 |         0 | 
60 - 69     |    -11.80 |         0 | 
70 - 79     |     -3.80 |         5 | 
80 - 89     |      9.00 |         9 | █████████████████████████████████████
90 - 99     |      7.00 |        12 | █████████████████████████████
```

```
In this sample output, we are generating 25 random track requests with a seed value of 2.
We recorded the number of tracks traversed for each of the 4 algorithms to compare their performance at a glance.
We then analyzed the fairness of each algorithm compared to FIFO in a detailed tables.
FIFO is the simplest algorithm but requires by far the most head movement at 769 tracks traversed.
The performance of the Scan algorithm lies between FIFO and SSTF,
but starvation is eliminated since each request is guaranteed to be served in one sweep of the r/w head.
SSTF has significantly less tracks traversed than FIFO because it always serves the shortest seek
times at its current position.
For C-scan, when the outmost request was reached, the r/w head goes to the outermost opposite track.
While SSTF theoretically should minimize head movement by prioritizing the nearest tracks,
it makes greedy local decisions that aren't always globally optimal which explains
why C-SCAN actually achieved lower total track traversal
in our tests.
```
```
The longest delay experienced by any track was 21 by the Scan and SSTF algorithms.
For the SSTF this can be due to the locally greedy decisions which aren't globally optimal
which leads to heavily delayed processing of requests farther away from the r/w head's current position.
For Scan, this could be because of the elevator-like behavior which causes requests made in the opposite
direction of the head have to wait much longer until its direction is reversed.

For the average delay, every negative number indicates that the average delays were comparatively less
than the average delay times using FIFO and vice versa.
These average delays are graphically shown in the histograms.
```

---

## 🧠 Goals & Takeaways

- Understand trade-offs in efficiency vs. fairness
- Visualize request delays in histograms
- Compare real-world implications of disk scheduling logic

---

## 🙏 Credits

This project was developed by [@SydneyEckstein](https://github.com/SydneyEckstein) and **Richard Stoiberer** for the course **Operating Systems** at **Rollins College**, taught by **Dr. Valerie Summet**.

---

## 📘 Project Inspiration

The project idea and specifications were adapted from the textbook:

**Operating Systems**  
zyBook ISBN: 979-8-203-91760-7  
**Author:** Lubomir Bic – Professor of Computer Science, University of California, Irvine

---

## 🚀 How to Run the Simulation

### 1. Compile the program

In your terminal, navigate to the project folder and run:
```
gcc main.c -o disk_scheduler
```
This compiles main.c and creates an executable named disk_scheduler.

### 2. Run the program
```
./disk_scheduler [number_of_requests] [random_seed]
```
Example:
```
./disk_scheduler 25 2
```
- number_of_requests: How many random track requests to generate (e.g., 25)(max 999)
- random_seed: Any integer for reproducibility (e.g., 2)

### 3. Output

The program will:
- Generate a file track_requests.txt with random disk requests
- Simulate four scheduling algorithms: FIFO, SSTF, SCAN, and C-SCAN
- Print:
  - Total tracks traversed per algorithm
  - Fairness analysis: max delay, average delay, early service stats
  - Delay histograms across track ranges

---
