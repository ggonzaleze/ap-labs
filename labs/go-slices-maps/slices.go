package main

import ("golang.org/x/tour/pic")

func Pic(dx, dy int) [][]uint8 {
	picture := make([][]uint8,dy,dy)
	for i,_ := range picture{
		picture[i] = make([]uint8,dx,dx)
		for j,_ := range picture[i]{
			picture[i][j] = uint8(i) + uint8(j)
		}
	}
	return picture
}

func main() {
	pic.Show(Pic)
}
