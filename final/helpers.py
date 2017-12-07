import csv
import urllib.request
import feedparser
import urllib.parse

from flask import redirect, render_template, request, session, url_for
from functools import wraps

def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("register", next=request.url))
        return f(*args, **kwargs)
    return decorated_function

def lookup(word):

    # get feed from Google
    feed = feedparser.parse("http://news.google.com/news?output=rss&q="+word)

    # if no items in feed, get feed from Onion
    if not feed["items"]:
        feed = feedparser.parse("http://www.theonion.com/feeds/rss")

    # cache results
    lookup.cache[word] = [{"link": item["link"], "title": item["title"]} for item in feed["items"]]

    # return results
    return lookup.cache[word]

# initialize cache
lookup.cache = {}
