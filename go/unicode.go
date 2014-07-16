package main

import "fmt"
import "reflect"

func main() {
	a := "√ \u221A \U0000221a"
	fmt.Println(a)

	b := []rune(a)
	fmt.Println(b)

	fmt.Println(len(a))
	fmt.Println(len(b))

	c := string(b)
	fmt.Println(c)

	d := []byte(c)
	fmt.Println(d)

	fmt.Println("type of a: ", reflect.TypeOf(a), " | ", len(a))
	fmt.Println("type of b: ", reflect.TypeOf(b), " | ", len(b))
	fmt.Println("type of c: ", reflect.TypeOf(c), " | ", len(c))
	fmt.Println("type of d: ", reflect.TypeOf(d), " | ", len(d))

	fmt.Println("a[0]: ", a[0], a[1])

	fmt.Println("string(i): ", string(8730), string(0x221a))

	fmt.Println(fmt.Sprint(23))
}
