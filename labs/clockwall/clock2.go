// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"math/rand"
	"time"
	"strconv"
	"os"
)

var randomHour string
var timezone string

func handleConn(c net.Conn) {
	timezone = os.Getenv("TZ")
	rand.Seed(time.Now().UnixNano())
	randomHour = strconv.Itoa(rand.Intn(12 - 0) + 0)
	defer c.Close()
	for {
		_, err := io.WriteString(c, timezone+" :\t\t\t"+ randomTimestamp()+"\n")
		if err != nil {
			return // e.g., client disconnected
		}
	time.Sleep(1 * time.Second)
	}
}

func randomTimestamp() string {
	t := time.Now()
   	m :=  strconv.Itoa(t.Minute())
   	s :=  strconv.Itoa(t.Second())
	time := randomHour+":"+m+":"+s
	return time
}

func main() {
	port := os.Args[2]
	listener, err := net.Listen("tcp", "localhost:"+port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
