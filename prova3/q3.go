

package main
 
import (
   "os"
   "fmt"
   "sync"
   "path/filepath"
)

var wg sync.WaitGroup


func walkDir(dir string, ch chan string){
   defer wg.Done()
   filepath.Walk(dir, func(path string, info os.FileInfo, err error) error{
      if !info.IsDir() {
         ch <- path
      }
      return nil
   })
   
   close(ch)
}


func readFile(ch chan string){
   defer wg.Done()
   for file := range ch {
      f, _ := os.Open(file)

      b1 := make([]byte, 1)
      f.Read(b1)
      

      if (b1[0] % 2) == 0 {
         fmt.Println("File", file)
      }
   }   
}
 

func main() {
   dir := os.Args[1]

   ch := make(chan string)

   wg.Add(2)
   go walkDir(dir, ch)
   go readFile(ch)

   wg.Wait()	 
}
