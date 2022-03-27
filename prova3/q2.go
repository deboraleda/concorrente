package main

import (
	"fmt"
	"os"
	"strconv"
	"math/rand"
	"time"
	"sync"
)

var array_s = []int{}
var n *int;
var mutex = make(chan int, 1)

var barrier sync.WaitGroup
var wg sync.WaitGroup

func goroutine_func(tid int, barrier *sync.WaitGroup, wg *sync.WaitGroup){
	defer wg.Done()
	time_sleep := rand.Intn(5)
	fmt.Printf("PHASE ONE: Thread %d sleeping for %d seconds\n", tid, time_sleep)
	time.Sleep(time.Duration(time_sleep))
	s := rand.Intn(10)
	<- mutex
	array_s = append(array_s, s)
	mutex <- 1
	fmt.Printf("PHASE ONE: Thread %d DONE! With random_s = %d\n", tid, s)
	barrier.Done()
	
	barrier.Wait()

	prev_id := (tid + *n - 1) % *n
	<- mutex
	prev_s := array_s[prev_id]
	mutex <- 1

	fmt.Printf("PHASE TWO: Thread %d sleeping for %d seconds\n", tid, prev_s)
	time.Sleep(time.Duration(prev_s))
	fmt.Printf("PHASE TWO: Thread %d DONE\n", tid)
}

func main() {
	x, _ := strconv.Atoi(os.Args[1])
	rand.Seed(time.Now().UnixNano())
	mutex <- 1

	n = &x

	barrier.Add(*n)
	wg.Add(*n)
	for i := 0; i < *n; i++ {
		go goroutine_func(i, &barrier, &wg)
	}

	wg.Wait()
}
