result_str = ""

for i in range(4):
    for j in range(4):
        result_str += "T" + str(i) + str(j) + ","
        result_str += "R" + str(i) + str(j) + ","
        result_str += "L" + str(i) + str(j) + ","
        result_str += "D" + str(i) + str(j) + ","

print(result_str)