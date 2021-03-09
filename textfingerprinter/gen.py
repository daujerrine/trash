import csv
import json
import sys
import random

data = json.load(open('data.json'))

if len(sys.argv) < 2:
    print("Enter a string.")
    sys.exit(1)

for i in sys.argv[1]:
    if (not ((i >= 'a' and i <= 'z') or (i >= 'A' and i <= 'Z'))) or (len(data[i]) == 0):
        print(i, end='')
        continue

    templist = []

    for j in data[i]:
        if j['score'] == 0 or j['score'] == 1:
            templist.append(j)

    if len(templist) == 0:
        print(i, end='')
        continue
    
    choice = random.randint(0, len(templist) - 1)
    print(templist[choice]['value'], end='')
