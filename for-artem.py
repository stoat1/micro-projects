s = '!,lesmu \x00Wtwmlie yAsiro  '
L = [''] * len(s)
for i in range(len(s)):
  L = sorted([s[i] + L[i] for i in range(len(s))])
print [x for x in L if x.endswith('\0')][0][:-1]
