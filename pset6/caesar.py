import cs50
import sys


def main():
    if len(sys.argv) != 2:
        print("Try Again")
        sys.exit(0)
    x = cs50.get_string()
    length = len(x)
    k = int(sys.argv[1])
    higher = 0
    lower = 0
    count = 0
    count1 = 0
    count2 = 0
    for count in range(length):
        y=[0 for count in range(length)]
        y[count]=0
        count = count + 1
    for count1 in range(length):
        higher = (ord(x[count1])+k-65)%26
        lower = (ord(x[count1])+k-97)%26
        if(x[count1].isalpha()):
            if(x[count1].islower()):
                y[count1]= lower+97
            if(x[count1].isupper()):
                y[count1]= higher+65
        count1 = count1 + 1
    for count2 in range(length):
        y[count2] = chr(y[count2])
        count2 = count2 + 1
    print("ciphertext: ")
    a = "".join(y)
    print(a)
if __name__ == "__main__":
    main()
            
        