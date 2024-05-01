import requests
from bs4 import BeautifulSoup

response = requests.get("https://www.nairaland.com/magnoliaa")
soup = BeautifulSoup(response.text, "html.parser")

post = soup.find_all('div',attrs={'data-post-type-id': 1})
for post in post:
    print(post.find('a', attrs={'class': 's-link'}).get('href'))