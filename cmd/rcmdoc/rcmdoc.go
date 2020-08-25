package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
)

func genDoc(filename string, verbose bool) error {
	if !strings.HasSuffix(filename, ".h") {
		return fmt.Errorf("not a header file: %s", filename)
	}
	base := filepath.Base(filename)
	buf, err := ioutil.ReadFile(filename)
	if err != nil {
		return err
	}

	dh, err := parseHeaderFile(buf, filename)
	if err != nil {
		return err
	}

	if verbose {
		dh.printInfo()
	}

	// open .adoc file
	docFile := filepath.Join("docs", strings.TrimSuffix(base, ".h")+".adoc")
	fmt.Println("writing", docFile)
	w, err := os.Create(docFile)
	if err != nil {
		return err
	}
	defer w.Close()

	return dh.Write(w)
}

func fatal(err error) {
	fmt.Fprintf(os.Stderr, "%s: error: %s\n", os.Args[0], err)
	os.Exit(1)
}

func main() {
	verbose := flag.Bool("v", false, "Be verbose")
	flag.Parse()
	for _, arg := range flag.Args() {
		if err := genDoc(arg, *verbose); err != nil {
			fatal(err)
		}
	}
}
