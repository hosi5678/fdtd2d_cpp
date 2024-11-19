import os

def count_csv_files(directory_path):
    csv_count = 0
    for filename in os.listdir(directory_path):
        if filename.endswith(".csv"):
            csv_count += 1

    return csv_count
