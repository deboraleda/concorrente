package main

import (
	"fmt"
	"os"
	"strconv"
	"math/rand"
	"time"
	"sync"
)

var barrier sync.WaitGroup
var wg sync.WaitGroup

func goroutine_func(tid int, send_ch chan int, receive_ch chan int){
	defer wg.Done()
	//Phase one
	time_sleep := rand.Intn(5)
	fmt.Printf("PHASE ONE: Thread %d sleeping for %d seconds\n", tid, time_sleep)
	time.Sleep(time.Duration(time_sleep))
	s := rand.Intn(10)
	send_ch <- s
	close(send_ch)
	fmt.Printf("PHASE ONE: Thread %d DONE! With random_s = %d\n", tid, s)
	barrier.Done()
	
	barrier.Wait()

	//Phase two
	prev_s := <- receive_ch
	fmt.Printf("PHASE TWO: Thread %d sleeping for %d seconds\n", tid, prev_s)
	time.Sleep(time.Duration(prev_s))
	fmt.Printf("PHASE TWO: Thread %d DONE\n", tid)
}

func main(){
	n, _ := strconv.Atoi(os.Args[1])
	rand.Seed(time.Now().UnixNano())

	ch_array := make([]chan int, n)
	for i := range ch_array {
		ch_array[i] = make(chan int, 1)
	}

	barrier.Add(n)
	wg.Add(n)
	for i := 0; i < n; i++ {
		prev_id := (i + n - 1) % n
		receive_ch := ch_array[prev_id];
		send_ch := ch_array[i]
		go goroutine_func(i, send_ch, receive_ch)
	}
	wg.Wait()
}
