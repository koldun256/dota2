def cycle(n, s):
    return s[n % len(s):] + s[:n % len(s)]

def occurrences(string, sub):
    count = start = 0
    while True:
        start = string.find(sub, start) + 1
        if start > 0:
            count+=1
        else:
            return count

def calc_occur(text, patterns):
    ans = 0
    for pattern in patterns:
        ans += occurrences(text, pattern)
    return ans

offset = 0
patterns = set()

def process_command(command, arg):
    global offset
    arg = cycle(offset, arg)
    if command == '+':
        patterns.add(arg)
    
    if command == '-':
        if arg in patterns:
            patterns.remove(arg)
        
    if command == '?':
        offset = calc_occur(arg, patterns)
        print(offset)

for _ in range(int(input())):
    command, arg = input().split()
    process_command(command, arg)