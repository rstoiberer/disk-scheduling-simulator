// Richie and Sydney, CMS 470, Dr. Summet, April 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
 
#define MAX_TRACKS 100
#define MAX_REQUESTS 1000
 
// Struct to track request information for fairness analysis
typedef struct {
    int track;          // Track number requested
    int entry_order;    // When the request entered the queue (FIFO order)
    int service_order;  // When the request was actually serviced
    int delay;          // Delay = service_order - entry_order
} Request;
 
// Function declarations
void generate_requests(int m, int seed, const char *filename);
void read_requests(const char *filename, int *requests, int *m);
int fifo_scheduler(int *requests, int m, Request *req_info);
int sstf_scheduler(int *requests, int m, Request *req_info);
int scan_scheduler(int *requests, int m, int initial_pos, Request *req_info);
int c_scan_scheduler(int *requests, int m, int initial_pos, Request *req_info);
void analyze_fairness(Request *req_info, int m);
void print_histogram(Request *req_info, int m);
 
/**
 * Main program entry point
 * Parses command line arguments, generates requests, and executes the disk
 * scheduling algorithms for comparison
 * 
 * Inputs: Command line arguments (number of requests and random seed)
 * Outputs: Comparison of disk scheduling algorithm performance and fairness metrics
 */
int main(int argc, char *argv[]) {
    int m = 100;  // Default number of requests
    int seed = time(NULL);  // Default seed is current time
    int *requests;
    Request *req_info_fifo, *req_info_sstf, *req_info_scan, *req_info_cscan;
   
    // Check for command line arguments
    if (argc >= 2) {
        m = atoi(argv[1]);
    }
    if (argc >= 3) {
        seed = atoi(argv[2]);
    }
   
    printf("Generating %d random track requests with seed %d\n", m, seed);
    generate_requests(m, seed, "track_requests.txt");
   
    // Allocate memory for requests and tracking structs
    requests = (int *)malloc(m * sizeof(int));
    req_info_fifo = (Request *)malloc(m * sizeof(Request));
    req_info_sstf = (Request *)malloc(m * sizeof(Request));
    req_info_scan = (Request *)malloc(m * sizeof(Request));
    req_info_cscan = (Request *)malloc(m * sizeof(Request));
   
    // Error message for failed memory allocation
    if (!requests || !req_info_fifo || !req_info_sstf || !req_info_scan || !req_info_cscan) {
        printf("Memory allocation failed\n");
        return 1;
    }
   
    // Read requests from file
    read_requests("track_requests.txt", requests, &m);
   
    printf("Testing with %d track requests\n", m);
   
    // Initial head position (start at track 50)
    int initial_pos = 50;
   
    // Execute each scheduling algorithm and measure performance
    int fifo_tracks = fifo_scheduler(requests, m, req_info_fifo);
    int sstf_tracks = sstf_scheduler(requests, m, req_info_sstf);
    int scan_tracks = scan_scheduler(requests, m, initial_pos, req_info_scan);
    int cscan_tracks = c_scan_scheduler(requests, m, initial_pos, req_info_cscan);
   
    // Print results
    printf("\n=== Disk Scheduling Algorithm Performance ===\n");
    printf("Initial head position: %d\n", initial_pos);
    printf("FIFO: %d tracks traversed\n", fifo_tracks);
    printf("SSTF: %d tracks traversed\n", sstf_tracks);
    printf("SCAN: %d tracks traversed\n", scan_tracks);
    printf("C-SCAN: %d tracks traversed\n", cscan_tracks);
   
    // Analyze fairness for each algorithm
    printf("\n=== Fairness Analysis (compared to FIFO) ===\n");
   
    printf("\nSSTF Fairness:\n");
    analyze_fairness(req_info_sstf, m);
    printf("\nSSTF Delay Histogram:\n");
    print_histogram(req_info_sstf, m);
   
    printf("\nSCAN Fairness:\n");
    analyze_fairness(req_info_scan, m);
    printf("\nSCAN Delay Histogram:\n");
    print_histogram(req_info_scan, m);
   
    printf("\nC-SCAN Fairness:\n");
    analyze_fairness(req_info_cscan, m);
    printf("\nC-SCAN Delay Histogram:\n");
    print_histogram(req_info_cscan, m);
   
    // Free allocated memory
    free(requests);
    free(req_info_fifo);
    free(req_info_sstf);
    free(req_info_scan);
    free(req_info_cscan);
   
    return 0;
}
 
/**
 * Generate random track requests and write them to a file
 * 
 * Inputs:
 *   - m: Number of track requests to generate
 *   - seed: Random seed value for reproducible results
 *   - filename: Name of the output file
 * 
 * Outputs: Creates a file with m random track requests
 */
void generate_requests(int m, int seed, const char *filename) {
    FILE *fp;
    int i;
   
    srand(seed);
   
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file for writing\n");
        exit(1);
    }
   
    // Write the number of requests to the first line
    fprintf(fp, "%d\n", m);
   
    // Generate and write m random track numbers
    for (i = 0; i < m; i++) {
        fprintf(fp, "%d\n", rand() % MAX_TRACKS);
    }
   
    fclose(fp);
}
 
/**
 * Read previously generated track requests from a file
 * 
 * Inputs:
 *   - filename: Name of the input file
 *   - requests: Array to store the track requests
 *   - m: Pointer to store the number of requests
 * 
 * Outputs: Populates the requests array and sets m to the number of requests
 */
void read_requests(const char *filename, int *requests, int *m) {
    FILE *fp;
    int i, num_requests;
   
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file for reading\n");
        exit(1);
    }
   
    // Read the number of requests from the first line
    fscanf(fp, "%d", &num_requests);
    *m = num_requests;
   
    // Read each track request
    for (i = 0; i < num_requests; i++) {
        fscanf(fp, "%d", &requests[i]);
    }
   
    fclose(fp);
}
 
/**
 * FIFO (First-In-First-Out) disk scheduling algorithm
 * Processes requests in the exact order they arrive
 * 
 * Inputs:
 *   - requests: Array of track numbers requested
 *   - m: Number of requests
 *   - req_info: Array to store request processing information
 * 
 * Outputs: Total number of tracks traversed by the disk head
 */
int fifo_scheduler(int *requests, int m, Request *req_info) {
    int i;
    int current_pos = requests[0];  // Start at the first request
    int total_movement = 0;
   
    for (i = 0; i < m; i++) {
        // Store request information for fairness analysis
        req_info[i].track = requests[i];
        req_info[i].entry_order = i;
        req_info[i].service_order = i;  // FIFO serves in original order
        req_info[i].delay = 0;          // No delay in FIFO
       
        if (i > 0) {
            // Calculate absolute distance between tracks
            total_movement += abs(requests[i] - requests[i-1]);
        }
    }
   
    return total_movement;
}
 
/**
 * SSTF (Shortest-Seek-Time-First) disk scheduling algorithm
 * Always selects the unserviced track closest to current head position
 * 
 * Inputs:
 *   - requests: Array of track numbers requested
 *   - m: Number of requests
 *   - req_info: Array to store request processing information
 * 
 * Outputs: Total number of tracks traversed by the disk head
 */
int sstf_scheduler(int *requests, int m, Request *req_info) {
    int i, j;
    int current_pos = requests[0];  // Start at the first request
    int total_movement = 0;
    bool *serviced = (bool *)calloc(m, sizeof(bool));
    int *service_order = (int *)malloc(m * sizeof(int));
   
    if (!serviced || !service_order) {
        printf("Memory allocation failed\n");
        exit(1);
    }
   
    // Initialize original request information
    for (i = 0; i < m; i++) {
        req_info[i].track = requests[i];
        req_info[i].entry_order = i;
    }
   
    // First request is served immediately
    serviced[0] = true;
    service_order[0] = 0;
   
    // Process remaining requests using SSTF
    for (i = 1; i < m; i++) {
        int min_distance = INT_MAX;
        int next_index = -1;
       
        // Find the closest unserviced track
        for (j = 0; j < m; j++) {
            if (!serviced[j]) {
                int distance = abs(requests[j] - current_pos);
                if (distance < min_distance) {
                    min_distance = distance;
                    next_index = j;
                }
            }
        }
       
        // Mark as serviced and update current position
        serviced[next_index] = true;
        service_order[i] = next_index;
        total_movement += min_distance;
        current_pos = requests[next_index];
    }
   
    // Update service order and calculate delays
    for (i = 0; i < m; i++) {
        int idx = service_order[i];
        req_info[idx].service_order = i;
        req_info[idx].delay = req_info[idx].service_order - req_info[idx].entry_order;
    }
   
    free(serviced);
    free(service_order);
   
    return total_movement;
}
 
/**
 * SCAN (Elevator) disk scheduling algorithm
 * Head moves in one direction until reaching the last request, then reverses
 * 
 * Inputs:
 *   - requests: Array of track numbers requested
 *   - m: Number of requests
 *   - initial_pos: Initial position of the disk head
 *   - req_info: Array to store request processing information
 * 
 * Outputs: Total number of tracks traversed by the disk head
 */
int scan_scheduler(int *requests, int m, int initial_pos, Request *req_info) {
    int i;
    int total_movement = 0;
    int current_pos = initial_pos;
    bool *serviced = (bool *)calloc(m, sizeof(bool));
    int *service_order = (int *)malloc(m * sizeof(int));
    int serviced_count = 0;
   
    // Direction: true = up, false = down
    bool direction = true; // Start moving up
   
    if (!serviced || !service_order) {
        printf("Memory allocation failed\n");
        exit(1);
    }
   
    // Initialize original request information
    for (i = 0; i < m; i++) {
        req_info[i].track = requests[i];
        req_info[i].entry_order = i;
    }
   
    // Keep going until all requests are serviced
    while (serviced_count < m) {
        // Collect all requests in the current direction and sort them
        int direction_requests[MAX_REQUESTS];
        int req_indices[MAX_REQUESTS];
        int direction_count = 0;
       
        if (direction) {
            // Moving upward: collect all requests >= current_pos
            for (i = 0; i < m; i++) {
                if (!serviced[i] && requests[i] >= current_pos) {
                    direction_requests[direction_count] = requests[i];
                    req_indices[direction_count] = i;
                    direction_count++;
                }
            }
           
            // Sort requests in ascending order (bubble sort)
            for (i = 0; i < direction_count - 1; i++) {
                for (int j = 0; j < direction_count - i - 1; j++) {
                    if (direction_requests[j] > direction_requests[j + 1]) {
                        // Swap requests
                        int temp = direction_requests[j];
                        direction_requests[j] = direction_requests[j + 1];
                        direction_requests[j + 1] = temp;
                       
                        // Swap indices
                        temp = req_indices[j];
                        req_indices[j] = req_indices[j + 1];
                        req_indices[j + 1] = temp;
                    }
                }
            }
        } else {
            
            // Moving downward: collect all requests <= current_pos
            for (i = 0; i < m; i++) {
                if (!serviced[i] && requests[i] <= current_pos) {
                    direction_requests[direction_count] = requests[i];
                    req_indices[direction_count] = i;
                    direction_count++;
                }
            }
           
            // Sort requests in descending order (bubble sort)
            for (i = 0; i < direction_count - 1; i++) {
                for (int j = 0; j < direction_count - i - 1; j++) {
                    if (direction_requests[j] < direction_requests[j + 1]) {
                        // Swap requests
                        int temp = direction_requests[j];
                        direction_requests[j] = direction_requests[j + 1];
                        direction_requests[j + 1] = temp;
                       
                        // Swap indices
                        temp = req_indices[j];
                        req_indices[j] = req_indices[j + 1];
                        req_indices[j + 1] = temp;
                    }
                }
            }
        }
       
        // Service all requests in the current direction
        if (direction_count > 0) {
            for (i = 0; i < direction_count; i++) {
                int next_track = direction_requests[i];
                int next_index = req_indices[i];
               
                // Add movement to next track
                total_movement += abs(next_track - current_pos);
                current_pos = next_track;
               
                // Mark as serviced
                serviced[next_index] = true;
                service_order[serviced_count++] = next_index;
            }
        }
       
        // Change direction
        direction = !direction;
    }
   
    // Update service order and calculate delays
    for (i = 0; i < m; i++) {
        int idx = service_order[i];
        req_info[idx].service_order = i;
        req_info[idx].delay = req_info[idx].service_order - req_info[idx].entry_order;
    }
   
    free(serviced);
    free(service_order);
   
    return total_movement;
}
 
/**
 * C-SCAN (Circular SCAN) disk scheduling algorithm
 * Head moves in one direction only, and when it reaches the end,
 * jumps back to the beginning and continues in the same direction
 * 
 * Inputs:
 *   - requests: Array of track numbers requested
 *   - m: Number of requests
 *   - initial_pos: Initial position of the disk head
 *   - req_info: Array to store request processing information
 * 
 * Outputs: Total number of tracks traversed by the disk head
 */
int c_scan_scheduler(int *requests, int m, int initial_pos, Request *req_info) {
    int i;
    int total_movement = 0;
    int current_pos = initial_pos;
    bool *serviced = (bool *)calloc(m, sizeof(bool));
    int *service_order = (int *)malloc(m * sizeof(int));
    int serviced_count = 0;
   
    // Direction: only upward for C-SCAN
   
    if (!serviced || !service_order) {
        printf("Memory allocation failed\n");
        exit(1);
    }
   
    // Initialize original request information
    for (i = 0; i < m; i++) {
        req_info[i].track = requests[i];
        req_info[i].entry_order = i;
    }
   
    // Keep going until all requests are serviced
    while (serviced_count < m) {
        // Collect all requests >= current_pos and sort them
        int upper_requests[MAX_REQUESTS];
        int upper_indices[MAX_REQUESTS];
        int upper_count = 0;
       
        // Moving upward: collect all requests >= current_pos
        for (i = 0; i < m; i++) {
            if (!serviced[i] && requests[i] >= current_pos) {
                upper_requests[upper_count] = requests[i];
                upper_indices[upper_count] = i;
                upper_count++;
            }
        }
       
        // Sort requests in ascending order (bubble sort)
        for (i = 0; i < upper_count - 1; i++) {
            for (int j = 0; j < upper_count - i - 1; j++) {
                if (upper_requests[j] > upper_requests[j + 1]) {
                    // Swap requests
                    int temp = upper_requests[j];
                    upper_requests[j] = upper_requests[j + 1];
                    upper_requests[j + 1] = temp;
                   
                    // Swap indices
                    temp = upper_indices[j];
                    upper_indices[j] = upper_indices[j + 1];
                    upper_indices[j + 1] = temp;
                }
            }
        }
       
        // Service all upper requests
        if (upper_count > 0) {
            for (i = 0; i < upper_count; i++) {
                int next_track = upper_requests[i];
                int next_index = upper_indices[i];
               
                // Add movement to next track
                total_movement += abs(next_track - current_pos);
                current_pos = next_track;
               
                // Mark as serviced
                serviced[next_index] = true;
                service_order[serviced_count++] = next_index;
            }
        }
       
        // If we haven't serviced all requests, jump to the beginning
        if (serviced_count < m) {
            // Reset to beginning (track 0) - this movement isn't counted in C-SCAN
            current_pos = 0;
           
            // Now collect all remaining requests (which are < initial_pos)
            int remaining_requests[MAX_REQUESTS];
            int remaining_indices[MAX_REQUESTS];
            int remaining_count = 0;
           
            for (i = 0; i < m; i++) {
                if (!serviced[i]) {
                    remaining_requests[remaining_count] = requests[i];
                    remaining_indices[remaining_count] = i;
                    remaining_count++;
                }
            }
           
            // Sort remaining requests in ascending order
            for (i = 0; i < remaining_count - 1; i++) {
                for (int j = 0; j < remaining_count - i - 1; j++) {
                    if (remaining_requests[j] > remaining_requests[j + 1]) {
                        // Swap requests
                        int temp = remaining_requests[j];
                        remaining_requests[j] = remaining_requests[j + 1];
                        remaining_requests[j + 1] = temp;
                       
                        // Swap indices
                        temp = remaining_indices[j];
                        remaining_indices[j] = remaining_indices[j + 1];
                        remaining_indices[j + 1] = temp;
                    }
                }
            }
           
            // Service all remaining requests
            for (i = 0; i < remaining_count; i++) {
                int next_track = remaining_requests[i];
                int next_index = remaining_indices[i];
               
                // Add movement to next track
                total_movement += abs(next_track - current_pos);
                current_pos = next_track;
               
                // Mark as serviced
                serviced[next_index] = true;
                service_order[serviced_count++] = next_index;
            }
        }
    }
   
    // Update service order and calculate delays
    for (i = 0; i < m; i++) {
        int idx = service_order[i];
        req_info[idx].service_order = i;
        req_info[idx].delay = req_info[idx].service_order - req_info[idx].entry_order;
    }
   
    free(serviced);
    free(service_order);
   
    return total_movement;
}
 
/**
 * Analyze fairness metrics of a scheduling algorithm
 * Calculates and displays metrics like maximum delay, average delay,
 * percent of requests delayed/early, etc.
 * 
 * Inputs:
 *   - req_info: Array containing request processing information
 *   - m: Number of requests
 * 
 * Outputs: Prints fairness metrics to standard output
 */
void analyze_fairness(Request *req_info, int m) {
    int i;
    int max_delay = 0;
    int delayed_count = 0;
    int total_delay = 0;
    int early_count = 0;
    int total_early = 0;
   
    // Find the maximum delay and calculate average delay
    for (i = 0; i < m; i++) {
        if (req_info[i].delay > 0) {
            delayed_count++;
            total_delay += req_info[i].delay;
            if (req_info[i].delay > max_delay) {
                max_delay = req_info[i].delay;
            }
        } else if (req_info[i].delay < 0) {
            early_count++;
            total_early += -req_info[i].delay; // Convert to positive
        }
    }
   
    // Print fairness statistics
    printf("Longest delay: %d requests\n", max_delay);
    printf("Requests delayed: %d out of %d (%.2f%%)\n",
           delayed_count, m, (float)delayed_count/m*100);
   
    if (delayed_count > 0) {
        printf("Average delay for delayed requests: %.2f requests\n",
               (float)total_delay/delayed_count);
    } else {
        printf("Average delay for delayed requests: 0\n");
    }
   
    printf("Requests serviced early: %d out of %d (%.2f%%)\n",
           early_count, m, (float)early_count/m*100);
   
    if (early_count > 0) {
        printf("Average early service: %.2f requests\n",
               (float)total_early/early_count);
    } else {
        printf("Average early service: 0\n");
    }
   
    // Print table header for the first 10 requests
    printf("\nDetail for first 10 requests:\n");
    printf("Track | Entry Order | Service Order | Delay\n");
    printf("------|-------------|--------------|------\n");
   
    // Print table data for the first 10 requests
    for (i = 0; i < (m < 10 ? m : 10); i++) {
        printf("%-5d | %-11d | %-12d | %-5d\n",
               req_info[i].track,
               req_info[i].entry_order,
               req_info[i].service_order,
               req_info[i].delay);
    }
}
 
/**
 * Creates a visual histogram of track request delays by track range
 * Divides tracks into 10 bins and displays average and maximum delay for each
 * 
 * Inputs:
 *   - req_info: Array containing request processing information
 *   - m: Number of requests
 * 
 * Outputs: Prints a formatted histogram to standard output
 */
void print_histogram(Request *req_info, int m) {
    int i, j;
    int bins[10] = {0}; // Divide tracks into 10 ranges
    int bin_size = MAX_TRACKS / 10;
    int max_delay_per_bin[10] = {0};
    float avg_delay_per_bin[10] = {0};
    int count_per_bin[10] = {0};
    int max_avg_delay = 0;
   
    // Calculate delay statistics for each bin
    for (i = 0; i < m; i++) {
        int track = req_info[i].track;
        int bin = track / bin_size;
       
        // Handle edge case for track 99
        if (bin >= 10) bin = 9;
       
        count_per_bin[bin]++;
        avg_delay_per_bin[bin] += req_info[i].delay;
       
        if (req_info[i].delay > max_delay_per_bin[bin]) {
            max_delay_per_bin[bin] = req_info[i].delay;
        }
    }
   
    // Calculate averages and find maximum average delay for scaling
    for (i = 0; i < 10; i++) {
        if (count_per_bin[i] > 0) {
            avg_delay_per_bin[i] /= count_per_bin[i];
            if (avg_delay_per_bin[i] > max_avg_delay) {
                max_avg_delay = (int)avg_delay_per_bin[i] + 1;
            }
        }
    }
   
    // Print histogram header
    printf("Track Range | Avg Delay | Max Delay | Histogram (each █ = approx. 1 unit of delay)\n");
    printf("------------|-----------|-----------|-----------------------------------\n");
   
    // Define histogram width and scale
    int hist_width = 50; // Width of histogram in characters
    float scale = (float)hist_width / max_avg_delay;
    if (scale < 1) scale = 1; // Ensure at least 1 character per unit
   
    // Print each bin
    for (i = 0; i < 10; i++) {
        printf("%2d - %2d     | %9.2f | %9d | ",
               i*bin_size, (i+1)*bin_size-1,
               avg_delay_per_bin[i], max_delay_per_bin[i]);
       
        // Print histogram bars with better visualization
        int bars = (int)(avg_delay_per_bin[i] * scale);
        for (j = 0; j < bars; j++) {
            printf("█"); // Using Unicode block character for better visualization
        }
        printf("\n");
    }
}