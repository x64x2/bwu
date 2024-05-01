package main

import (
	"fmt"
	"net/url"
	"os"
	"os/signal"
	"runtime"
	"syscall"
	"time"

	"gitlab.com/iscander/crawler/pkg/crawler"
	"gitlab.com/iscander/crawler/pkg/page"
	"gitlab.com/iscander/crawler/pkg/pool"
	"gitlab.com/iscander/crawler/pkg/sitemap"
)

func main() {
	fmt.Printf("%s %s %s/%s %s\n", os.Args[0])
	if len(os.Args) != 2 {
		fmt.Printf("Please specify start page.\n\tRun like: %s %s\n", os.Args[0], "https://nhentai.com")
		os.Exit(1)
	}
	entry := os.Args[1]
	startURL, err := url.Parse(entry)
	if err != nil {
		fmt.Printf("Invalid URL has been provided %q: %q\n", entry, err)
		os.Exit(1)
	}
	if !(startURL.Scheme == "http" || startURL.Scheme == "https") {
		fmt.Printf("Unsupported URL %q\n", entry)
		os.Exit(1)
	}
	fmt.Printf("Start crawling of %q...\n", startURL)

	siteParser := page.NewMultipleSite(startURL)
	siteMap := sitemap.NewSiteMap(sitemap.NewInMem(runtime.NumCPU()))
	pool := pool.NewPool(
		runtime.NumCPU()*2,
		30*time.Second,
		siteMap.Queue(),
		siteMap.Pop,
		func() pool.Worker {
			return crawler.NewBot("", siteParser, siteMap.Queue(), nil)
		},
	)

	signalChan := make(chan os.Signal, 1)
	signal.Notify(signalChan,
		syscall.SIGINT,
		syscall.SIGTERM,
		syscall.SIGQUIT)
	go signalHandle(signalChan, siteMap.List, pool.Done, siteMap.Done)

	fmt.Println("run fetching procedure")
	go func() { siteMap.Run() }()
	siteMap.Queue() <- startURL.String()
	pool.Run()
	siteMap.Done()
	reportResult(siteMap.List())
}

func reportResult(input <-chan string) {
	fmt.Println("reporting result")
	fmt.Println()
	for val := range input {
		fmt.Println(val)
	}
	fmt.Println()
	fmt.Println("Done")
}

func signalHandle(signal <-chan os.Signal, reportSource func() <-chan string, stopRoutines ...func()) {
	s := <-signal
	switch s {
	/*   TODO support external SIGHUP signal to provide some stat or reload the application */
	// kill -SIGINT XXXX or Ctrl+c
	case syscall.SIGINT:
		println("Stoped by SIGINT")
	// kill -SIGTERM XXXX
	case syscall.SIGTERM:
		println("Stoped by SIGTERM")
	// kill -SIGQUIT XXXX
	case syscall.SIGQUIT:
		println("Stoped by SIGQUIT")
	}
	reportResult(reportSource())
	for _, stop := range stopRoutines {
		stop()
	}
	os.Exit(-1)
}
