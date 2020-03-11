package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	counts := make(map[string]int)
	words := strings.Split(s, " ")
	for _,w := range words{
		counts[w] = counts[w] + 1
	}
	return counts
}

func main() {
	wc.Test(WordCount)
}

