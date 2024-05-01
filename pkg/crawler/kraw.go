package crawler

import (
	"crypto/tls"
	"net/http"
	"time"

	"gitlab.com/iscander/crawler/pkg/page"
)

const defaultUserAgent = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"

//NewBot return a bot to collect pages
func NewBot(userAgent string, parser page.Parser, out chan<- string, client *http.Client) *Bot {
	ua := defaultUserAgent
	httpClient := newDefaultClient()
	if userAgent != "" {
		ua = userAgent
	}
	if client != nil {
		httpClient = client
	}
	return &Bot{
		parser:    parser,
		out:       out,
		client:    httpClient,
		userAgent: ua,
	}
}

//Bot a type to collect information from a web page
type Bot struct {
	userAgent string
	parser    page.Parser
	client    *http.Client
	out       chan<- string
}

//Get request a page for given URL and output links found to a channel
func (b *Bot) Get(url string) {
	//println("fetching", url) //uncomment for debugging
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("User-Agent", b.userAgent)
	resp, err := b.client.Do(req)
	if err != nil {
		return
	}
	defer resp.Body.Close()
	//TODO process resp.Status
	links := b.parser.Parse(resp.Body)
	for _, link := range links {
		b.out <- link
	}
}

//newDefaultClient returns an HTTP client which verifies HTTPS certificates and has 15 sec timeout
func newDefaultClient() *http.Client {
	transport := &http.Transport{
		TLSClientConfig: &tls.Config{
			InsecureSkipVerify: false,
		},
	}
	return &http.Client{
		Transport: transport,
		Timeout:   15 * time.Second,
	}
}
