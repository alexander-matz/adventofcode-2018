#!/usr/bin/env python

steps = {}
for line in open('day7-input'):
    line = line.strip()
    if len(line) == 0 or line[0] == '#': continue
    # fragile, but we know the input
    dep = line[5]
    node = line[36]
    # initialize nodes
    if not node in steps: steps[node] = set()
    if not dep in steps: steps[dep] = set()
    steps[node] |= set(dep)

# Part 1

# out = ""
# while True:
#     # discover ready node
#     ready = None
#     for node, deps in sorted(steps.items()):
#         if len(deps) == 0:
#             ready = node
#             break
# 
#     # we're done if there are no ready nodes
#     if ready is None:
#         break
# 
#     out += ready
# 
#     # remove discovered nodes (as nodes and as dependencies)
#     del steps[ready]
#     for node in steps:
#         steps[node] -= set(ready)
# 
# print(out)

# Part 2

out = ""
numelves = 5
elves = [(0, None)] * numelves # (done time, node)
now = 0

while True:
    # finalize finished steps
    for idx, elf in enumerate(elves):
        if elf[0] > now or elf[1] is None:
            continue
        # 1. remove as dependency
        for node in steps:
            steps[node] -= set(elf[1])
        # 2. add to output
        out += elf[1]
        # 3. reset to avoid 'double finalization'
        elves[idx] = (elf[0], None)

    # pick new step
    for idx, elf in enumerate(elves):
        if elf[0] > now:
            continue

        # 1. find first step without dependencies
        ready = None
        for node, deps in sorted(steps.items()):
            if len(deps) == 0:
                ready = node
                break
        if ready is None:
            elves[idx] = (-1, None)
        else:
            # 2. delete from step graph
            del steps[ready]
            # 3. set elf to busy
            elves[idx] = (now + ord(ready)-4, ready)

    idle = 0
    for elf in elves:
        if elf[0] == -1:
            idle += 1
    if idle == numelves:
        break
    now += 1

print(now)
