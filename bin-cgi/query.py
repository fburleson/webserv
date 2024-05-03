#!/usr/bin/env python3
import cgi

# Required for CGI scripts to work properly
print("Content-Type: text/html\n\n")

# Get query string data
form = cgi.FieldStorage()

# Extract parameters from query string
name = form.getvalue('name', 'Guest')
age = form.getvalue('age', 'Unknown')

# HTML content of the page
html_content = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI with Query Strings</title>
    <style>
        /* CSS styles can go here */
        body {{
            font-family: Arial, sans-serif;
            background-color: #e6ecff;
            margin: 0;
            padding: 0;
        }}
        .container {{
            max-width: 800px;
            margin: 50px auto;
            padding: 20px;
            background-color: #ffffff;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }}
        h1 {{
            color: #333;
            text-align: center;
            border-bottom: 2px solid #007bff;
            padding-bottom: 10px;
        }}
        p {{
            color: #555;
            line-height: 1.6;
        }}
        .footer {{
            text-align: center;
            margin-top: 20px;
            color: #777;
            font-size: 14px;
        }}
    </style>
</head>
<body>
  <div class="container">
    <h1>Hello, {name}!</h1>
    <p>You are {age} years old.</p>
    <p>Return to <a href="/">homepage</a>.</p>
  </div>
  <p class="footer">This page was generated using CGI and query strings.</p>
</body>
</html>
""".format(name=name, age=age)

# Print the HTML content
print(html_content)

