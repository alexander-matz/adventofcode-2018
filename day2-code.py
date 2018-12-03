#!/usr/bin/env python

# part 1

# twos = 0
# threes = 0
# for line in open("day2-input"):
#     counts = {}
#     for letter in line:
#         if letter in counts: counts[letter] += 1
#         else: counts[letter] = 1
#     two = 0
#     three = 0
#     for letter in counts:
#         if counts[letter] == 2: two = 1
#         if counts[letter] == 3: three = 1
#     twos += two
#     threes += three
# print('twos: %d, threes: %d, product: %d' % (twos, threes, twos * threes))

# part 2

def shared_letters(a, b):
    dist = 0
    shared = []
    for i in range(min(len(a), len(b))):
        if a[i] == b[i]:
            shared += [a[i]]
        else:
            dist += 1
    return shared, dist

lines = [line.strip() for line in open("day2-input")]
lines.sort()
last = None
for line in lines:
    if last != None:
        shared, dist = shared_letters(last, line)
        if dist == 1:
            print("similar: %s and %s" % (last, line))
            print("shared: %s" % (''.join(shared)))
    last = line
