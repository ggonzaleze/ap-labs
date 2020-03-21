
package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	var directories, symbolics, devices, sockets, others = 0,0,0,0,0
	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		if err != nil{
			fmt.Println("%v",err)
			os.Exit(1)
		}else if info.Mode() & os.ModeDir != 0 {
			directories = directories + 1
		}else if info.Mode() & os.ModeSymlink != 0{
			symbolics = symbolics + 1
		}else if info.Mode() & os.ModeDevice != 0{
			devices = devices + 1
		}else if info.Mode() & os.ModeSocket != 0{
			sockets = sockets + 1
		}else{
			others = others + 1
		}
        	return nil
	})
	if err != nil {
		panic(err)
	}
	fmt.Println("Directory Scanner Tool")
	fmt.Println("+---------------+-------+")
	fmt.Println("|Path           |",dir)
	fmt.Println("+---------------+-------+")
	fmt.Println("|Directories    |", directories)
	fmt.Println("|Symbolic Links |", symbolics)
	fmt.Println("|Devices        |", devices)
	fmt.Println("|Sockets        |", sockets)
	fmt.Println("|Other files    |",others)
	fmt.Println("+---------------+-------+")
	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}else if len(os.Args) == 2{
		scanDir(os.Args[1])
	}else if len(os.Args) == 3{
		scanDir(os.Args[2])
	}
}
