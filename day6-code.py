#!/usr/bin/env python

import sys

points = []

for line in open('day6-input'):
    coords = tuple(int(x) for x in line.replace(',', '').split())
    points += [tuple(int(x) for x in line.replace(',', '').split())]

#points = [(1,1), (1,6), (8, 3), (3, 4), (5, 5), (8, 9)]

top = float('inf')
left = float('inf')
bottom = float('-inf')
right = float('-inf')
for point in points:
    left = min(left, point[0])
    top = min(top, point[1])
    right = max(right, point[0])
    bottom = max(bottom, point[1])

# Part 1

# # unoptimized calculation, for each field, check all points
# areas = [0] * len(points)
# for y in range(top, bottom+1):
#     for x in range(left, right+1):
#         closestDist = float('inf')
#         closestID = -1
#         for i, point in enumerate(points):
#             dist = abs(point[0] - x) + abs(point[1] - y)
#             if dist < closestDist:
#                 closestDist = dist
#                 closestID = i
#             elif dist == closestDist:
#                 closestID = '.' #  means 'two or more closest'
#         if closestID != '.':
#             # disqualify areas of points at the border -> infinite size
#             # i simply use nan so it poisons all other computations
#             if x == left or y == top or x == right or y == bottom:
#                 areas[closestID] = float('nan')
#             else:
#                 areas[closestID] += 1
#         #sys.stdout.write('%02s' % closestID)
#     #sys.stdout.write('\n')
# 
# # replace nan with 0
# areas = [0 if x != x else x for x in areas]
# 
# largestArea = max(areas)
# largestID = areas.index(largestArea)
# print('area: %d, id: %d' % (largestArea, largestID))


# Part 2

area = 0
maxdist = 10000
for y in range(top, bottom+1):
    for x in range(left, right+1):
        totaldist = 0
        for point in points:
            totaldist += abs(point[0] - x) + abs(point[1] - y)
        if totaldist < maxdist:
            area += 1
#            sys.stdout.write('#')
#        else:
#            sys.stdout.write('.')
#    sys.stdout.write('\n')

print('area within %d tiles of all points: %d' % (maxdist, area))
