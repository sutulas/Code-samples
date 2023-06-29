import random

def filestuff(filename):
    file = open(filename, "r")
    wlist = file.read()
    file.close()
    cword = []
    cword = wlist.split(r",")
    for i in range(len(cword)-1):
        cword[i] = cword[i][1]+cword[i][2]+cword[i][3]+cword[i][4]+cword[i][5]
    return (cword)

x = 'y'
while (x == 'y' or x == 'Y'):
    cleanword = filestuff("realword.txt")

    cleanlist = filestuff("answerwords.txt")

    random_number = random.randint(0, len(cleanlist)-1)

    word = cleanlist[random_number]
    guess = ''
    out = ''
    counter = 0
    guessletters = []
    rletters = "abcdefghijklmnopqrstuvwxyz"
    print("Enter a five letter word")
    while (guess != word and counter < 6):
        c = ['_' for i in range(5)]
        out = ""
        guess = str(input())
        if guess in cleanword or guess in cleanlist and len(guess) == 5:

            letter_count = [0 for i in range(5)]
            flags = [0 for i in range(5)]
            
            for i in range(5):
                if guess[i] not in guessletters:
                    guessletters.append(guess[i])

                flag = 0
                
                for j in range(5):
                    if guess[j] == word[j] and flags[j] ==0:
                        c[j] = guess[j]
                        flags[j] = 1
                for j in range(5):
                    if guess[i] == word[j] and i != j and flags[j] == 0 and c[i] == '_':
                        c[i] = "*"
                        flags[j] = 1
            for i in range(5):
                out += c[i]
            counter += 1
            print(out)
            guessletters.sort()
            letters = ""
            rlttr = ""
            for i in guessletters:
                letters = letters + i
            for i in rletters:
                if i not in guessletters:
                    rlttr = rlttr + i
            if guess != word and counter < 6:
                print("Enter a five letter word\t\t Guessed letters: ",
                    letters, "\t\t\tRemaining letters: ", rlttr)
        else:
            print('Please enter a valid word')

    if guess.__eq__(word):
        print("You win!")
        print("Play Again? (y/n)")
        x = input()
    else:
        print(word)
        print("You lose")
        print("Play Again? (y/n)")
        x = input()

