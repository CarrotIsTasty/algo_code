import sys
import os

if len(sys.argv) < 2:
    print("Usage: python check_dataset.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

if not os.path.exists(filename):
    print(f"File '{filename}' not found.")
    sys.exit(1)

count = 0
with open(filename, "r") as f:
    for _ in f:
        count += 1

print(f"Rows    : {count}")
print(f"File    : {filename}")
print(f"Size    : {os.path.getsize(filename):,} bytes")
