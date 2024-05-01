package sitemap

//Queue is an interface which is an adapter between backends and main loop
type Queue interface {
	List() <-chan string
	Queue() chan<- string
	Pop() (string, bool)
	Done()
	Run()
}

//Backend is an interface to store visited links
type Backend interface {
	Next() ([]string, bool)
	Append(string) bool
	Size() uint64
}
