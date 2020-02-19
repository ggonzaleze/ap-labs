package main
import "fmt"

func longestSubstring(s string) int{
	indexes := make(map[byte]int)
	i := 0
	currMax := 0
	actMax := 0
	for i < len(s){
		_,found := indexes[s[i]]
		if (found == false){
			currMax = currMax + 1
			indexes[s[i]] = i
		}else{
			if ( currMax > actMax){
				actMax = currMax
			}
			currMax = 0
			i = indexes[s[i]]
			indexes = make(map[byte]int)
		}
		i = i + 1
	}
	if(currMax > actMax){
		actMax = currMax
	}
	return actMax
}

func main(){
	var str string
	fmt.Println("Enter a string:")
	fmt.Scan(&str)
	fmt.Println(longestSubstring(str))
}
