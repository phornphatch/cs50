menus = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}

def main():
    total = 0

    while True:
        try:
            item = input("Item: ").title()
            # print(item)
            # print(menus)
            if item in menus:
                # print(f"{item}")
                total += menus.get(item)
                print(f"${total}")
            else:
                print("no item in menu")
        except EOFError:
            print("\n")
            return False
            sys.exit(1)
main()