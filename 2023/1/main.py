first = ""
last = ""
total = 0

arr = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]


with open('input.txt', 'r') as file:
    for line in file:
        for char in line:
            if char.isdigit():
                first = char
                break
        for char in line.strip()[::-1]:  # Reverse the line and iterate over characters
            if char.isdigit():
                last = char
                break
        final = first + last
        total += int(final)

print(total)

