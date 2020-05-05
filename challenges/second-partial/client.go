// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
)

//!+
func main() {
	args := os.Args
	user := args[2]
	//fmt.Println("irc-server > Welcome to the Simple IRC Server")
	server := args[4]
	conn, err := net.Dial("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	//fmt.Println("irc-server > Your user "+user+" is successfully logged")
	fmt.Fprintf(conn,user+"\000")
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
