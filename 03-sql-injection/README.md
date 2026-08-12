# 03 — SQL Injection

## The bug

Both vulnerable endpoints build SQL with Python f-strings instead of
parameterized queries:

```python
query = f"SELECT * FROM users WHERE username = '{username}' AND password = '{password}'"
```

```python
query = f"SELECT id, name, price FROM products WHERE name LIKE '%{q}%'"
```

Whatever the user types gets pasted directly into the SQL text. The
database can't tell the difference between "data" and "code" — if your
input contains SQL syntax (a `'` to close a string, `--` to start a
comment, `UNION SELECT` to append a second query), the database just
executes it.

## Run it

```sh
pip install flask requests
python3 app.py
```

Visit `http://127.0.0.1:5000/`.

## Try it yourself first

### 1. `/login` — authentication bypass

At the login form, you don't know `admin`'s password. But you don't need
it — you need the *query* to evaluate to true. Try username:

```
admin' -- -
```

with any password. The query becomes:

```sql
SELECT * FROM users WHERE username = 'admin' -- -' AND password = 'anything'
```

`--` starts a SQL comment, so everything after it (including the password
check) is ignored. The query just becomes "find the user named admin," and
you're logged in without ever knowing the password.

### 2. `/search` — UNION-based data extraction

The `products` table is the only thing this endpoint is *supposed* to
expose. But there's also a `secrets` table with a flag in it that no page
links to. Search for:

```
nonexistent%' UNION SELECT id, secret, 'x' FROM secrets -- -
```

`UNION SELECT` appends the results of a second query to the first, as long
as both `SELECT`s return the same number of columns. `products` gives 3
columns (`id, name, price`), so our injected `SELECT` also returns 3
(`id, secret, 'x'`) — padding with a dummy value to line up the column
count — and the `secrets` table's contents show up in the results table.

## The exploit script

```sh
python3 app.py &     # start the server
python3 exploit.py
```

Automates both techniques above with `requests`.

## How to fix it

Use parameterized queries — let the database driver handle escaping, never
build SQL with string interpolation:

```python
row = con.execute(
    "SELECT * FROM users WHERE username = ? AND password = ?",
    (username, password),
).fetchone()
```

```python
rows = con.execute(
    "SELECT id, name, price FROM products WHERE name LIKE ?",
    (f"%{q}%",),
).fetchall()
```

With placeholders, `'`, `--`, and `UNION` typed by a user are just
characters in a string value — never SQL syntax. This closes both bugs
with no change to behavior for legitimate input.

Also worth knowing: even with this fixed, storing plaintext passwords
(as this demo does) is its own separate bug — real apps should hash
passwords with something like bcrypt/argon2, never compare them directly.
