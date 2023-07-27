import csv

books = []

# Add books to your shelf by reading from books.csv
with open("books.csv") as file: # หมายความว่า open csv นั้น แล้วเรียกว่า file
    file_reader = csv.DictReader(file)
    for book in file_reader:
        books.append(book) # append คือ adds an item to the end of the list.

# Print titles of books
for book in books:
    print(book["title"])