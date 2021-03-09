import csv
import json
import sys
import pprint

"""
Score points (Lesser is better):
    * Thickness (1 point)
    * Width (1 point)
    * Height (1 point)
    * Relative Position (1 point)
    * Glyph style (Sans Serif/Serif/etc.) (Multiple Points depending on glyph)
    * Glyph Decorations (Inside Circle, underlined etc.) (2 points)
    * Support by fonts (Based on empirical observation) (1 point)
    * Variability between font faces
"""

csvfile = csv.reader(open(sys.argv[1]))
scripts = next(csvfile)
adict = dict()

for entry in csvfile:
    adict[entry[0]] = list()
    for i in range(1, len(entry), 2):
        if len(entry[i]) > 0:
            adict[entry[0]].append({ 'score': int(entry[i + 1]), 'script': scripts[i], 'value': entry[i] })

pp = pprint.PrettyPrinter(indent=4)

print(json.dumps(adict, indent = 4))
