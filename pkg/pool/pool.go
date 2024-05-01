package pool

import (
	"sync"
	"time"
)

//NewPool returns initialised concurrent workers pool
//it accepts:
//	concurency: limit maximum page fetchers routines running simultaneously
//	timeout: limit waitime for new URL to process.
//			If there is no new URL delivered during timeout and no workers running gently interrupt the processing loop.
//	queue provides a channel which fetchers uses to report collected URLs
//	pop: a function to return new links for fetching
//	spawn: a function which spawn new instance of page fetcher(crawler.Worker)
func NewPool(
	concurency int,
	timeout time.Duration,
	queue chan<- string,
	pop func() (string, bool),
	spawn Spawn,
) *ConcurrentWorkers {
	p := &ConcurrentWorkers{
		concurency: concurency,
		queue:      queue,
		pop:        pop,
		timeout:    timeout,
		done:       make(chan struct{}),
	}
	p.makeWorkers(spawn)
	return p
}

//ConcurrentWorkers implements concurrent workers pool to run page fetching simultaneously
type ConcurrentWorkers struct {
	concurency int
	timeout    time.Duration
	workers    chan Worker
	pop        func() (string, bool)
	queue      chan<- string
	done       chan struct{}
	stoping    bool
	sync.Mutex
}

//Run start a pool processing routine ussualy should be a dedicated go-routine
func (p *ConcurrentWorkers) Run() {
	input := make(chan string)
	go func() {
		defer close(input)
		for {
			if p.stoping {
				return
			}
			if newURL, ok := p.pop(); ok && !p.stoping {
				input <- newURL
			}
		}
	}()
	for {
		select {
		case <-time.After(p.timeout):
			p.interrupt()
		case <-p.done:
			p.stop()
			return
		case newURL, ok := <-input:
			if ok {
				go p.process(newURL)
			}
		}
	}
}

//Done an external method to call to force stop pool processing
func (p *ConcurrentWorkers) Done() {
	p.Lock()
	p.stoping = true
	close(p.done) //start stoping procedure
	p.Unlock()
}

func (p *ConcurrentWorkers) interrupt() {
	if len(p.workers) < p.concurency { //have a number of still running workers
		return
	}
	p.Done()
}

func (p *ConcurrentWorkers) process(url string) {
	if !p.stoping {
		c := <-p.workers
		c.Get(url)
		p.workers <- c
	}
}

func (p *ConcurrentWorkers) stop() {
	p.Lock()
	close(p.workers)
	p.Unlock()
}

func (p *ConcurrentWorkers) makeWorkers(spawn Spawn) {
	p.workers = make(chan Worker, p.concurency)
	for i := 0; i < p.concurency; i++ {
		p.workers <- spawn()
	}
}
