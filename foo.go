package main

import (
	"fmt"
)

func main() {
	a := []interface{}{1, "hi"}
	a = append(a, "kljlk")

	for i := 0; i < len(a); i++ {
		fmt.Println(a[i])
	}

	// fmt.Println(a)
}
