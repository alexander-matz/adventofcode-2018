#!/usr/bin/env python

import sys
from blist import blist # standard list kills performance (at insert)

#players = 9
#highest = 1000

players = 458
highest = 72019 * 100

scores = [0] * players
marbles = blist([0])
current = 1


def print_marbles():
    for idx, marble in enumerate(marbles):
        if idx == current:
            sys.stdout.write('(%d) ' % marble)
        else:
            sys.stdout.write(' %d  ' % marble)
    sys.stdout.write('\n')

turn = 1
#print_marbles()
while turn <= highest:
    if turn % 23 == 0:
        playerIdx = (turn-1) % players
        other = (current - 7) % len(marbles)
        scores[playerIdx] += turn
        scores[playerIdx] += marbles[other]
        marbles.pop(other)
        current = other
    else:
        ipoint = ((current + 1) % (len(marbles)) + 1)
        marbles.insert(ipoint, turn)
        current = ipoint
    turn += 1
    #print_marbles()

print('highest score: %d' %(max(scores)))
