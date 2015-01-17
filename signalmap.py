import sh

things = sh.kill('--table').split()
signals = {}
while things:
    signals[things.pop(0)] = things.pop(0)

print('#include <string.h>')

print('int signalstr(const char* arg) {')
print('    if(!arg) return -1;')

for sig,num in signals.items():
    print('    if(0==strcasecmp(arg,"{}")) return {};'.format(sig,num))

print('return -1;')
print('}')
