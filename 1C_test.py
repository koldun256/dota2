text = input()

words = [input() for _ in range(int(input()))]
ans = [[] for _ in words]

for i in range(len(text)):
    for j in range(i, len(text)):
        for k in range(len(words)):
            if text[i:j + 1] == words[k]:
                ans[k].append(i + 1)
                
for a in ans:
    print(f"{len(a)} {' '.join(str(b) for b in a)}")