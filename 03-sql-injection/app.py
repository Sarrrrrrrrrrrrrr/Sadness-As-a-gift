#!/usr/bin/env python3
"""
Deliberately vulnerable Flask + SQLite app for practicing SQL injection.

Two vulnerable endpoints:
  - POST /login   : builds its query with raw string formatting -> auth bypass
  - GET  /search   : builds its query with raw string formatting -> UNION-based
                      extraction of data from a table the endpoint never
                      intended to expose

DO NOT deploy this. It's built to be broken, for local learning only.
"""
import os
import sqlite3

from flask import Flask, request, render_template_string

DB_PATH = os.path.join(os.path.dirname(__file__), "app.db")

app = Flask(__name__)


def init_db():
    if os.path.exists(DB_PATH):
        os.remove(DB_PATH)
    con = sqlite3.connect(DB_PATH)
    cur = con.cursor()
    cur.execute("CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT)")
    cur.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, price TEXT)")
    cur.execute("CREATE TABLE secrets (id INTEGER PRIMARY KEY, secret TEXT)")
    cur.executemany(
        "INSERT INTO users (username, password) VALUES (?, ?)",
        [("admin", "correct-horse-battery-staple"), ("guest", "guest123")],
    )
    cur.executemany(
        "INSERT INTO products (name, price) VALUES (?, ?)",
        [("Rubber Duck", "$5"), ("USB Cable", "$8"), ("Mechanical Keyboard", "$70")],
    )
    cur.execute("INSERT INTO secrets (secret) VALUES (?)", ("FLAG{sql1_uni0n_s3l3ct_ftw}",))
    con.commit()
    con.close()


def get_db():
    con = sqlite3.connect(DB_PATH)
    con.row_factory = sqlite3.Row
    return con


LOGIN_PAGE = """
<h2>Login</h2>
<form method="post">
  Username: <input name="username"><br>
  Password: <input name="password" type="password"><br>
  <button type="submit">Log in</button>
</form>
{% if result %}<pre>{{ result }}</pre>{% endif %}
"""

SEARCH_PAGE = """
<h2>Product Search</h2>
<form method="get">
  <input name="q" value="{{ q }}">
  <button type="submit">Search</button>
</form>
<table border="1" cellpadding="4">
{% for row in rows %}
  <tr>{% for col in row %}<td>{{ col }}</td>{% endfor %}</tr>
{% endfor %}
</table>
{% if error %}<pre>{{ error }}</pre>{% endif %}
"""


@app.route("/")
def index():
    return '<a href="/login">Login</a> | <a href="/search">Search</a>'


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template_string(LOGIN_PAGE, result=None)

    username = request.form.get("username", "")
    password = request.form.get("password", "")

    # VULNERABLE: raw string formatting instead of parameterized query
    query = f"SELECT * FROM users WHERE username = '{username}' AND password = '{password}'"

    con = get_db()
    try:
        row = con.execute(query).fetchone()
    except sqlite3.OperationalError as e:
        return render_template_string(LOGIN_PAGE, result=f"SQL error: {e}\nQuery was: {query}")
    finally:
        con.close()

    if row:
        return render_template_string(
            LOGIN_PAGE, result=f"Welcome, {row['username']}! (query: {query})"
        )
    return render_template_string(LOGIN_PAGE, result=f"Invalid credentials.\nQuery was: {query}")


@app.route("/search")
def search():
    q = request.args.get("q", "")

    # VULNERABLE: raw string formatting instead of parameterized query
    query = f"SELECT id, name, price FROM products WHERE name LIKE '%{q}%'"

    con = get_db()
    error = None
    rows = []
    try:
        rows = con.execute(query).fetchall()
    except sqlite3.OperationalError as e:
        error = f"SQL error: {e}\nQuery was: {query}"
    finally:
        con.close()

    return render_template_string(SEARCH_PAGE, q=q, rows=rows, error=error)


if __name__ == "__main__":
    init_db()
    app.run(host="127.0.0.1", port=5000)
