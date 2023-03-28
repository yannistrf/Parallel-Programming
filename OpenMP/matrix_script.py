import os

# times we run the program with the same arguments
# to get more accurate results
REPS = 5
# number of threads we will run the experiment
threads = [2, 4]
# types of schedule we will run the experiment
schedules = ["static", "dynamic", "guided", "auto"]
# the dimensions of the matrix
args = "10000 10000"

# build executable
os.system("make mat")
# the name of the executable
app = "bin/omp_mat_vect_rand_split"

print(f"\n*** Running experiment with args = {args} ***")

# doesn't matter, running on 1 thread
os.environ["OMP_SCHEDULE"] = "static"
# execute experiment
for i in range(REPS):
    os.system(f"./{app} 1 {args} >> temp.txt")

out = open("temp.txt", "r")

# read the output from the temp file
# and calculate the average time
lines = out.readlines()
avg_serial_time = 0
for line in lines:
    serial_time = float(line.split()[3])
    avg_serial_time += serial_time

avg_serial_time /= REPS
print(f"\n>> Serial time: {round(avg_serial_time, 5)}")

# loop through the number of threads
for thread in threads:
    print(f"\n### THREADS = {thread} ###")
    print("*******************")
    # loop through the schedules
    for schedule in schedules:
        print(f"> Schedule = {schedule}")
        os.environ["OMP_SCHEDULE"] = schedule
        avg_parallel_time = 0
        # execute experiment
        for i in range(REPS):
            os.system(f"./{app} {thread} {args} >> temp.txt")
        # read the output from the temp file
        # and calculate the average time
        lines = out.readlines()
        for line in lines:
            parallel_time = float(line.split()[3])
            avg_parallel_time += parallel_time
        avg_parallel_time /= REPS
        print(f"Average execution time: {round(avg_parallel_time, 5)}")
        print("-------------------------------")
    
# delete temp and binary files
out.close()
os.system("make clean")
os.system("rm -f temp.txt")