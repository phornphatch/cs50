import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


def round_total(history):
    history["total"] = round(history["total"], 2)
    return history


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    current_user_id = session["user_id"]
    buy_histories = db.execute(
        "SELECT symbol, name, AVG(price) as price, SUM(total) as total, SUM(shares) as shares FROM buy_histories GROUP BY symbol, name"
    )
    total_buy = db.execute(
        "SELECT SUM(total) as total FROM buy_histories WHERE user_id = ?",
        current_user_id,
    )

    sum_amount = db.execute("SELECT SUM(amount) as amount FROM cash_histories")
    total_added_cash = sum_amount[0]["amount"]
    total = "%.2f" % 10000

    if not total_added_cash:
        total_added_cash = 0

    total_with_deposite = 10000 + total_added_cash

    print(total_buy)

    if total_buy[0]["total"] != None:
        cash = "%.2f" % round((10000 - total_buy[0]["total"]), 2)
        cash_with_deposite = "%.2f" % round(
            (10000 - total_buy[0]["total"] + total_added_cash), 2
        )
    else:
        cash = "%.2f" % round((10000 - sum_amount[0]["amount"]), 2)
        cash_with_deposite = "%.2f" % round(
            (10000 - sum_amount[0]["amount"] + total_added_cash), 2
        )

    return render_template(
        "index.html",
        buy_histories=map(round_total, buy_histories),
        cash=cash,
        total=total,
        cash_with_deposite=cash_with_deposite,
        total_with_deposite=total_with_deposite,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # submitted via post, lookup the stock symbol by calling the lookup function and display the result
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol not empty
        if not symbol:
            return apology("must provide symbol", 400)
        # Ensure shares not empty
        if not shares:
            return apology("must provide shares", 400)
        else:
            if not shares.isnumeric():
                return apology("Invalid shares", 400)
            elif float(shares) < 0:
                return apology("Invalid shares", 400)
            elif not str(shares).replace(".", "").isdigit():
                return apology("Invalid shares", 400)

        current_user_id = session["user_id"]
        result = lookup(symbol)
        if not result:
            return apology("invalid symbol", 400)
        symbol = result["symbol"]
        name = result["name"]
        current_price = result["price"]
        total_price = current_price * float(shares)
        sum_amount = db.execute("SELECT SUM(amount) as amount FROM cash_histories")
        total_added_cash = sum_amount[0]["amount"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", current_user_id)

        # real valid cash
        valid_cash = cash[0]["cash"] + total_added_cash

        if valid_cash < total_price:
            return apology("can not afford", 400)

        if result:
            # Add one or more new tables to finance.db via which to keep track of the purchase.
            db.execute(
                "INSERT INTO buy_histories (symbol, name, shares, price, total, user_id) VALUES(?, ?, ?, ?, ?, ?)",
                symbol,
                name,
                shares,
                current_price,
                total_price,
                current_user_id,
            )
            return redirect("/")
        else:
            return apology("invalid symbol", 400)

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    history = db.execute("SELECT * FROM buy_histories")
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # submitted via post, lookup the stock symbol by calling the lookup function and display the result
        symbol = request.form.get("symbol")

        # Ensure symbol not empty
        if not symbol:
            return apology("must provide symbol", 400)

        result = lookup(symbol)

        if result:
            return render_template(
                "quoted.html", symbol=result["symbol"], price=result["price"]
            )
        else:
            return apology("invalid symbol", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username not empty
        if not username:
            return apology("must provide username", 400)

        # Ensure password not empty
        elif not password:
            return apology("must provide password", 400)

        # Ensure confirmation not empty
        elif not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure confirmation not empty
        elif not password == confirmation:
            return apology("passwords do not match", 400)

            # Ensure username already exist
        elif username:
            checkUsername = db.execute(
                "SELECT username FROM users WHERE username = ?", username
            )
            if checkUsername:
                return apology("username already exist", 400)
            else:
                print("Unique usename")

        db.execute(
            "INSERT INTO users (username, hash) VALUES(?, ?)",
            username,
            generate_password_hash(password),
        )
        print("Successful Register !")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        shares_request = request.form.get("shares")
        selected_symbol = request.form.get("symbol")

        if not selected_symbol:
            return apology("must provide symbol", 400)

        # Ensure shares not empty
        if not shares_request:
            return apology("must provide shares", 400)
        else:
            if not shares_request.isnumeric():
                return apology("Invalid shares", 400)
            elif float(shares_request) < 0:
                return apology("Invalid shares", 400)
            elif not str(shares_request).replace(".", "").isdigit():
                return apology("Invalid shares", 400)

        # sum shares of selected symbol
        sum_shares = db.execute(
            "SELECT SUM(shares) as shares FROM buy_histories WHERE symbol = ?",
            selected_symbol,
        )
        valid_shares = sum_shares[0]["shares"]
        print(valid_shares)

        if valid_shares < float(shares_request):
            return apology("Not enough shares", 400)

        remaining_shares = valid_shares - float(shares_request)
        print("Successful Register !")
        # update shares in buy_histories
        # db.execute(
        #   "UPDATE buy_histories SET shares = ? WHERE symbol = ?", remaining_shares, selected_symbol)

        current_user_id = session["user_id"]
        name = db.execute(
            "SELECT name FROM buy_histories WHERE symbol = ?", selected_symbol
        )
        selected_name = name[0]["name"]
        current_price = db.execute(
            "SELECT price FROM buy_histories WHERE symbol = ?", selected_symbol
        )
        current_price_sell = current_price[0]["price"]
        total_price_sell = current_price_sell * float(shares_request)
        db.execute(
            "INSERT INTO buy_histories (symbol, name, shares, price, total, user_id) VALUES(?, ?, ? * -1, ?, ? * -1, ?)",
            selected_symbol,
            selected_name,
            shares_request,
            current_price_sell,
            total_price_sell,
            current_user_id,
        )
        return redirect("/")

    else:
        symbols = db.execute("SELECT symbol FROM buy_histories GROUP BY symbol")
        return render_template("sell.html", symbols=symbols)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        amount = request.form.get("amount")
        # Ensure amount not empty
        if not amount:
            return apology("must provide amount", 400)

        current_user_id = session["user_id"]
        db.execute(
            "INSERT INTO cash_histories (amount, user_id) VALUES(?, ?)",
            amount,
            current_user_id,
        )
        return redirect("/")
    else:
        sum_amount = db.execute("SELECT SUM(amount) as amount FROM cash_histories")
        if sum_amount[0]["amount"]:
            total_deposite = 10000 + sum_amount[0]["amount"]
        else:
            total_deposite = 10000
        return render_template("add_cash.html", total_deposite=total_deposite)
