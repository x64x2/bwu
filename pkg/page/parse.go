package page

import (
	"fmt"
	"io"
	"net/url"
	"path"
	"strings"

	"golang.org/x/net/html"
)

// implements Parser interface which returns single domain URL for the given HTML page
type MultipleSitePages struct {
	baseURL string
}

//returns
func NewMultipleSite(startURL *url.URL) *MultipleSitePages {
	return &MultipleSitePages{
		baseURL: fmt.Sprintf("%s://%s", startURL.Scheme, startURL.Host),
	}
}

//Parse HTML page body to a set of locla domain's link on that page. It returns uniq absolute URLs without local anhcors
func (l *MultipleSitePages) Parse(input io.Reader) []string {
	links := []string{}
	page := html.NewTokenizer(input)
	for {
		tokenType := page.Next()
		if tokenType == html.ErrorToken {
			return uniq(links)
		}
		token := page.Token()
		if tokenType == html.StartTagToken && token.DataAtom.String() == "a" {
			for _, attr := range token.Attr {
				if attr.Key == "href" {
					href := trimHash(attr.Val)
					if link, isLocal := l.localLink(href); isLocal {
						links = append(links, link)
					}
				}
			}
		}
	}
}

func (l *MultipleSitePages) localLink(href string) (string, bool) {
	if strings.HasPrefix(href, l.baseURL) {
		return href, true
	}
	if strings.HasPrefix(href, "/") {
		u, err := url.Parse(l.baseURL)
		if err != nil {
			panic(err)
		}
		u.Path = path.Join(u.Path, href)
		return u.String(), true

	}
	return "", false
}

func uniq(input []string) []string {
	existing := make(map[string]struct{}, len(input))
	response := make([]string, 0, len(input))
	for _, link := range input {
		if _, exists := existing[link]; exists {
			continue
		}
		existing[link] = struct{}{}
		response = append(response, link)
	}
	return response
}

func trimHash(input string) string {
	i := strings.Index(input, "#")
	if i > 0 {
		return input[:i]
	}
	return input
}
