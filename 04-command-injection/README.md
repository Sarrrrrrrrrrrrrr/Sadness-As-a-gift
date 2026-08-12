# 04 — OS Command Injection

## The bug

```python
cmd = f"ping -c 1 -W 1 {host}"
result = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=5)
```

`shell=True` runs `cmd` through `/bin/sh -c "..."`, exactly like typing it
in a terminal. Building that string with an f-string means whatever the
user types becomes part of the shell command line — and shells treat
characters like `;`, `|`, `&&`, and `` ` `` `` as *syntax*, not data. Any of
those in `host` lets you chain on an arbitrary second command.

There's a `flag.txt` in this directory that the app never reads or serves.
Goal: read it anyway, through the ping form.

## Run it

```sh
pip install flask requests
python3 app.py
```

Visit `http://127.0.0.1:5001/ping`. (If your machine doesn't have a `ping`
binary, the ping part of the command will just fail — the injected part
still runs, since a shell runs each `;`-separated command independently.)

## Try it yourself first

In the Host field, instead of an IP, try:

```
127.0.0.1; cat flag.txt
```

The shell sees this as *two* commands: `ping -c 1 -W 1 127.0.0.1` and then
`cat flag.txt`, run one after another. The output of both gets captured
and shown back to you on the page — including the flag.

Other separators that work here for the same reason (`sh -c` treats them
as command boundaries or substitutions): `&&`, `|`, `` `cat flag.txt` ``,
`$(cat flag.txt)`.

## The exploit script

```sh
python3 app.py &
python3 exploit.py
```

## How to fix it

The real fix is to **not invoke a shell at all**. Pass the command as a
list of arguments and drop `shell=True`:

```python
result = subprocess.run(
    ["ping", "-c", "1", "-W", "1", host],
    capture_output=True, text=True, timeout=5,
)
```

Without a shell in between, `host` is passed as a single literal argument
to `ping` — no matter what characters it contains, there's no shell left
to interpret `;`, `|`, or `` ` `` as syntax.

If you truly need shell features, at minimum validate `host` against a
strict allowlist (e.g. a hostname/IP regex) *before* using it, and prefer
`shlex.quote()` as defense in depth — but "don't use `shell=True` with
untrusted input" is the fix that actually closes the bug class.
