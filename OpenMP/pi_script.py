import os

# Build executable
os.system("make pi")
print("\n\n")

# For how many threads we want to run our experiment
thread_values = [2, 4]
# How many darts in each execution
throws_values = [100000, 1000000, 10000000]
# Run the same program with the same arguments
# and get the average values
EXECUTIONS = 5
print(f"OpenMP")
print("-------------------------------------------------------------------------")
for thread_value in thread_values:
    for throws_value in throws_values:
        avg_serial_time = 0 
        avg_parallel_time = 0
        avg_thread_time = 0
        for i in range(0, EXECUTIONS):  
            # Run program and redirect the output to a temp.txt file
            os.system(f"./bin/pi_comp {thread_value} {throws_value} > temp.txt")
            file = open("temp.txt", "r")

            # Parse output
            line = file.readline()
            serial_time = float(line.split()[4])
            line = file.readline()
            parallel_time = float(line.split()[4])
            line = file.readline()
            single_thread_time = float(line.split()[3])

            avg_serial_time += serial_time
            avg_parallel_time += parallel_time
            avg_thread_time += single_thread_time

            file.close()
        
        # Calculate average values
        avg_serial_time = round(avg_serial_time / EXECUTIONS, 4)
        avg_parallel_time = round(avg_parallel_time / EXECUTIONS, 4)
        avg_thread_time = round(avg_thread_time / EXECUTIONS, 4)

        print(f"Threads: {thread_value} | Throws: {throws_value}")
        print(f"Serial time: {avg_serial_time} | Parallel time: {avg_parallel_time} | Single thread time: {avg_thread_time}")
        print("-------------------------------------------------------------------------\n")

# Delete any unnecessary files
os.system("make clean")
os.system("rm -f temp.txt")