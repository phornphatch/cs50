from cs50 import get_int

credit_card_number = get_int("Number: ")
credit_card_string = str(credit_card_number)
number_length = len(credit_card_string)
sum_even = 0
sum_odd = 0
sum = 0
two_first_number = int(credit_card_string[0:2])
first_number = int(credit_card_string[0])
start = 0
end_even = 0
end_odd = 0

if number_length % 2 == 0:
    end_even = -1
    end_odd = 0
else:
    end_odd = -1
    end_even = 0

for i in range(number_length - 2, end_even, -2):
    multiply = int(credit_card_string[i]) * 2
    # print(int(credit_card_string[i]))
    # print(multiply)
    if multiply < 10:
        sum_even += multiply
    else:
        multiply = str(multiply)
        sum_even += int(multiply[0]) + int(multiply[1])
    # print(sum_even)

for j in range(number_length - 1, end_odd, -2):
    # print(int(credit_card_string[j]))
    sum_odd += int(credit_card_string[j])
    # print(sum_odd)

sum = sum_even + sum_odd
print(sum)

# Check checksum is valid according to Luhn's Algorithm or not
if sum % 10 != 0:
    print("INVALID")
# Check card length and two first number
elif number_length == 15 and (two_first_number == 34 or two_first_number == 37):
    print("AMEX")
elif number_length == 16 and (50 < two_first_number < 56):
    print("MASTERCARD")
elif number_length == 16 and first_number == 4:
    print("VISA")
elif number_length == 13 and first_number == 4:
    print("VISA")
else:
    print("INVALID")
