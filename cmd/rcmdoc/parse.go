package main

import (
	"bytes"
	"fmt"
	"io"
	"path/filepath"
	"regexp"
	"strings"
)

var (
	reHeader = regexp.MustCompile(`(?s)
#ifndef RCM_API
#define RCM_API extern
#endif

/\*
(.+?)
\*/`)

	reDefineGuard = regexp.MustCompile(`/\* (.+) \*/
#ifndef .+?
(#define .+?)
#endif`)

	reDefine = regexp.MustCompile(`/\* (.+) \*/
(#define .+)`)

	reTypedef = regexp.MustCompile(`(?s)/\* (.+) \*/
(typedef .+)`)

	reFunc = regexp.MustCompile(`(?s)/\* (.+) \*/
.*RCM_API (.+?;)`)
)

type definition struct {
	doc  string
	code string
}

type definitions []definition

type docHeader struct {
	filename   string
	moduleName string
	moduleDesc string
	defines    definitions
	typedefs   definitions
	functions  definitions
}

func parseHeaderFile(buf []byte, filename string) (*docHeader, error) {
	dh := &docHeader{filename: filename}

	// find header
	headerFile := string(buf)
	sub := reHeader.FindStringSubmatch(headerFile)
	if len(sub) == 0 {
		return nil, fmt.Errorf("%s: no header found", filename)
	}
	lines := strings.Split(sub[1], "\n")
	dh.moduleName = strings.TrimSuffix(lines[0], ":")
	dh.moduleDesc = strings.Join(lines[1:], "\n")

	blocks := bytes.Split(buf, []byte("\n\n"))
	for _, block := range blocks {
		/*
			fmt.Println("BLOCK")
			fmt.Println(string(block))
		*/

		// find defines with guards
		defGuard := reDefineGuard.FindSubmatch(block)
		if len(defGuard) >= 2 {
			dh.defines = append(dh.defines, definition{doc: string(defGuard[1]), code: string(defGuard[2])})
		}

		// find defines
		def := reDefine.FindSubmatch(block)
		if len(def) >= 2 {
			dh.defines = append(dh.defines, definition{doc: string(def[1]), code: string(def[2])})
		}

		// find typedefs
		typedef := reTypedef.FindSubmatch(block)
		if len(typedef) >= 2 {
			dh.typedefs = append(dh.typedefs, definition{doc: string(typedef[1]), code: string(typedef[2])})
		}

		// find funcs
		fn := reFunc.FindSubmatch(block)
		if len(fn) >= 2 {
			// remove attributes
			funcName := string(fn[2])
			idx := strings.Index(funcName, ")")
			if idx == -1 {
				return nil, fmt.Errorf("')' not found in: %s\n", funcName)
			}
			funcName = funcName[:idx+1]
			lines := strings.Split(funcName, "\n")
			var newLines []string
			for i, line := range lines {
				if i == 0 {
					newLines = append(newLines, line)
				} else {
					newLines = append(newLines, line[8:]) // offset RCM_API
				}
			}
			funcName = strings.Join(newLines, "\n")
			dh.functions = append(dh.functions, definition{doc: string(fn[1]), code: funcName + ";"})
		}
	}

	return dh, nil
}

func (defs definitions) printInfo() {
	for _, def := range defs {
		fmt.Println("DOC")
		fmt.Println(def.doc)
		fmt.Println("CODE")
		fmt.Println(def.code)
	}
}

func (dh *docHeader) printInfo() {
	fmt.Println("MODULE NAME")
	fmt.Println(dh.moduleName)
	fmt.Println("MODULE DESC")
	fmt.Println(dh.moduleDesc)
	fmt.Println("DEFINES")
	dh.defines.printInfo()
	fmt.Println("TYPEDEFS")
	dh.typedefs.printInfo()
	fmt.Println("FUNCTIONS")
	dh.functions.printInfo()
}

func (dh *docHeader) Write(w io.Writer) error {
	base := filepath.Base(dh.filename)

	fmt.Fprintf(w, "// generated from %s with `rcmdoc`\n", "../"+dh.filename)
	fmt.Fprintln(w, "")
	fmt.Fprintf(w, "[[%s]]\n", base)
	fmt.Fprintf(w, "=== `%s` -- %s\n", base, dh.moduleName)
	fmt.Fprintln(w, dh.moduleDesc)
	fmt.Fprintln(w, "")

	// print defines
	if len(dh.defines) > 0 {
		fmt.Fprintf(w, "==== Defines\n\n")
		for _, define := range dh.defines {
			fmt.Fprintln(w, "[source,c]")
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, define.code)
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, "")
			fmt.Fprintln(w, define.doc)
			fmt.Fprintln(w, "")
		}
	}

	// print typedefs
	if len(dh.typedefs) > 0 {
		fmt.Fprintf(w, "==== Typedefs\n\n")
		for _, typedef := range dh.typedefs {
			fmt.Fprintln(w, "[source,c]")
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, typedef.code)
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, "")
			fmt.Fprintln(w, typedef.doc)
			fmt.Fprintln(w, "")
		}
	}

	// print functions
	if len(dh.functions) > 0 {
		fmt.Fprintf(w, "==== Functions\n\n")
		for _, f := range dh.functions {
			fmt.Fprintln(w, "[source,c]")
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, f.code)
			fmt.Fprintln(w, "----")
			fmt.Fprintln(w, "")
			fmt.Fprintln(w, f.doc)
			fmt.Fprintln(w, "")
		}
	}

	// find functions (on reverse buffer)
	/*
		fn := reFunc.FindAllSubmatch(buf, -1)
		for i := len(fn) - 1; i >= 0; i-- {
			if len(fn[i]) > 2 {
				funcName := removeWhitespace(string(reverse(fn[i][1])))
				// remove attributes
				idx := strings.Index(funcName, ")")
				if idx == -1 {
					return fmt.Errorf("')' not found in: %s\n", funcName)
				}
				funcName = funcName[:idx+1]
				fmt.Fprintf(w, "`%s;`::\n", funcName)
				fmt.Fprintln(w, removeWhitespace(string(reverse(fn[i][2]))))
				fmt.Fprintln(w, "")
			}
		}
	*/

	return nil
}
