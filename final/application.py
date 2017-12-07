from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
import re
import http.client, urllib.request, urllib.parse, urllib.error, base64

from helpers import *

app = Flask(__name__)

if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response
        
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///final.db")
@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        company_name = request.form.get("company_name")
        x = db.execute("SELECT company_name FROM data WHERE company_name = :company_name", company_name = company_name )
        if x:
            return("This company_name is already taken")
        password = request.form.get("password")
        z = request.form.get("location")
        y = request.form['industry']
        email = request.form.get("email")
        money = request.form.get("money")
        password = pwd_context.encrypt(password)
        ss = db.execute("INSERT INTO data(company_name,hash,address,company_type,email,money) VALUES(:company_name,:password,:address,:company_type,:email,:money)",company_name = company_name,
        password = password,address = z,company_type = y,email = email,money = money)
        rows = db.execute("SELECT * FROM data WHERE company_name = :company_name", company_name=request.form.get("company_name"))
        session["user_id"] = rows[0]["id"]
        return render_template("product.html")
    else:
        return render_template("register.html")
@app.route("/login", methods=["GET", "POST"])
def login():
    session.clear()

    if request.method == "POST":

        if not request.form.get("email"):
            return("must provide email")

        elif not request.form.get("password"):
            return("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM data WHERE email = :email", email=request.form.get("email"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return ("invalid email and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("product"))
        
    else:
        return render_template("login.html")
@app.route("/product", methods=["GET", "POST"])
@login_required
def product():
    x = db.execute("SELECT * FROM products WHERE id = :id",id = session["user_id"])
    return render_template("product.html",result = x)
    
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))
@app.route("/product_redirect", methods=["GET", "POST"])
@login_required
def product_redirect():
    product_name = request.form.get("name")
    session["oldproduct_name"] = product_name
    return redirect(url_for("product_edit"))
@app.route("/more_redirect", methods=["GET", "POST"])
@login_required
def more_redirect():
    product_name = request.form.get("product")
    session["oldproduct_name"] = product_name
    return redirect(url_for("more"))
@app.route("/product_edit", methods=["GET", "POST"])
@login_required
def product_edit():
    if request.method == "POST":
        name = request.form.get("name")
        cost = request.form.get("cost")
        list = request.form.get("list")
        qty = request.form.get("qty")
        type = request.form['product_type']
        oldproduct_name = session["oldproduct_name"]
        x = db.execute("UPDATE products SET product_name = :product_name , type = :type , cost = :cost , list = :list, Quantity = :qty WHERE product_name = :oldproduct_name",
        product_name = name, type = type,cost = cost,list = list,qty = qty,oldproduct_name = oldproduct_name  )    
        return product()
    else:
        oldproduct_name = session["oldproduct_name"]
        y = db.execute("SELECT * FROM products WHERE product_name = :product_name",product_name = oldproduct_name)
        return render_template("product_edit.html",x = y)
@app.route("/product_new", methods=["GET", "POST"])
@login_required
def product_new():
    if request.method == "POST":
        name = request.form.get("name")
        cost = request.form.get("cost")
        list = request.form.get("list")
        type = request.form['product_type']
        x = db.execute("INSERT INTO products(id,product_name,type,cost,list,Quantity) VALUES(:currentid,:product_name,:type,:cost,:list,:qty)",
        currentid = session["user_id"],product_name = name, type = type,cost = cost,list = list,qty = 0)    
        return product()
    else:
        return render_template("product_new.html")
@app.route("/order", methods=["GET", "POST"])
@login_required
def order():
    if request.method == "POST":
        product_name = request.form["product_name"]
        z = request.form.get("quantity")
        x = db.execute("SELECT * FROM products WHERE product_name = :product_name",product_name = product_name)
        y = db.execute("UPDATE products SET Quantity = Quantity - :orderq WHERE product_name = :product_name",orderq = int(z),product_name = product_name)
        db.execute("UPDATE data SET money = money + :money WHERE id = :id",id = session["user_id"],money = x[0]["list"] * int(z))
        return render_template("order.html",products = x)
    else:
        y = db.execute("SELECT product_name FROM products WHERE id = :id",id = session["user_id"])
        return render_template("order.html",products = y)
@app.route("/more", methods=["GET", "POST"])
@login_required
def more():
    if request.method == "POST":
        y = session["oldproduct_name"]
        z = request.form.get("num")
        q = db.execute("SELECT * FROM products WHERE product_name = :product_name",product_name = y)
        newquantity = q[0]["Quantity"] + int(z)
        amount = q[0]["cost"] * int(z)
        x = db.execute("UPDATE Products SET Quantity = :newquantity WHERE product_name = :product_name",newquantity = newquantity,product_name = y)
        db.execute("UPDATE data SET money = money - :amount WHERE id = :id",amount = amount,id = session["user_id"])
        return product()
    else:
        return render_template("more.html")
@app.route("/", methods=["GET", "POST"])
@login_required
def info():
        y = db.execute("SELECT money FROM data WHERE id = :id",id = session["user_id"])
        return render_template("index.html",x = y)
@app.route("/news", methods=["GET", "POST"])
@login_required
def news():
    category = db.execute("SELECT company_type FROM data WHERE id = :currentid",currentid = session["user_id"])
    x = lookup(category[0]["company_type"])
    return render_template("news.html",news = x,industry = category[0]["company_type"])


    
