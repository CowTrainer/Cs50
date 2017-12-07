from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)

    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)
    postweet=0
    negtweet=0
    neutweet=0
    counter1=0
    for counter1 in range(len(tweets)):
        score = analyzer.analyze(tweets[counter1])
        if score > 0.0:
            postweet = postweet + 1
        elif score < 0.0:
            negtweet = negtweet + 1
        else:
            neutweet = neutweet + 1
    positive, negative, neutral = postweet, negtweet, neutweet

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
