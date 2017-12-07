from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from decimal import Decimal

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    result = db.execute("SELECT symbol,shares FROM stocks WHERE id = :currentid AND shares != 0",currentid = session["user_id"])
    list = [None]*len(result)
    list2 = [None]*len(result)
    cash = db.execute("SELECT cash FROM users WHERE id = :currentid",currentid=session["user_id"])
    total = cash[0]["cash"]
    for count in range(len(result)):
        x = lookup(result[count]["symbol"])
        list[count] = round(x["price"],2)
        total = x["price"] * result[count]["shares"] + total 
    for count1 in range(len(result)):
        x = lookup(result[count1]["symbol"])
        list2[count1] = x["name"]
    for count1 in range(len(result)):
        x = lookup(result[count1]["symbol"])
    return render_template("Index.html",result=result,list=list,cash = cash,list2 = list2,total2 = total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        bsymbol = request.form.get("bsymbol")
        shares = request.form.get("shares")
        x = lookup(bsymbol)
        if not bsymbol:
            return apology("Missing symbol")
        elif not shares:
            return apology("Missing shares")
        elif shares.isdigit() == False:
            return apology("Invaild shares")
        elif not x:
            return apology("invalid stock")
        else:
            result = db.execute("SELECT cash FROM users WHERE id = :sessionid", sessionid=session["user_id"])
            x = lookup(bsymbol)
            y = x["price"]
            totalprice = round(y * int(shares),2)
            if not x:
                return apology("Invalid symbol")
            if result[0]['cash']<totalprice:
                return apology("Insufficient Funds")
            q = db.execute("SELECT * FROM stocks WHERE id = :currentid AND symbol = :currentsymbol",currentid = session["user_id"],currentsymbol = bsymbol)
            if not q:
                db.execute("INSERT INTO stocks(id,symbol,shares) VALUES(:id, :symbol, :shares)",id=session["user_id"],symbol = bsymbol
                ,shares = shares)
            else:
                db.execute("UPDATE stocks SET shares = shares + :plusshares WHERE symbol = :currentsymbol",plusshares=int(shares),
                currentsymbol = bsymbol)
            z=db.execute("INSERT INTO history(id,symbol,shares,price) VALUES(:id, :symbol, :shares, :price)",
            id = session["user_id"], symbol = bsymbol, shares = request.form.get("shares"), price = totalprice)
            db.execute("UPDATE users SET cash = cash - :price WHERE id = :currentid",price = totalprice, currentid = session["user_id"])
            return index()

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    result = db.execute("SELECT * FROM history WHERE id = :currentid",currentid = session["user_id"])
    return render_template("history.html",result = result)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Missing symbol")
        info = lookup(symbol)
        if not info:
            return apology("Invalid Symbol")
        x = usd(info["price"])
        return render_template("quote2.html",name = info["name"],symbol = info["symbol"],price = x)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        x = request.form.get("username")
        y = request.form.get("password")
        z = request.form.get("passwordcon")
        passwordh= pwd_context.encrypt(y)
        if not x or not y or not z:
            return apology("Please fill in all required fields")
        elif y != z:
            return apology("Passwords do not match")
        else:
            result = db.execute("INSERT INTO users (username,hash) VALUES(:username, :hash)",
            username = request.form.get("username"),
            hash = passwordh)
            if not result:
                return apology("Username already taken")
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        ssymbol = request.form.get("ssymbol")
        sshares = request.form.get("sshares")
        total = 0
        if not ssymbol:
            return apology("Missing symbol")
        elif not sshares:
            return apology("Missing shares")
        elif sshares.isdigit() == False:
            return apology("Invaild shares")
        x = lookup(ssymbol)
        if not x:
            return apology("Invalid symbol")
        y = db.execute("SELECT shares FROM history WHERE id = :currentid AND symbol = :currentsymbol",currentid = session["user_id"]
        ,currentsymbol = ssymbol)
        total = db.execute("SELECT shares FROM stocks WHERE symbol = :currentsymbol",currentsymbol = ssymbol)
        if not total:
            return apology("Stock not owned")
        if total[0]["shares"] < int(sshares):
            return apology("You dont own enough stock")
        totalprice = round(int(sshares)*x["price"],2)
        db.execute("UPDATE stocks SET shares = shares - :soldshares",soldshares = int(sshares))
        db.execute("UPDATE users SET cash = cash + :value WHERE id = :currentid",
        value=x["price"]*int(sshares),currentid = session["user_id"])
        db.execute("INSERT INTO history(id,symbol,shares,price) VALUES(:id, :symbol, :shares, :price)",
        id = session["user_id"], symbol = ssymbol, shares= -int(sshares), price = totalprice)
        return index();
    else:
        return render_template("sell.html")
@app.route("/cash", methods=["GET","POST"])
@login_required
def cash():
    if request.method == "POST":
        x = request.form.get("amount")
        z = 0
        try:
            a = float(x)
        except ValueError:
            return apology("Invalid amount")
        for countt in range(len(x)):
            if x[countt] == ".":
                z = z + 1
        if z == 1:
            e = len(x.split(".")[1])
            if e > 2:
                return apology("Too many decimal place")
            x = float(x)
            x = float("{:,.2f}".format(x))
        else:
            x = int(x)
        if x == 0.00 or x<0:
            return apology("You cannot take 0 or negative money")
        y = db.execute("UPDATE users SET cash = cash + :value WHERE id = :currentid",value = x,currentid = session["user_id"])
        return index()
    else:
        return render_template("cash.html")
@app.route("/password", methods=["GET","POST"])
@login_required
def password():
    if request.method == "POST":
        x = request.form.get("password")
        if not x:
            return apology("Please fill in the required field")
        x = pwd_context.encrypt(x)
        y = db.execute("UPDATE users SET hash = :hash WHERE id = :currentid",hash = x,currentid = session["user_id"])
        return index()
    else:
        return render_template("Password.html")
    
