import cs50

counter = 0
counter2 = -1
rownumber = 1
height = 0
while True:
    print("How high is the pyramid?")
    height = cs50.get_int()
    if height < 24 and height > 0:
        break
for rownumber in range(height+1):
    for counter in range(height-rownumber):
        print(" ",end="")
        counter=counter+1
    for counter2 in range(rownumber):
        print("#",end="")
        counter2=counter2+1
    print()
    rownumber=rownumber + 1
