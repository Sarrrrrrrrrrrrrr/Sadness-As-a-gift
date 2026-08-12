#!/usr/bin/env python3
"""
Deliberately vulnerable Flask app: a "network diagnostics" page that pings
a host you give it. Built to be broken -- local learning only, do not
deploy.
"""
import os
import subprocess

from flask import Flask, request, render_template_string

app = Flask(__name__)

PAGE = """
<h2>Network Diagnostics</h2>
<form method="get">
  Host: <input name="host" value="{{ host }}">
  <button type="submit">Ping</button>
</form>
<pre>{{ output }}</pre>
"""


@app.route("/")
def index():
    return '<a href="/ping">Ping tool</a>'


@app.route("/ping")
def ping():
    host = request.args.get("host", "")
    output = ""
    if host:
        # VULNERABLE: user input concatenated directly into a shell command
        cmd = f"ping -c 1 -W 1 {host}"
        try:
            result = subprocess.run(
                cmd, shell=True, capture_output=True, text=True, timeout=5
            )
            output = result.stdout + result.stderr
        except subprocess.TimeoutExpired:
            output = "(timed out)"
    return render_template_string(PAGE, host=host, output=output)


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5001)
