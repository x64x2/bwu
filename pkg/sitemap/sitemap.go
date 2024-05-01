package sitemap

import (
	"github.com/sheerun/queue"
)

//NewSiteMap accepts URL's storage backend and return an object to manage incoming flow and maintain a query
func NewSiteMap(b Backend) *SiteMap {
	return &SiteMap{
		backend: b,
		input:   make(chan string),
		done:    make(chan struct{}),
		queue:   queue.New(),
	}
}

//SiteMap implements Queue interface, which allows to add URLs and update unvisited URL query
type SiteMap struct {
	backend Backend
	input   chan string
	done    chan struct{}
	queue   *queue.Queue
}

//List iterate over backend's list of visited URLS and push individual URL to chanel, returns chanel to read
func (s *SiteMap) List() <-chan string {
	out := make(chan string)
	go func() {
		var partial []string
		for hasMore := true; hasMore; { //run at least once
			partial, hasMore = s.backend.Next()
			for _, url := range partial {
				out <- url
			}
		}
		close(out)
	}()
	return out
}

//Queue returns channel to receive a flow of incoming URLs
func (s *SiteMap) Queue() chan<- string {
	return s.input
}

//Run is a background function which receives new URL pass each of them to backend and add new URLs to output
func (s *SiteMap) Run() {
	for {
		select {
		case pretender, ok := <-s.input:
			if ok {
				if isNew := s.backend.Append(pretender); isNew {
					s.queue.Append(pretender)
				}
			}
		case <-s.done:
			return
		}
	}
}

//Done stops SiteMaps queuing runtine
func (s *SiteMap) Done() {
	close(s.done)
}

//Pop extracts first element from queue
func (s *SiteMap) Pop() (string, bool) {
	select {
	case <-s.queue.NotEmpty:
		e := s.queue.Pop()
		if e == nil {
			return "", false
		}
		if val, ok := e.(string); ok {
			return val, true
		}
	default:
	}
	return "", false
}
