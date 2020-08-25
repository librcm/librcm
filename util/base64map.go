// base64map generate the C map for the rcm_base64 module.
package main

import (
	"fmt"
)

const alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

func main() {
	var decodeMap [256]byte
	for i := 0; i < len(decodeMap); i++ {
		decodeMap[i] = 0xff
	}
	for i := 0; i < len(alpha); i++ {
		decodeMap[alpha[i]] = byte(i)
	}
	fmt.Println("/* generated with base64map.go */")
	fmt.Println("static const unsigned char rcm_base64_map[256] = {")
	for i := 0; i < len(decodeMap); i++ {
		if i > 0 {
			fmt.Print(",")
			if i%12 == 0 {
				fmt.Println("")
			}
		}
		fmt.Printf(" 0x%x", decodeMap[i])
	}
	fmt.Println("\n};")
}
