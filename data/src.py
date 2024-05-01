import requests

# Make a request to https://codedamn-classrooms.github.io/webscraper-python-codedamn-classroom-website/
# Store the result in 'res' variable
res = requests.get(
    'https://www.nairaland.com/magnoliaa')
txt = res.text
status = res.status_code

print(txt, status)
# print the result