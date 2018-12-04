#!/usr/bin/env python

import re
import datetime

entry_re = re.compile(r'\[([^\]]+)\] (.+)')


entries = []

for line in open("day4-input"):
    line = line.strip()
    m = entry_re.match(line)
    assert(not m is None)
    date = datetime.datetime.strptime(m.group(1), "%Y-%m-%d %H:%M")
    entries += [(date, m.group(2))]
entries.sort(key = lambda x: x[0])

guards = {}
currentGuard = None
start = None
for entry in entries:
    text = entry[1]
    minute = entry[0].minute
    if text[:7] == 'Guard #':
        assert(start is None)
        currentGuard = int(text.split()[1][1:])
        if not currentGuard in guards:
            guards[currentGuard] = [0] * 60
    elif text == 'falls asleep':
        assert(start is None)
        start = minute
    elif text == 'wakes up':
        assert(not start is None)
        for i in range(start, minute):
            guards[currentGuard][i] += 1
        start = None

# # part 1
# 
# bestGuard = None
# bestTime = None
# bestMinute = None
# 
# for (guard, minutes) in guards.items():
#     asleep = 0
#     best = 0
#     for i in range(60):
#         asleep += minutes[i]
#         if minutes[i] > minutes[best]:
#             best = i
# 
#     if bestTime is None or asleep > bestTime:
#         bestGuard = guard
#         bestTime = asleep
#         bestMinute = best
# 
# print("guard: #%d, asleep: %d, best minute: %d, solution: %d" % \
#         (bestGuard, bestTime, bestMinute, bestGuard * bestMinute))

# part 2

bestGuard = 0
bestFrequency = 0
bestMinute = 0

for (guard, minutes) in guards.items():
    best = 0
    for i in range(60):
        if minutes[i] > bestFrequency:
            bestMinute = i
            bestFrequency = minutes[i]
            bestGuard = guard

print("guard: #%d, best frequency: %d, best minute: %d, solution: %d" % \
        (bestGuard, bestFrequency, bestMinute, bestGuard * bestMinute))
