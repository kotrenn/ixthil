numbers = set()
with open('stdout.txt') as f:
    for line in f:
        numbers.update([int(line)])
print 'Found ' + str(len(numbers)) + ' numbers'
print numbers
