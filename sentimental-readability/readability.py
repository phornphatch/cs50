from cs50 import get_string

text = get_string("Text: ")
actual_grade = 0
grade = 0
total_letter = 0
total_comma = 0
total_word = 1  # count by space + 1 ot start with 1
total_sentence = 0  # count by . or !

# find grade
for i in range(len(text)):
    character = text[i]
    # print(character)

    if character == ",":
        total_comma += 1

    if character == ',' or character == ' ':
        total_word += 1
    elif character == "." or character == "!" or character == ":" or character == "?":
        total_sentence += 1
    elif character != "'":
        total_letter += 1

total_word = len(text.split(" "))
L = (total_letter // (total_word - total_comma)) * 100
S = (total_sentence // (total_word - total_comma)) * 100
actual_grade = (0.0588 * L) - (0.296 * S) - 15.8

print(total_sentence // (total_word - total_comma))
print(x)
print(total_word)
print(total_comma)
# print(total_letter)
# print(L)
print(S)
# print(actual_grade)

if actual_grade > 16:
    print("Grade 16+")
elif actual_grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {actual_grade}")
