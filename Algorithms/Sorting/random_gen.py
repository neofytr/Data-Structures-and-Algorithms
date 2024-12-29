import random

file_path = "input.txt"
num_numbers = 10**3
chunk_size = 10**2

def generate_large_file(file_path, num_numbers, chunk_size):
    with open(file_path, 'w') as file:
        for _ in range(0, num_numbers, chunk_size):
            chunk = (str(random.randint(0, 10**9)) + ' ' for _ in range(chunk_size))
            file.write(''.join(chunk))

generate_large_file(file_path, num_numbers, chunk_size)

file_path
