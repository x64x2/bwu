package pool

//Worker an interface which provides an ability to fetch pages
type Worker interface {
	Get(url string)
}

// Spawn a function which spawn new instance of page fetcher(Worker)
type Spawn func() Worker
