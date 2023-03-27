import os

filename = "pth_mat_vect_rand"
# Make all 3 executables, one for each method
os.system(f"gcc -g -Wall -o {filename} ./src/{filename}.c -lpthread")
os.system(f"gcc -g -Wall -o {filename}1 ./src/{filename}1.c -lpthread")
os.system(f"gcc -g -Wall -o {filename}2 ./src/{filename}2.c -lpthread")

# Thread numbers that we are gonna experiment with
thread_values = [2, 4]
# Dimension pairs that we are gonna experiment with
dimension_values = [(8000000, 8), (8000, 8000), (8, 8000000)]
# How many times we execute the same program with the same parameters
EXECUTIONS = 1


for thread_value in thread_values:
    for dimensions in dimension_values:
        avg_time_default = 0
        avg_thread_time_default = 0
        avg_time_method1 = 0
        avg_thread_time_method1 = 0
        avg_time_method2 = 0
        avg_thread_time_method2 = 0
        for i in range(0, EXECUTIONS):
            # Run default method program
            os.system(f"./{filename} {thread_value} {dimensions[0]} {dimensions[1]} > temp.txt")
            
            file = open("temp.txt", "r")
            # Parse output
            line = file.readline()
            default_time = float(line.split()[3])
            line = file.readline()
            thread_time_default = float(line.split()[3])

            # Run method1 program
            os.system(f"./{filename}2 {thread_value} {dimensions[0]} {dimensions[1]} > temp.txt")
            # Parse output
            file.seek(0)
            line = file.readline()
            method1_time = float(line.split()[3])
            line = file.readline()
            thread_time_method1 = float(line.split()[3])

            # Run method2 program
            os.system(f"./{filename}2 {thread_value} {dimensions[0]} {dimensions[1]} > temp.txt")
            # Parse output
            file.seek(0)
            line = file.readline()
            method2_time = float(line.split()[3])
            line = file.readline()
            thread_time_method2 = float(line.split()[3])


            avg_time_default += default_time
            avg_thread_time_default += thread_time_default

            avg_time_method1 += method1_time
            avg_thread_time_method1 += thread_time_method1

            avg_time_method2 += method2_time
            avg_thread_time_method2 += thread_time_method2

        # Calculate average values
        avg_time_default = round(avg_time_default / EXECUTIONS, 4)
        avg_thread_time_default = round(avg_thread_time_default / EXECUTIONS, 4)

        avg_time_method1 = round(avg_time_method1 / EXECUTIONS, 4)
        avg_thread_time_method1 = round(avg_thread_time_method1 / EXECUTIONS, 4)

        avg_time_method2 = round(avg_time_method2 / EXECUTIONS, 4)
        avg_thread_time_method2 = round(avg_thread_time_method2 / EXECUTIONS, 4)


        print(f"*** Threads: {thread_value} | Dimensions: {dimensions[0]} x {dimensions[1]} ***")
        
        print("# DEFAULT METHOD")
        print(f"Total Execution time: {avg_time_default} | Single thread time: {avg_thread_time_default}")
        print("----------------------------------------------------------")

        print("# METHOD 1")
        print(f"Total Execution time: {avg_time_method1} | Single thread time: {avg_thread_time_method1}")
        print("----------------------------------------------------------")

        print("# METHOD 2")
        print(f"Total Execution time: {avg_time_method2} | Single thread time: {avg_thread_time_method2}")
        print("----------------------------------------------------------")

        print("\n")
        file.close()

# Delete all unnecessary files
os.system(f"rm -f {filename} {filename}1 {filename}2 temp.txt")