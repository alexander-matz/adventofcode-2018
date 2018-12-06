#!/usr/bin/env python

import re

spec_re = re.compile(r'#(\d+) @ (\d+),(\d+): (\d+)x(\d+)')

# Part 1

# width, height = 0, 0
# rects = []
# 
# # parse input, calculate size of fabric
# for line in open("day3-input"):
#     m = spec_re.match(line) 
#     if m == None:
#         raise Exception("line didn't match '%s'" % (line))
#     rect = [int(x) for x in m.groups()[1:]]
#     width = max(width, rect[0] + rect[2])
#     height = max(height, rect[1] + rect[3])
#     rects += [rect]
# 
# # linearized 2d matrix of fabric
# fabric = [0] * width * height
# 
# # mark fabric with "taken count"
# for rect in rects:
#     for y in range(rect[3]):
#         for x in range(rect[2]):
#             fabric[ (rect[1] + y) * width + (rect[0] + x) ] += 1
# 
# # count square inches with more than 1 use
# overused = 0
# for usecount in fabric:
#     if usecount > 1:
#         overused += 1
# print('fabric: %dinx%din, overused: %dsqin' % (width, height, overused))


# Part 2

width, height = 0, 0
rects = []

# set of non-overlapping claims
claims = {}

# parse input, calculate size of fabric
for line in open("day3-input"):
    m = spec_re.match(line) 
    if m == None:
        raise Exception("line didn't match '%s'" % (line))
    rect = [int(x) for x in m.groups()] # CONTAINS ID! INDICES CHANGE
    width = max(width, rect[1] + rect[3])
    height = max(height, rect[2] + rect[4])
    rects += [rect]
    claims[rect[0]] = True

# linearized 2d matrix of fabric
fabric = [0] * width * height

# mark fabric with ids of claims
for rect in rects:
    for y in range(rect[4]):
        for x in range(rect[3]):
            pos = (rect[2] + y) * width + (rect[1] + x)
            if fabric[pos] == 0: # not taken yet, mark as taken
                fabric[pos] = rect[0]
            else: # taken, remove both claims from set
                if fabric[pos] in claims:
                    del claims[fabric[pos]]
                if rect[0] in claims:
                    del claims[rect[0]]

# according to problem description, only one non-overlapping
# claim should remain
for id in claims:
    print('non-overlapping claim: %d' % id)
