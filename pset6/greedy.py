import cs50
quartercount = 0
dimecount = 0
nickelcount = 0
pennycount = 0
total = 0
while True:
    print("How much change is owed")
    amount = cs50.get_float()
    if amount > 0:
        break
amount = round(amount,2)
amount = amount*100
while amount >= 25:
    amount = amount - 25
    quartercount=quartercount + 1
while amount >= 10:
    amount = amount - 10
    dimecount=dimecount + 1
while amount >= 5:
    amount = amount - 5
    nickelcount=nickelcount + 1
while amount >= 1:
    amount = amount - 1
    pennycount=pennycount + 1
total = quartercount+dimecount+nickelcount+pennycount
print("{}".format(total))
