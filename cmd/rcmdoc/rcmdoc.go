package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"strings"
)

var (
	reHeader = regexp.MustCompile(`(?s)
#ifndef RCM_API
#define RCM_API extern
#endif

/\* (.+?) \*/`)

	reFunc = regexp.MustCompile(`(?s)
(;.+?) IPA_MCR
/\* (.+?) \*/`)
)

func reverse(buf []byte) []byte {
	for i, j := 0, len(buf)-1; i < j; i, j = i+1, j-1 {
		buf[i], buf[j] = buf[j], buf[i]
	}
	return buf
}

func removeWhitespace(s string) string {
	return strings.Join(strings.Fields(strings.ReplaceAll(strings.TrimSpace(s), "\n", "")), " ")
}

func genDoc(filename string) error {
	if !strings.HasSuffix(filename, ".h") {
		return fmt.Errorf("not a header file: %s", filename)
	}
	base := filepath.Base(filename)
	buf, err := ioutil.ReadFile(filename)
	if err != nil {
		return err
	}

	// find header
	headerFile := string(buf)
	sub := reHeader.FindStringSubmatch(headerFile)
	if len(sub) == 0 {
		return fmt.Errorf("%s: no header found", filename)
	}

	// open .adoc file
	docFile := filepath.Join("docs", strings.TrimSuffix(base, ".h")+".adoc")
	fmt.Println("writing", docFile)
	w, err := os.Create(docFile)
	if err != nil {
		return err
	}
	defer w.Close()

	lines := strings.Split(sub[1], "\n")
	header := strings.Join(lines[1:], "")

	fmt.Fprintf(w, "// generated from %s with `rcmdoc`\n", "../"+filename)
	fmt.Fprintln(w, "")
	fmt.Fprintf(w, "[[%s]]\n", base)
	fmt.Fprintf(w, "=== `%s` -- %s\n", base, strings.TrimSuffix(lines[0], ":"))
	fmt.Fprintln(w, "")
	fmt.Fprintln(w, removeWhitespace(header))
	fmt.Fprintln(w, "")

	// find functions (on reverse buffer)
	reverse(buf)
	//fmt.Println(string(buf))
	fn := reFunc.FindAllSubmatch(buf, -1)
	for i := len(fn) - 1; i >= 0; i-- {
		funcName := removeWhitespace(string(reverse(fn[i][1])))
		// remove attributes
		idx := strings.Index(funcName, ")")
		if idx == -1 {
			return fmt.Errorf("')' not found in: %s\n", funcName)
		}
		funcName = funcName[:idx] + ";"
		if len(fn[i]) > 2 {
			fmt.Fprintf(w, "`%s;`::\n", funcName)
			fmt.Fprintln(w, removeWhitespace(string(reverse(fn[i][2]))))
			fmt.Fprintln(w, "")
		}
	}
	return nil
}

func fatal(err error) {
	fmt.Fprintf(os.Stderr, "%s: error: %s\n", os.Args[0], err)
	os.Exit(1)
}

func main() {
	for i, arg := range os.Args {
		if i == 0 {
			continue
		}
		if err := genDoc(arg); err != nil {
			fatal(err)
		}
	}
}
