package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
)

const (
	licenseIdentifier = "/* SPDX-License-Identifier: Unlicense OR MIT */"
)

func emptyLine(filename string, ln int, line string) error {
	if line != "" {
		return fmt.Errorf("%s: %d: line is not empty\nhave: %s\nwant: %s",
			filename, ln, line, "")
	}
	return nil
}

func checkHeader(filename string, lines []string) error {
	filebase := filepath.Base(filename)
	defineName := strings.Replace(strings.ToUpper(filebase), ".", "_", -1)
	lastLine := len(lines) - 1
	for i, line := range lines {
		ln := i + 1
		switch ln {
		case 3:
			want := "#ifndef " + defineName
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong include guard\nhave: %s\nwant: %s",
					filename, ln, line, want)
			}
		case 4:
			want := "#define " + defineName
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong include guard\nhave: %s\nwant: %s",
					filename, ln, line, want)
			}
		case 5:
			if err := emptyLine(filename, ln, line); err != nil {
				return err
			}
		case 6:
			want := "/* Documentation: https://librcm.org/#" + filebase + " */"
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong documentation line\nhave: %s\nwant: %s",
					filename, ln, line, want)
			}
		case 7:
			if err := emptyLine(filename, ln, line); err != nil {
				return err
			}
		case 8:
			want := "#ifndef RCM_API"
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong RCM_API definition\nhave: %s\nwant: %s",
					filename, ln, line, want)

			}
		case 9:
			want := "#define RCM_API extern"
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong RCM_API definition\nhave: %s\nwant: %s",
					filename, ln, line, want)

			}
		case 10:
			want := "#endif"
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong RCM_API definition\nhave: %s\nwant: %s",
					filename, ln, line, want)

			}
		case lastLine:
			want := "#endif /* " + defineName + " */"
			if line != want {
				return fmt.Errorf("%s: %d: missing/wrong include guard\nhave: %s\nwant: %s",
					filename, ln, line, want)
			}
		}
	}
	return nil
}

func check(filename string) error {
	// read file and split into lines
	buf, err := ioutil.ReadFile(filename)
	if err != nil {
		return err
	}
	lines := strings.Split(string(buf), "\n")
	// all files
	for i, line := range lines {
		ln := i + 1
		switch ln {
		case 1:
			if line != licenseIdentifier {
				return fmt.Errorf("%s: %d: missing/wrong license identifier\nhave: %s\nwant: %s",
					filename, ln, line, licenseIdentifier)
			}
		case 2:
			if err := emptyLine(filename, ln, line); err != nil {
				return err
			}
		default:
			if strings.Contains(line, "int ret") {
				return fmt.Errorf("%s: %d: use `int rc;` instead of `int ret;`", filename, ln)
			}
		}
	}
	// header files only
	var isHeader bool
	if strings.HasSuffix(filename, ".h") {
		isHeader = true
	}
	if isHeader {
		if err := checkHeader(filename, lines); err != nil {
			return err
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
		fmt.Println("checking", arg)
		if err := check(arg); err != nil {
			fatal(err)
		}
	}
}
