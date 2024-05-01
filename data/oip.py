import requests
from bs4 import BeautifulSoup

# Make a request
page = requests.get(
    "https://www.nairaland.com/magnoliaa")
soup = BeautifulSoup(page.content, 'html.parser')

# Extract title of page
page_title = soup.title.text

# Extract body of page
page_body = soup.body

# Extract head of page
page_head = soup.head

# print the result
print(page_body, page_head)