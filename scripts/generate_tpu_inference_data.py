import json
import random

# Function to generate inference data for a systolic array simulation
def generate_tpu_inference_data(num_entries, array_size, filename):
    data = []
    for _ in range(num_entries):
        entry = [round(random.uniform(0, 1), 4) for _ in range(array_size)]
        data.append(entry)

    with open(filename, 'w') as f:
        f.write(json.dumps(data, indent=2))

# Parameters
num_entries = 1000  # number of inference inputs
array_size = 16      # size of each inference input (should match your systolic array)
output_file = "output/tpu_inference_data.json"

# Generate the data
generate_tpu_inference_data(num_entries, array_size, output_file)

print(f"Generated {num_entries} inference entries in '{output_file}'")

'''

### Parameters Explanation:
- **`num_entries`**: Total number of data entries you want for inference.
- **`array_size`**: Number of numeric features per inference input, matching the width of your systolic array.
- **Random values**: Generates numbers between 0 and 1, suitable for normalized inference data.

You can easily customize `num_entries` and `array_size` to suit the number of TPUs and your workload requirements.
'''