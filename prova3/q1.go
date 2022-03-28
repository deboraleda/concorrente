package main

import (
	"fmt"
	"os"
	"strconv"
	"math/rand"
	"time"
)

var wg sync.WaitGroup

func goroutine_func(tid int){
	defer wg.Done()
	time_sleep := rand.Intn(5)
	fmt.Printf("Thread %d sleeping for %d seconds\n", tid, time_sleep)
	time.Sleep(time.Duration(time_sleep))
	fmt.Printf("Thread %d DONE!\n", tid)
}

func main(){
	n, _ := strconv.Atoi(os.Args[1])
	rand.Seed(time.Now().UnixNano())

	wg.Add(n)
	for i := 0; i < n; i++ {
		go goroutine_func(i)
	}

	wg.Wait()
	fmt.Printf("Thread mãe - o valor de n foi: %d\n", n)
}