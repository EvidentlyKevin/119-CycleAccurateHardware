import numpy as np
import os

def generate_vector_file(file_number, num_batches, samples_per_batch, vector_size):
    filename = f'0801008n{file_number}'
    with open(filename, 'w') as f:
        for batch in range(num_batches):
            for sample in range(samples_per_batch):
                vector = np.random.randint(-127, 128, size=vector_size, dtype=np.int8)
                vector_str = ','.join(map(str, vector))
                f.write(vector_str + '\n')
            
            if batch < num_batches - 1:
                f.write('#\n')

def generate_multiple_files(num_files, num_batches, samples_per_batch, vector_size):
    for i in range(num_files):
        generate_vector_file(i, num_batches, samples_per_batch, vector_size)
        print(f"Generated file: 0801008n{i}")

def main():
    num_files = 5
    num_batches = 8
    samples_per_batch = 10
    vector_size = 8

    generate_multiple_files(num_files, num_batches, samples_per_batch, vector_size)
    
    print("\nAll files have been generated. Here's a preview of the first file:")
    with open('0801008n0', 'r') as f:
        content = f.read(500)
        print(content + "..." if len(content) == 500 else content)

if __name__ == "__main__":
    main()
