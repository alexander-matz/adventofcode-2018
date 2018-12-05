#!/usr/bin/env python

def reacts(a, b):
    return abs(ord(a) - ord(b)) == 32

# each iteration either resets pointer one position back and removes to elements
# or increments counter by 1, max iterations = len(polymer), single "pass" through
# input. String manipulation probably most expensive part
def react(polymer):
    pos = 0
    while pos < len(polymer)-1:
        if reacts(polymer[pos], polymer[pos+1]):
            polymer = polymer[:pos] + polymer[pos+2:] # cut out pos and pos+1
            pos = max(0, pos-1) #  position back to catch new combinations
        else:
            pos += 1 # increment position if no reaction
    return polymer

buf = open("day5-input", "r").read().strip()
#buf = "dabAcCaCBAcCcaDA" # test input

# part 1
# 
print("units after reacting: %s" % (len(react(buf))))

# part 2

shortest = float('inf')
for char in "abcdefghijklmnopqrstuvwxyz":
    pol = buf.replace(char, "").replace(chr(ord(char)-32), "")
    pol = react(pol)
    if len(pol) < shortest:
        shortest = len(pol)
print("shortest polymer by eliminating one type: %s" % (shortest))
