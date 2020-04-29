package main

import(
        "os"
	"net"
	"log"
	"io"
	"strings"
)

func main(){
        args := os.Args[1:]
	done := make(chan int)
	for _, a := range args{
		server := strings.Split(a, "=")
		conn, err := net.Dial("tcp", server[1])
    		if err != nil {
        		log.Fatal(err)
    		}
    		go func() {
        		io.Copy(os.Stdout, conn)
        		log.Println("done")
        		done <- 1
    		}()
	}
    		x := 1
    		x = <-done
    		log.Println("Channel Closed with value: ", x)
    		close(done)

}
