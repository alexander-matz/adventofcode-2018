#!/bin/python

# part one

# akk = 0
# for line in open('day1-input'):
#     akk += int(line)
# print('value: %d' % (akk))


# part two

akk = 0
seen = {}
done = False
while not done:
    for line in open('day1-input'):
        akk += int(line)
        if akk in seen:
            print('saw %d twice' % akk)
            done = True
            break
        seen[akk] = True
