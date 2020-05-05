// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	allUsers map[string]string = make(map[string]string)
	cons map[net.Conn](chan string) = make(map[net.Conn](chan string))
	admin string
)

func localTime() string{
	t := time.Now()
	return t.String()
}

func showUsers() string{
	users := ""
	for k, _ := range allUsers {
    		users = users + k + "  |  "
	}
	return users
}

func findCh(addr string) (chan string){
	for k, v := range cons {
		if k.RemoteAddr().String() == addr{
			return v
		}
	}
	return nil
}

func findCon(user string) net.Conn{
	for k, _ := range cons {
		if k.RemoteAddr().String() == allUsers[user]{
			return k
		}
	}
	return nil
}

func userCommand(in []string, ch (chan string) ) {
	if len(in) < 2{
		ch <- "irc-server > Please enter username"
	}else if allUsers[in[1]]!=""{
		ch <- "irc-server > username: "+ in[1] + ", IP: " + allUsers[in[1]]
	}else{
		ch <- "irc-server > " + "Not found"
	}
}

func msgCommand(in []string, ch (chan string), who string) {
	if len(in) < 3{
		ch <- "irc-server > Please enter username and message"
	}else{
		destCh := findCh(allUsers[in[1]])
		if destCh == nil{
			ch <- "irc-server > Could not send message. Please verify username "
		}else{
			mesg := ""
			for i := 2; i < len(in); i++{
				mesg = mesg + in[i] + " "
			}
			destCh <- who + " private > " + mesg
		}
	}
}

func kickCommand(in []string, ch (chan string), who string) {
	if who != admin{
		ch <- "irc-server > You are not allowed to perform this command"
	}else if len(in) < 2{
		ch <- "irc-server > Please enter username"
	}else{
		destCon := findCon(in[1])
		if destCon == nil{
			ch <- "irc-server > Could not perform action. Please verify username"
		}else{
			fmt.Fprintf(destCon,"irc-server > You're kicked from this channel ")
			delete(allUsers, in[1])
			destCon.Close()
			messages <- "irc-server > " + in[1] + " was kicked from channel"
			fmt.Println("irc-server > " + in[1] + " was kicked")
		}
	}
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	ch <- "irc-server > Welcome to the Simple IRC Server"
	user,_ := bufio.NewReader(conn).ReadString('\000')
	who := strings.TrimSuffix(user,"\000")
	if allUsers[who] != "" {
                fmt.Fprintf(conn,"irc-server > User taken. Please choose another one ")
		conn.Close()
		return
	}
	allUsers[who] = conn.RemoteAddr().String()
	cons[conn] = ch
	ch <- "irc-server > You are " + who
	ch <- "irc-server > Your user "+user+" is successfully logged"
	fmt.Println("irc-server > New connected user "+who)
	if len(allUsers) == 1{
		ch <- "irc-server > Congrats, you were the first user"
		admin = who
		ch <- "irc-server > You're the new IRC Server ADMIN"
		fmt.Println("irc-server > " + who + " was promoted as the channel ADMIN")
	}
	messages <- "irc-server > New connected user " + who
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		if input.Text() == "/time"{
			ch <- "irc-server > Local Time: " + localTime()
		}else if input.Text() == "/users"{
			ch <- "irc-server > " + showUsers()
		}else if in:=strings.Split(input.Text(), " ");in[0]=="/user" {
			userCommand(in, ch)
		}else if in[0]=="/msg"{
			msgCommand(in, ch, who)
		}else if in[0]=="/kick"{
			kickCommand(in, ch, who)
		}else{
			messages <- who + " > " + input.Text()
		}
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	if allUsers[who] != ""{
		delete(allUsers, who)
		fmt.Println("irc-server > " + who + " left")
		messages <- "irc-server > " + who + " left channel"
	}
	if who == admin && len(allUsers) > 0{
		for k,_ := range allUsers{
			admin = k
			break
		}
		messages <- "irc-server > " + admin + " was promoted as the channel ADMIN"
		fmt.Println("irc-server > " + admin + " was promoted as the channel ADMIN")
	}
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	args := os.Args
	server:= args[2]
	port := args[4]
	listener, err := net.Listen("tcp", server+":"+port)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Simple IRC Server started at localhost:9000")
	go broadcaster()
	fmt.Println("irc-server > Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
